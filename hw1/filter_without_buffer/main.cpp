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
    cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name"
         << endl;
    return 0;
  }

  //Create modules and signals
  Testbench tb("tb");
  MedianMeanFilter median_mean_filter("median_mean_filter");
  sc_clock clk("clk", CLOCK_PERIOD, SC_NS);
  sc_signal<bool> rst("rst");
  sc_signal<bool> en_median("en_median");
  sc_signal<bool> en_mean("en_mean");

  //Create FIFO channels
  sc_fifo<unsigned char> r;
  sc_fifo<unsigned char> g;
  sc_fifo<unsigned char> b;
  sc_fifo<int> result;

  //Connect FIFO channels with modules
  tb.i_clk(clk);
  tb.o_rst(rst);
  tb.o_en_median(en_median);
  tb.o_en_mean(en_mean);
  median_mean_filter.i_clk(clk);
  median_mean_filter.i_rst(rst);
  median_mean_filter.i_en_median(en_median);
  median_mean_filter.i_en_mean(en_mean);
  tb.o_r(r);
  tb.o_g(g);
  tb.o_b(b);
  tb.i_result(result);
  median_mean_filter.i_r(r);
  median_mean_filter.i_g(g);
  median_mean_filter.i_b(b);
  median_mean_filter.o_result(result);

  cout << "input file name : " << argv[1] << endl;
  tb.read_bmp(argv[1]);
  sc_start();
  std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
  tb.write_bmp(argv[2]);

  return 0;
}
