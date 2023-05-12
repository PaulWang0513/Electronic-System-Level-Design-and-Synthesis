#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), median_mean_filter("median_mean_filter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	median_mean_filter.i_clk(clk);
	median_mean_filter.i_rst(rst);

    tb.o_rgb_median(rgb_median);
    tb.i_result_median(result_median);
	median_mean_filter.i_rgb_median(rgb_median);
	median_mean_filter.o_result_median(result_median);

    // tb.o_rgb_mean(rgb_mean);
    // tb.i_result_mean(result_mean);
    // median_mean_filter.i_rgb_mean(rgb_mean);
    // median_mean_filter.o_result_mean(result_mean);

    tb.read_bmp(input_bmp);
}

System::~System() {
    tb.write_bmp(_output_bmp);
}
