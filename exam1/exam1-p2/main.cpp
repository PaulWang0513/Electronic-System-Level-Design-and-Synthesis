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

    //Create FIFO channels
    sc_fifo<float> data;
    sc_fifo<float> result;

    //Connect channels with modules
    tb.i_clk(clk);
    tb.o_rst(rst);
    decimation_filter.i_clk(clk);
    decimation_filter.i_rst(rst);
    tb.o_data(data);
    tb.i_result(result);
    decimation_filter.i_data(data);
    decimation_filter.o_result(result);

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
