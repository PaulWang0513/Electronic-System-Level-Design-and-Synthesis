#ifndef AUTOCORRELATION_FUNCTION_H_
#define AUTOCORRELATION_FUNCTION_H_

#include <systemc>
// #include <cynw_p2p.h>
#include "def.h"
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
using namespace sc_core;

// change this to select the implementation to do simulation or synthesis
// avaliable options: BASIC, MEM_1000X2, MEM_500X2, MEM_250X4, MEM_250X4_AREA
// #define MEM_250X4_AREA

class AutocorrelationFunction : public sc_module {
public:
    tlm_utils::simple_target_socket<AutocorrelationFunction> tsock;

    sc_fifo<sc_dt::sc_uint<8>> i_data;
    sc_fifo<sc_dt::sc_uint<26>> o_result;
    // cynw_p2p<sc_dt::sc_uint<8>>::in   i_data;
    // cynw_p2p<sc_dt::sc_uint<26>>::out o_result;

    SC_HAS_PROCESS(AutocorrelationFunction);

    AutocorrelationFunction(sc_module_name n) : 
        sc_module(n), tsock("t_skt") 
    {
        tsock.register_b_transport(this, &AutocorrelationFunction::blocking_transport);
        SC_THREAD(do_ACF);
    }
    
    ~AutocorrelationFunction() {
    }

    unsigned char mem0[250];
    unsigned char mem1[250];
    unsigned char mem2[250];
    unsigned char mem3[250];

    void do_ACF() {
        { wait(CLOCK_PERIOD, SC_NS); }

        // array initialize
        for (unsigned int i=0; i<250; i++) {
            mem0[i] = 0;
            mem1[i] = 0;
            mem2[i] = 0;
            mem3[i] = 0;
            wait(CLOCK_PERIOD, SC_NS);
        }

        // main loop
        while (true) {
            // ========== read input ==========
            for (unsigned int i=0; i<250; i++) {
                mem0[i] = i_data.read();
                wait(CLOCK_PERIOD, SC_NS);
            }
            for (unsigned int i=0; i<250; i++) {
                mem1[i] = i_data.read();
                wait(CLOCK_PERIOD, SC_NS);
            }
            for (unsigned int i=0; i<250; i++) {
                mem2[i] = i_data.read();
                wait(CLOCK_PERIOD, SC_NS);
            }
            for (unsigned int i=0; i<250; i++) {
                mem3[i] = i_data.read();
                wait(CLOCK_PERIOD, SC_NS);
            }

            // ========== calculate ACF ==========
            // result 0 ~ 499
            for (unsigned int lag=0; lag<500; lag++) {
                sc_dt::sc_uint<26> sum = 0;
                for (unsigned int idx=0; idx<250; idx++) {
                    unsigned int original_signal_0 = mem0[idx];
                    unsigned int original_signal_1 = mem1[idx];
                    unsigned int original_signal_2 = mem2[idx];
                    unsigned int original_signal_3 = mem3[idx];
                    wait(CLOCK_PERIOD, SC_NS);

                    unsigned int lagged_signal_0, lagged_signal_1, lagged_signal_2, lagged_signal_3;
                    if (idx+lag < 250) {
                        unsigned int temp = idx+lag;
                        lagged_signal_0 = mem0[temp];
                        lagged_signal_1 = mem1[temp];
                        lagged_signal_2 = mem2[temp];
                        lagged_signal_3 = mem3[temp];
                    } else if (idx+lag < 500) {
                        unsigned int temp = idx+lag-250;
                        lagged_signal_0 = mem1[temp];
                        lagged_signal_1 = mem2[temp];
                        lagged_signal_2 = mem3[temp];
                        lagged_signal_3 = 0;
                    } else {
                        unsigned int temp = idx+lag-500;
                        lagged_signal_0 = mem2[temp];
                        lagged_signal_1 = mem3[temp];
                        lagged_signal_2 = 0;
                        lagged_signal_3 = 0;
                    }
                    wait(CLOCK_PERIOD, SC_NS);

                    unsigned int temp_0 = original_signal_0 * lagged_signal_0;
                    wait(CLOCK_PERIOD, SC_NS);
                    unsigned int temp_1 = original_signal_1 * lagged_signal_1;
                    wait(CLOCK_PERIOD, SC_NS);
                    unsigned int temp_2 = original_signal_2 * lagged_signal_2;
                    wait(CLOCK_PERIOD, SC_NS);
                    unsigned int temp_3 = original_signal_3 * lagged_signal_3;
                    wait(CLOCK_PERIOD, SC_NS);
                    sum += temp_0 + temp_1 + temp_2 + temp_3;
                    wait(CLOCK_PERIOD, SC_NS);
                }
                sum /= SIGNAL_LEN;
                wait(CLOCK_PERIOD, SC_NS);
                o_result.write(sum);
                wait(CLOCK_PERIOD, SC_NS);
            }
            // result 500 ~ 749
            for (unsigned int lag=0; lag<500; lag++) {
                sc_dt::sc_uint<26> sum = 0;
                for (unsigned int idx=0; idx<250; idx++) {
                    unsigned int original_signal_0 = mem0[idx];
                    unsigned int original_signal_1 = mem1[idx];
                    wait(CLOCK_PERIOD, SC_NS);

                    unsigned int lagged_signal_2, lagged_signal_3;
                    if (idx+lag < 250) {
                        unsigned int temp = idx+lag;
                        lagged_signal_2 = mem2[temp];
                        lagged_signal_3 = mem3[temp];
                    } else if (idx+lag < 500) {
                        unsigned int temp = idx+lag-250;
                        lagged_signal_2 = mem3[temp];
                        lagged_signal_3 = 0;
                    } else {
                        lagged_signal_2 = 0;
                        lagged_signal_3 = 0;
                    }
                    wait(CLOCK_PERIOD, SC_NS);

                    unsigned int temp_0 = original_signal_0 * lagged_signal_2;
                    wait(CLOCK_PERIOD, SC_NS);
                    unsigned int temp_1 = original_signal_1 * lagged_signal_3;
                    wait(CLOCK_PERIOD, SC_NS);
                    sum += temp_0 + temp_1;
                    wait(CLOCK_PERIOD, SC_NS);
                }
                sum /= SIGNAL_LEN;
                wait(CLOCK_PERIOD, SC_NS);
                o_result.write(sum);
                wait(CLOCK_PERIOD, SC_NS);
            }
        }
    }

    void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay){
        wait(delay);
        // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
        // auto len = payload.get_data_length();
        tlm::tlm_command cmd = payload.get_command();
        sc_dt::uint64 addr = payload.get_address();
        unsigned char *data_ptr = payload.get_data_ptr();

        // addr -= base_offset;
        // cout << (int)data_ptr[0] << endl;
        // cout << (int)data_ptr[1] << endl;
        // cout << (int)data_ptr[2] << endl;
        word buffer;

        switch (cmd) {
        case tlm::TLM_READ_COMMAND:
            switch (addr) {
            case ACF_RESULT_ADDR:
                buffer.uint = o_result.read();
                break;
            default:
                std::cerr << "READ Error! MedianMeanFilter::blocking_transport: address 0x"
                        << std::setfill('0') << std::setw(8) << std::hex << addr
                        << std::dec << " is not valid" << std::endl;
            }
            data_ptr[0] = buffer.uc[0];
            data_ptr[1] = buffer.uc[1];
            data_ptr[2] = buffer.uc[2];
            data_ptr[3] = buffer.uc[3];
            break;
        case tlm::TLM_WRITE_COMMAND:
            switch (addr) {
            case ACF_R_ADDR:
                i_data.write(data_ptr[0]);
                break;
            default:
                std::cerr << "WRITE Error! MedianMeanFilter::blocking_transport: address 0x"
                        << std::setfill('0') << std::setw(8) << std::hex << addr
                        << std::dec << " is not valid" << std::endl;
            }
            break;
        case tlm::TLM_IGNORE_COMMAND:
            payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            return;
        default:
            payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            return;
        }
        payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
    }

};

#endif
