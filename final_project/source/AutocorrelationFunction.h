#ifndef AUTOCORRELATION_FUNCTION_H_
#define AUTOCORRELATION_FUNCTION_H_

#include <systemc>
#include <cynw_p2p.h>
#include "def.h"
using namespace sc_core;

class AutocorrelationFunction : public sc_module {
public:
    sc_in_clk i_clk;
    sc_in<bool> i_rst;
    cynw_p2p<sc_dt::sc_uint<8>>::in   i_data;
    cynw_p2p<sc_dt::sc_uint<26>>::out o_result;

    SC_HAS_PROCESS(AutocorrelationFunction);
    AutocorrelationFunction(sc_module_name n);
    ~AutocorrelationFunction();

private:
    void do_ACF();
    unsigned char mem[SIGNAL_LEN];
};
#endif
