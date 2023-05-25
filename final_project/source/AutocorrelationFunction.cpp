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
    for (unsigned int i=0; i<SIGNAL_LEN; i++) {
        mem[i] = 0;
    }

    unsigned int idx = 0;
    // main loop, currently just add 1 to each input data
    while (true) {
        // read input
        sc_dt::sc_uint<8> input_data;
        {
            HLS_DEFINE_PROTOCOL("input");
            input_data = i_data.get();
            wait();
        }
        mem[idx] = input_data + 1;
    
        // output result
        sc_dt::sc_uint<26> output_data = mem[idx];
        {
			HLS_DEFINE_PROTOCOL("output");
            o_result.put(output_data);
			wait();
		}
        idx++;
    }
}