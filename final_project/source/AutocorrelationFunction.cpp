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

    // main loop
    while (true) {
        // read input of size SIGNAL_LEN
        for (unsigned int i=0; i<SIGNAL_LEN; i++) {
            {
                HLS_DEFINE_PROTOCOL("input");
                mem[i] = i_data.get();
                wait();
            }
        }
        
        // calculate ACF
        for (unsigned int lag=0; lag<SIGNAL_LEN; lag++) {
            sc_dt::sc_uint<26> sum = 0;
            for (unsigned int i=0; i<SIGNAL_LEN-lag; i++) {
                sum += mem[i] * mem[i+lag];
            }
            sum /= SIGNAL_LEN;
            {
                HLS_DEFINE_PROTOCOL("output");
                o_result.put(sum);
                wait();
            }
        }
    }
}

// area: 4477.2
// total run time: 15074990