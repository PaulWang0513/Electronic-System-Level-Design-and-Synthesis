#include <cmath>
#include "stratus_hls.h"
#include "AutocorrelationFunction.h"
// #include "cynw_utilities.h"

AutocorrelationFunction::AutocorrelationFunction(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_ACF);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

    i_data.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
}

AutocorrelationFunction::~AutocorrelationFunction() {}

void AutocorrelationFunction::do_ACF() {
    {
        HLS_DEFINE_PROTOCOL("reset");
        i_data.reset();
        o_result.reset();
        wait();
    }

    // array initialize
#if defined(MEM_1000X2)
    for (unsigned int i=0; i<SIGNAL_LEN; i++) {
        mem0[i] = 0;
        mem1[i] = 0;
    }
#elif defined(MEM_500X2)
    for (unsigned int i=0; i<500; i++) {
        mem0[i] = 0;
        mem1[i] = 0;
    }
#else
    for (unsigned int i=0; i<SIGNAL_LEN; i++) {
        mem[i] = 0;
    }
#endif

    // main loop
    while (true) {

        // ========== read input ==========
#if defined(MEM_1000X2)
        for (unsigned int i=0; i<SIGNAL_LEN; i++) {
            {
                HLS_DEFINE_PROTOCOL("input");
                unsigned int data = i_data.get();
                mem0[i] = data;
                mem1[i] = data;
                wait();
            }
        }
#elif defined(MEM_500X2)
        for (unsigned int i=0; i<500; i++) {
            {
                HLS_DEFINE_PROTOCOL("input");
                mem0[i] = i_data.get();
                wait();
            }
        }
        for (unsigned int i=0; i<500; i++) {
            {
                HLS_DEFINE_PROTOCOL("input");
                mem1[i] = i_data.get();
                wait();
            }
        }
#else
        for (unsigned int i=0; i<SIGNAL_LEN; i++) {
            {
                HLS_DEFINE_PROTOCOL("input");
                mem[i] = i_data.get();
                wait();
            }
        }
#endif

        // ========== calculate ACF ==========
#if defined(MEM_1000X2)
        for (unsigned int lag=0; lag<SIGNAL_LEN; lag++) {
            sc_dt::sc_uint<26> sum = 0;
            for (unsigned int idx=0; idx<SIGNAL_LEN; idx++) {
                unsigned int original_signal = mem0[idx];
                unsigned int lagged_signal = (idx+lag < SIGNAL_LEN) ? mem1[idx+lag] : 0;
                sum += original_signal * lagged_signal;
            }
            sum /= SIGNAL_LEN;
            {
                HLS_DEFINE_PROTOCOL("output");
                o_result.put(sum);
                wait();
            }
        }
#elif defined(MEM_500X2)
        // the first 500 output
        for (unsigned int lag=0; lag<500; lag++) {
            sc_dt::sc_uint<26> sum = 0;
            for (unsigned int idx=0; idx<500; idx++) {
                unsigned int original_signal_0 = mem0[idx];
                unsigned int original_signal_1 = mem1[idx];
                unsigned int lagged_signal_0 = (idx+lag < 500) ? mem0[idx+lag] : mem1[idx+lag-500];
                unsigned int lagged_signal_1 = (idx+lag < 500) ? mem1[idx+lag] : 0;
                unsigned int temp_0 = original_signal_0 * lagged_signal_0;
                unsigned int temp_1 = original_signal_1 * lagged_signal_1;
                sum += temp_0 + temp_1;
            }
            sum /= SIGNAL_LEN;
            {
                HLS_DEFINE_PROTOCOL("output");
                o_result.put(sum);
                wait();
            }
        }
        // the second 500 output
        for (unsigned int lag=0; lag<500; lag++) {
            sc_dt::sc_uint<26> sum = 0;
            for (unsigned int idx=0; idx<500; idx++) {
                unsigned int original_signal_0 = mem0[idx];
                unsigned int lagged_signal_1 = (idx+lag < 500) ? mem1[idx+lag] : 0;
                sum += original_signal_0 * lagged_signal_1;
            }
            sum /= SIGNAL_LEN;
            {
                HLS_DEFINE_PROTOCOL("output");
                o_result.put(sum);
                wait();
            }
        }
#else
        for (unsigned int lag=0; lag<SIGNAL_LEN; lag++) {
            sc_dt::sc_uint<26> sum = 0;
            for (unsigned int idx=0; idx<SIGNAL_LEN; idx++) {
                unsigned int original_signal = mem[idx];
                unsigned int lagged_signal = (idx+lag < SIGNAL_LEN) ? mem[idx+lag] : 0;
                sum += original_signal * lagged_signal;
            }
            sum /= SIGNAL_LEN;
            {
                HLS_DEFINE_PROTOCOL("output");
                o_result.put(sum);
                wait();
            }
        }
#endif

    }
}
