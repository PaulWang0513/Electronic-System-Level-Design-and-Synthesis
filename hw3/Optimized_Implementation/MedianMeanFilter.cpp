#include <cmath>
#include "stratus_hls.h"
#include "MedianMeanFilter.h"
#include "cynw_utilities.h"

MedianMeanFilter::MedianMeanFilter(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_median_filter);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

    SC_THREAD(do_mean_filter);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

    i_rgb_median.clk_rst(i_clk, i_rst);
    o_result_median.clk_rst(i_clk, i_rst);
    i_rgb_mean.clk_rst(i_clk, i_rst);
    o_result_mean.clk_rst(i_clk, i_rst);
}

MedianMeanFilter::~MedianMeanFilter() {}

void MedianMeanFilter::do_median_filter() {
    {
        HLS_DEFINE_PROTOCOL("median_reset");
        i_rgb_median.reset();
        o_result_median.reset();
        wait();
    }

    // array initialize
    for (unsigned int i=0; i<9; i++) {
        val_median[i] = 0;
    }
    for (unsigned int i=0; i<2; i++) {
        for (unsigned int j=0; j<3; j++) {
            for (unsigned int k=0; k<3; k++) {
                buffer[i][j][k] = 0;
            }
        }
    }

    // try to get the first 2x3 pixels
    for (unsigned int v = 1; v < 3; ++v) {
        for (unsigned int u = 0; u < 3; ++u) {
            sc_dt::sc_uint<24> rgb;
            {
                HLS_DEFINE_PROTOCOL("median_input_1");
                rgb = i_rgb_median.get();
                wait();
            }
            unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
            buffer[1][v][u] = grey;
        }
    }

    unsigned int col = 0;    // to record the column of current pixel
    while (true) {
        // get all pixels in the filter
        if (col == 0) { // first column
            // get pixels from buffer, and the bottom most pixels from input
            for (unsigned int v = 0; v < 2; ++v) {
                for (unsigned int u = 0; u < 3; ++u) {
                    buffer[1][v][u] = buffer[1][v+1][u];
                    buffer[0][v][u] = buffer[1][v+1][u];
                    val_median[v*3+u] = buffer[1][v+1][u];
                }
            }
            for (unsigned int u = 0; u < 3; ++u) {
                sc_dt::sc_uint<24> rgb;
                {
                    HLS_DEFINE_PROTOCOL("median_input_2");
                    rgb = i_rgb_median.get();
                    wait();
                }
                unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
                buffer[1][2][u] = grey;
                buffer[0][2][u] = grey;  // for the first column of next row
                val_median[6+u] = grey;
            }
        } else {    // other columns
            // get pixels from buffer, and the right most pixels from input
            for (unsigned int v = 0; v < 3; ++v) {
                for (unsigned int u = 0; u < 2; ++u) {
                    buffer[0][v][u] = buffer[0][v][u+1];
                    val_median[v*3+u] = buffer[0][v][u+1];
                }
                sc_dt::sc_uint<24> rgb;
                {
                    HLS_DEFINE_PROTOCOL("median_input_3");
                    rgb = i_rgb_median.get();
                    wait();
                }
                unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
                buffer[0][v][2] = grey;
                val_median[v*3+2] = grey;
            }
        }

        // sort grey values (only need first 5 values)
        for (unsigned int i = 0; i < 5; ++i) {
            for (unsigned int j = i + 1; j < 9; ++j) {
                if (val_median[i] > val_median[j]) {
                    cynw::swap(val_median[i], val_median[j]);
                }
            }
        }

        // output median
        int median = val_median[4];
        {
			HLS_DEFINE_PROTOCOL("median_output");
            o_result_median.put(median);
			wait();
		}

        // move to next column
        col = (col == 511) ? 0 : (col + 1);
    }
}

const sc_dt::sc_uint<2> mask[MASK_X][MASK_Y] = {{1, 1, 1}, {1, 2, 1}, {1, 1, 1}};

void MedianMeanFilter::do_mean_filter() {
    {
        HLS_DEFINE_PROTOCOL("mean_reset");
        i_rgb_mean.reset();
        o_result_mean.reset();
        wait();
    }

    // reg initialize
    val_mean = 0;

    // try to get the first 2x3 pixels
    for (unsigned int v = 1; v < 3; ++v) {
        for (unsigned int u = 0; u < 3; ++u) {
            sc_dt::sc_uint<24> rgb;
            {
                HLS_DEFINE_PROTOCOL("mean_input_1");
                rgb = i_rgb_mean.get();
                wait();
            }
            unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
            buffer[1][v][u] = grey;
        }
    }

    unsigned int col = 0;    // to record the column of current pixel
    while (true) {
        val_mean = 0; // flush val
        // get all pixels in the filter
        if (col == 0) { // first column
            // get pixels from buffer, and the bottom most pixels from input
            for (unsigned int v = 0; v < 2; ++v) {
                for (unsigned int u = 0; u < 3; ++u) {
                    buffer[1][v][u] = buffer[1][v+1][u];
                    buffer[0][v][u] = buffer[1][v+1][u];  // for the first column of next row
                    val_mean += buffer[1][v+1][u] * mask[v][u];
                }
            }
            for (unsigned int u = 0; u < 3; ++u) {
                sc_dt::sc_uint<24> rgb;
                {
                    HLS_DEFINE_PROTOCOL("mean_input_2");
                    rgb = i_rgb_mean.get();
                    wait();
                }
                unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
                buffer[1][2][u] = grey;
                buffer[0][2][u] = grey;  // for the first column of next row
                val_mean += grey * mask[2][u];
            }
        } else {    // other columns
            // get pixels from buffer, and the right most pixels from input
            for (unsigned int v = 0; v < 3; ++v) {
                for (unsigned int u = 0; u < 2; ++u) {
                    buffer[0][v][u] = buffer[0][v][u+1];
                    val_mean += buffer[0][v][u] * mask[v][u];
                }
                sc_dt::sc_uint<24> rgb;
                {
                    HLS_DEFINE_PROTOCOL("mean_input_2");
                    rgb = i_rgb_mean.get();
                    wait();
                }
                unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
                buffer[0][v][2] = grey;
                val_mean += grey * mask[v][2];
            }
        }

        // output mean
        int mean = val_mean / 10;
        {
			HLS_DEFINE_PROTOCOL("mean_output");
            o_result_mean.put(mean);
			wait();
		}

        // move to next column
        col = (col == 511) ? 0 : (col + 1);
    }
}
