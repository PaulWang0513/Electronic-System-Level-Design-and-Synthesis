#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
#include <sysc/datatypes/fx/sc_ufixed.h>
#include "datatypes.h"
#include "ready_valid_port.h"

using namespace sc_core;
using namespace sc_dt;

class Testbench : public sc_module {
public:
    sc_in_clk i_clk;
    sc_out<bool> o_rst;
    rdyvld_port_out<float> o_data;
    rdyvld_port_in<float> i_result;
    
    SC_HAS_PROCESS(Testbench);

    Testbench(sc_module_name n);
    ~Testbench() = default;

    float y_output_signal[64];
private:
    int result_idx;
    void do_decimation_filter();
    void sink();
};
#endif
