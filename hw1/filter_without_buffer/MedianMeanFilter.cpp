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
    while (true) {
        // waiting for enable signal
        while (i_en_median.read() == false) {
            wait();
        }
        int idx = 0;
        // buffer all grey values
        for (unsigned int v = 0; v < MASK_Y; ++v) {
            for (unsigned int u = 0; u < MASK_X; ++u) {
                unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
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
        wait(10); //emulate module delay
    }
}

const int mask[MASK_X][MASK_Y] = {{1, 1, 1}, {1, 2, 1}, {1, 1, 1}};

void MedianMeanFilter::do_mean_filter() {
    while (true) {
        // waiting for enable signal
        while (i_en_mean.read() == false) {
            wait();
        }
        // clear val
        val[0] = 0;
        // accumulate
        for (unsigned int v = 0; v < MASK_Y; ++v) {
            for (unsigned int u = 0; u < MASK_X; ++u) {
                unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
                val[0] += grey * mask[v][u];
            }
        }
        // output mean
        int mean = val[0] / 10;
        o_result.write(mean);
        wait(10); //emulate module delay
    }
}
