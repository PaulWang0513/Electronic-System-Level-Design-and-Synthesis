#ifndef AUTOCORRELATION_FUNCTION_H_
#define AUTOCORRELATION_FUNCTION_H_

#include <systemc>
#include <cynw_p2p.h>
#include "def.h"
using namespace sc_core;

// change this to select the implementation to do simulation or synthesis
// avaliable options: BASIC, MEM_1000X2, MEM_500X2, MEM_250X4
#define MEM_250X4

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
#if defined(MEM_1000X2)
    unsigned char mem0[SIGNAL_LEN];
    unsigned char mem1[SIGNAL_LEN];
#elif defined(MEM_500X2)
    unsigned char mem0[500];
    unsigned char mem1[500];
#elif defined(MEM_250X4)
    unsigned char mem0[250];
    unsigned char mem1[250];
    unsigned char mem2[250];
    unsigned char mem3[250];
#else
    unsigned char mem[SIGNAL_LEN];
#endif

};
#endif
