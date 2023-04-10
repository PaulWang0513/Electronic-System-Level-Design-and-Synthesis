#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "MedianMeanFilter.h"
#include "Testbench.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv) {
    if ((argc < 3) || (argc > 4)) {
        cout << "No arguments for the executable : " << argv[0] << endl;
        cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name" << endl;
        return 0;
    }
    cout << "ouo" << endl;

    //Create modules and signals
    Testbench tb("tb");
    MedianMeanFilter median_mean_filter("median_mean_filter");
    sc_clock clk("clk", CLOCK_PERIOD, SC_NS);
    sc_signal<bool> rst("rst");

    //Create FIFO channels
    sc_fifo<unsigned char> r_median;
    sc_fifo<unsigned char> g_median;
    sc_fifo<unsigned char> b_median;
    sc_fifo<int> result_median;
    sc_fifo<unsigned char> r_mean;
    sc_fifo<unsigned char> g_mean;
    sc_fifo<unsigned char> b_mean;
    sc_fifo<int> result_mean;

    //Connect channels with modules
    tb.i_clk(clk);
    tb.o_rst(rst);
    
    median_mean_filter.i_clk(clk);
    median_mean_filter.i_rst(rst);

    tb.o_r_median(r_median);
    tb.o_g_median(g_median);
    tb.o_b_median(b_median);
    tb.i_result_median(result_median);
    tb.o_r_mean(r_mean);
    tb.o_g_mean(g_mean);
    tb.o_b_mean(b_mean);
    tb.i_result_mean(result_mean);

    median_mean_filter.i_r_median(r_median);
    median_mean_filter.i_g_median(g_median);
    median_mean_filter.i_b_median(b_median);
    median_mean_filter.o_result_median(result_median);
    median_mean_filter.i_r_mean(r_mean);
    median_mean_filter.i_g_mean(g_mean);
    median_mean_filter.i_b_mean(b_mean);
    median_mean_filter.o_result_mean(result_mean);

    cout << "input file name : " << argv[1] << endl;
    tb.read_bmp(argv[1]);
    sc_start();
    std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
    tb.write_bmp(argv[2]);

    return 0;
}
