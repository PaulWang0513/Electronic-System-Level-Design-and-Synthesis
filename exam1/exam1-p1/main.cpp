#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "Decimation_filter.h"
#include "Testbench.h"
#include <iomanip>

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv) {
    // Create modules and signals
    Testbench tb("tb");
    DecimationFilter decimation_filter("decimation_filter");
    sc_clock clk("clk", CLOCK_PERIOD, SC_NS);
    sc_signal<bool> rst("rst");

    sc_signal<float> s_i_data_msg;
    sc_signal<bool> s_i_data_rdy;
    sc_signal<bool> s_i_data_vld;
    sc_signal<float> s_o_result_msg;
    sc_signal<bool> s_o_result_rdy;
    sc_signal<bool> s_o_result_vld;

    //Connect channels with modules
    tb.i_clk(clk);
    tb.o_rst(rst);
    decimation_filter.i_clk(clk);
    decimation_filter.i_rst(rst);
    
    tb.o_data(data);
    tb.i_result(result);
    decimation_filter.i_data(data);
    decimation_filter.o_result(result);

    testbench.o_data.msg(s_i_data_msg);
    testbench.o_data.vld(s_i_data_vld);
    testbench.o_data.rdy(s_i_data_rdy);
    testbench.i_result.msg(s_o_result_msg);
    testbench.i_result.vld(s_o_result_vld);
    testbench.i_result.rdy(s_o_result_rdy);
    decimation_filter.i_data.msg(s_i_data_msg);
    decimation_filter.i_data.vld(s_i_data_vld);
    decimation_filter.i_data.rdy(s_i_data_rdy);
    decimation_filter.o_result.msg(s_o_result_msg);
    decimation_filter.o_result.vld(s_o_result_vld);
    decimation_filter.o_result.rdy(s_o_result_rdy);
    std::cout << "Simulated start" << std::endl;
    sc_start();

    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            std::cout << std::setw(10) << tb.y_output_signal[i*8+j];
        }
        std::cout << std::endl;
    }
    std::cout << "Simulated done" << std::endl;

    std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;

    return 0;
}
