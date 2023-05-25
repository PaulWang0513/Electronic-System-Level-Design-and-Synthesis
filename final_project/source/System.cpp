#include "System.h"
System::System( sc_module_name n, string input_file_name, string golden_file_name ): sc_module( n ), 
	tb("tb"), ACF("ACF"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	ACF.i_clk(clk);
	ACF.i_rst(rst);

    tb.o_data(data);
    tb.i_result(result);
	ACF.i_data(data);
	ACF.o_result(result);

    tb.read_input_and_golden(input_file_name, golden_file_name);
}

System::~System() {
}
