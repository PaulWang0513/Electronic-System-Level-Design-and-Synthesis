#ifndef DECIMATION_FILTER_H_
#define DECIMATION_FILTER_H_
#include <systemc>
#include <sysc/datatypes/fx/sc_ufixed.h>
#include "datatypes.h"
#include "ready_valid_port.h"
using namespace sc_core;
using namespace sc_dt;

class DecimationFilter : public sc_module {
public:
    sc_in_clk i_clk;
    sc_in<bool> i_rst;
    rdyvld_port_in<float> i_data;
    rdyvld_port_out<float> o_result;

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
