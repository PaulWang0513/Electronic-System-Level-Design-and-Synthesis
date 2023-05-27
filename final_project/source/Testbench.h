#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#include <cynw_p2p.h>
#include "def.h"

class Testbench : public sc_module {
public:
    sc_in_clk i_clk;
    sc_out<bool> o_rst;
    cynw_p2p<sc_dt::sc_uint<8>>::base_out o_data;
    cynw_p2p<sc_dt::sc_uint<26>>::base_in i_result;

    SC_HAS_PROCESS(Testbench);

    Testbench(sc_module_name n);
    ~Testbench();

    // Utility functions
    void read_input_and_golden(string input_file_name, string golden_file_name);
    void validate();
    int clock_cycle( sc_time time ); // to convert a time in simulation time units to clock cycles

private:
    unsigned char *input_data;
    unsigned int *golden_data;
    unsigned int *result_data;

	sc_time total_start_time;
	sc_time total_run_time;
    unsigned int latency_sum;
    
    void feed_data();
	void fetch_result();
};
#endif
