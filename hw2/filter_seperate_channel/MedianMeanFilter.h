#ifndef MEDIANMEAN_FILTER_H_
#define MEDIANMEAN_FILTER_H_
#include <systemc>
using namespace sc_core;

#include "filter_def.h"

class MedianMeanFilter : public sc_module {
public:
    sc_in_clk i_clk;
    sc_in<bool> i_rst;
    sc_fifo_in<unsigned char> i_r_median;
    sc_fifo_in<unsigned char> i_g_median;
    sc_fifo_in<unsigned char> i_b_median;
    sc_fifo_out<int> o_result_median;
    sc_fifo_in<unsigned char> i_r_mean;
    sc_fifo_in<unsigned char> i_g_mean;
    sc_fifo_in<unsigned char> i_b_mean;
    sc_fifo_out<int> o_result_mean;

    SC_HAS_PROCESS(MedianMeanFilter);
    MedianMeanFilter(sc_module_name n);
    ~MedianMeanFilter() = default;

private:
    void do_median_filter();
    void do_mean_filter();
    int val_median[MASK_X * MASK_Y];    // to store pixels for median filter to find the median value
    int val_mean;                       // to sum up pixels for mean filter to find the mean value
    unsigned char buffer[2][MASK_Y][MASK_X];   // use the first 2D array to buffer the last 511 pixels of a row => xoo
                                               //                                                                  xoo
                                               //                                                                  xoo
                                               // use the last 2D array to buffer the first pixel of the next row => xxx
                                               //                                                                    ooo
                                               //                                                                    ooo
};
#endif
