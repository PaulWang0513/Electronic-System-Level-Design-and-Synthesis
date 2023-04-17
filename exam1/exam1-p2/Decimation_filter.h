#ifndef DECIMATION_FILTER_H_
#define DECIMATION_FILTER_H_
#include <systemc>
#include <sysc/datatypes/fx/sc_ufixed.h>
#include "datatypes.h"
using namespace sc_core;

class DecimationFilter : public sc_module {
public:
    sc_in_clk i_clk;
    sc_in<bool> i_rst;
    sc_fifo_in<float> i_data;
    sc_fifo_out<float> o_result;

    SC_HAS_PROCESS(DecimationFilter);
    DecimationFilter(sc_module_name n);
    ~DecimationFilter() = default;

private:
    int M;
    void do_filter();
    float psum;
    float buffer;
};
#endif
