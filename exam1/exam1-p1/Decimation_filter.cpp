#include <cmath>
#include "Decimation_filter.h"

const float mask[3] = {float(1)/float(2), float(1)/float(3), float(1)/float(6)};

DecimationFilter::DecimationFilter(sc_module_name n)
    : sc_module(n), M(2), buffer(0) {
    SC_THREAD(do_filter);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);
    // std::cout << mask[0] << " " << mask[1] << " " << mask[2] << std::endl;
}

void DecimationFilter::do_filter() {
    float input_0 = 0;
    float input_1 = 0;
    float input_2 = 0;

    while (true) {
        // get first input from buffer and FIFO
        input_0 = buffer;
        input_1 = i_data.read();
        wait(1);
        input_2 = i_data.read();
        wait(1);

        // buffer for next iteration
        buffer = input_2;

        psum = 0;
        // do filter
        psum += input_2 * mask[0];
        wait(1);
        psum += input_1 * mask[1];
        wait(1);
        psum += input_0 * mask[2];
        wait(1);
        // std::cout << "filter: " << input_2 << " " << input_1 << " " << input_0 << " " << psum << std::endl;

        // output
        o_result.write(psum);
    }
}
