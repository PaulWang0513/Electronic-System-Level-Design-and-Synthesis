#include <cmath>

#include "MedianMeanFilter.h"

MedianMeanFilter::MedianMeanFilter(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_median_filter);
    sensitive << i_clk.pos();
    dont_initialize();
    SC_THREAD(do_mean_filter);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);
}

void MedianMeanFilter::do_median_filter() {
    int col = 0;    // to record the column of current pixel
    while (true) {
        // waiting for enable signal
        while (i_en_median.read() == false) {
            wait();
        }
        
        int idx = 0;
        // get all pixels in the filter
        if (col == 0) { // first column
            // buffer all grey values
            for (unsigned int v = 0; v < MASK_Y; ++v) {
                for (unsigned int u = 0; u < MASK_X; ++u) {
                    unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
                    buffer[0][v][u] = grey;
                    val[idx++] = (int)grey;
                }
            }
        } else {    // other columns
            // get pixels from buffer, and the right most pixels from input
            for (unsigned int v = 0; v < MASK_Y; ++v) {
                for (unsigned int u = 0; u < MASK_X-1; ++u) {
                    buffer[0][v][u] = buffer[0][v][u+1];
                    val[idx++] = (int)buffer[0][v][u];
                }
                unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
                buffer[0][v][MASK_X-1] = grey;
                val[idx++] = (int)grey;
            }
        }

        // sort grey values
        for (unsigned int i = 0; i < MASK_X*MASK_Y-1; ++i) {
            for (unsigned int j = i + 1; j < MASK_X*MASK_Y; ++j) {
                if (val[i] > val[j]) {
                    int temp = val[i];
                    val[i] = val[j];
                    val[j] = temp;
                }
            }
        }
        // output median
        int median;
        if (MASK_X*MASK_Y % 2 == 0) {
            median = (val[MASK_X*MASK_Y / 2] + val[MASK_X*MASK_Y / 2 + 1]) / 2;
        } else {
            median = val[(int)floor(MASK_X*MASK_Y / 2)];
        }
        o_result.write(median);

        // move to next column
        col = (col + 1) % 512;
        wait(10); //emulate module delay
    }
}

const int mask[MASK_X][MASK_Y] = {{1, 1, 1}, {1, 2, 1}, {1, 1, 1}};

void MedianMeanFilter::do_mean_filter() {
    int col = 0;    // to record the column of current pixel
    while (true) {
        // waiting for enable signal
        while (i_en_mean.read() == false) {
            wait();
        }
        val[0] = 0;
        if (col == 0) { // first column
            // buffer all grey values
            for (unsigned int v = 0; v < MASK_Y; ++v) {
                for (unsigned int u = 0; u < MASK_X; ++u) {
                    unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
                    buffer[0][v][u] = grey;
                    val[0] += grey * mask[v][u];
                }
            }
        } else {    // other columns
            // get pixels from buffer, and the right most pixels from input
            for (unsigned int v = 0; v < MASK_Y; ++v) {
                for (unsigned int u = 0; u < MASK_X-1; ++u) {
                    buffer[0][v][u] = buffer[0][v][u+1];
                    val[0] += buffer[0][v][u] * mask[v][u];
                }
                unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
                buffer[0][v][MASK_X-1] = grey;
                val[0] += grey * mask[v][MASK_X-1];
            }
        }

        // output mean
        int mean = val[0] / 10;
        o_result.write(mean);

        // move to next column
        col = (col + 1) % 512;
        wait(10); //emulate module delay
    }
}
