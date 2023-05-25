#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#include "AutocorrelationFunction_wrap.h"

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n, std::string input_file_name, std::string golden_file_name );
	~System();
private:
    Testbench tb;
	AutocorrelationFunction_wrapper ACF;

	sc_clock clk;
	sc_signal<bool> rst;
	cynw_p2p< sc_dt::sc_uint<8>> data;
	cynw_p2p< sc_dt::sc_uint<26>> result;
};
#endif
