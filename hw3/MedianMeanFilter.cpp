#include <cmath>
#include "stratus_hls.h"
#include "MedianMeanFilter.h"

MedianMeanFilter::MedianMeanFilter(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_median_filter);
    sensitive << i_clk.pos();
    dont_initialize();
    // SC_THREAD(do_mean_filter);
    // sensitive << i_clk.pos();
    // dont_initialize();
    reset_signal_is(i_rst, false);

    i_rgb_median.clk_rst(i_clk, i_rst);
    o_result_median.clk_rst(i_clk, i_rst);
    // i_rgb_mean.clk_rst(i_clk, i_rst);
    // o_result_mean.clk_rst(i_clk, i_rst);
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
    for (unsigned int i=0; i<MASK_X*MASK_Y; i++) {
        val_median[i] = 0;
    }
    for (unsigned int i=0; i<2; i++) {
        for (unsigned int j=0; j<MASK_Y; j++) {
            for (unsigned int k=0; k<MASK_X; k++) {
                buffer[i][j][k] = 0;
            }
        }
    }

    bool first_row = true;
    int col = 0;    // to record the column of current pixel
    
    while (true) {
        int idx = 0;    // index of val
        // get all pixels in the filter
        if (col == 0) { // first column
            if (first_row) {
                first_row = false;
                // buffer all grey values
                for (unsigned int v = 0; v < MASK_Y; ++v) {
                    for (unsigned int u = 0; u < MASK_X; ++u) {
                        sc_dt::sc_uint<24> rgb;
                        {
                            HLS_DEFINE_PROTOCOL("input");
                            rgb = i_rgb_median.get();
                            wait();
                        }
                        unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
                        buffer[0][v][u] = grey;
                        buffer[1][v][u] = grey; // for the first column of next row
                        val_median[idx++] = (int)grey;
                    }
                }
            } else {
                // get pixels from buffer, and the bottom most pixels from input
                for (unsigned int v = 0; v < MASK_Y-1; ++v) {
                    for (unsigned int u = 0; u < MASK_X; ++u) {
                        buffer[1][v][u] = buffer[1][v+1][u];
                        buffer[0][v][u] = buffer[1][v][u];  // for the first column of next row
                        val_median[idx++] = (int)buffer[1][v][u];
                    }
                }
                for (unsigned int u = 0; u < MASK_X; ++u) {
                    sc_dt::sc_uint<24> rgb;
                    {
                        HLS_DEFINE_PROTOCOL("input");
                        rgb = i_rgb_median.get();
                        wait();
                    }
                    unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
                    buffer[1][MASK_Y-1][u] = grey;
                    buffer[0][MASK_Y-1][u] = grey;  // for the first column of next row
                    val_median[idx++] = (int)grey;
                }
            }
        } else {    // other columns
            // get pixels from buffer, and the right most pixels from input
            for (unsigned int v = 0; v < MASK_Y; ++v) {
                for (unsigned int u = 0; u < MASK_X-1; ++u) {
                    buffer[0][v][u] = buffer[0][v][u+1];
                    val_median[idx++] = (int)buffer[0][v][u];
                }
                sc_dt::sc_uint<24> rgb;
                {
                    HLS_DEFINE_PROTOCOL("input");
                    rgb = i_rgb_median.get();
                    wait();
                }
                unsigned char grey = (rgb.range(7,0) + rgb.range(15,8) + rgb.range(23, 16)) / 3;
                buffer[0][v][MASK_X-1] = grey;
                val_median[idx++] = (int)grey;
            }
        }

        // sort grey values
        for (unsigned int i = 0; i < MASK_X*MASK_Y-1; ++i) {
            for (unsigned int j = i + 1; j < MASK_X*MASK_Y; ++j) {
                if (val_median[i] > val_median[j]) {
                    int temp = val_median[i];
                    val_median[i] = val_median[j];
                    val_median[j] = temp;
                }
            }
        }

        // output median
        int median;
        if (MASK_X*MASK_Y % 2 == 0) {
            median = (val_median[MASK_X*MASK_Y / 2] + val_median[MASK_X*MASK_Y / 2 + 1]) / 2;
        } else {
            median = val_median[(int)floor(MASK_X*MASK_Y / 2)];
        }

        {
			HLS_DEFINE_PROTOCOL("output");
            o_result_median.put(median);
			wait();
		}

        // move to next column
        col = (col + 1) % 512;
    }
}

// const int mask[MASK_X][MASK_Y] = {{1, 1, 1}, {1, 2, 1}, {1, 1, 1}};

// void MedianMeanFilter::do_mean_filter() {
//     bool first_row = true;
//     int col = 0;    // to record the column of current pixel
//     while (true) {
//         val_mean = 0; // flush val
//         // get all pixels in the filter
//         if (col == 0) { // first column
//             if (first_row) {
//                 first_row = false;
//                 // buffer all grey values
//                 for (unsigned int v = 0; v < MASK_Y; ++v) {
//                     for (unsigned int u = 0; u < MASK_X; ++u) {
//                         unsigned char grey = (i_r_mean.read() + i_g_mean.read() + i_b_mean.read()) / 3;
//                         buffer[0][v][u] = grey;
//                         buffer[1][v][u] = grey; // for the first column of next row
//                         val_mean += grey * mask[v][u];
//                     }
//                 }
//             } else {
//                 // get pixels from buffer, and the bottom most pixels from input
//                 for (unsigned int v = 0; v < MASK_Y-1; ++v) {
//                     for (unsigned int u = 0; u < MASK_X; ++u) {
//                         buffer[1][v][u] = buffer[1][v+1][u];
//                         buffer[0][v][u] = buffer[1][v][u];  // for the first column of next row
//                         val_mean += buffer[1][v][u] * mask[v][u];
//                     }
//                 }
//                 for (unsigned int u = 0; u < MASK_X; ++u) {
//                     unsigned char grey = (i_r_mean.read() + i_g_mean.read() + i_b_mean.read()) / 3;
//                     buffer[1][MASK_Y-1][u] = grey;
//                     buffer[0][MASK_Y-1][u] = grey;  // for the first column of next row
//                     val_mean += grey * mask[MASK_Y-1][u];
//                 }
//             }
//         } else {    // other columns
//             // get pixels from buffer, and the right most pixels from input
//             for (unsigned int v = 0; v < MASK_Y; ++v) {
//                 for (unsigned int u = 0; u < MASK_X-1; ++u) {
//                     buffer[0][v][u] = buffer[0][v][u+1];
//                     val_mean += buffer[0][v][u] * mask[v][u];
//                 }
//                 unsigned char grey = (i_r_mean.read() + i_g_mean.read() + i_b_mean.read()) / 3;
//                 buffer[0][v][MASK_X-1] = grey;
//                 val_mean += grey * mask[v][MASK_X-1];
//             }
//         }

//         // output mean
//         int mean = val_mean / 10;
//         o_result_mean.write(mean);

//         // move to next column
//         col = (col + 1) % 512;
//         wait(10); //emulate module delay
//     }
// }
