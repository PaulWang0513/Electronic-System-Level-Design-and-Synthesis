#ifndef MEDIANMEAN_FILTER_H_
#define MEDIANMEAN_FILTER_H_
#include <systemc>
using namespace sc_core;

#include "filter_def.h"

class MedianMeanFilter : public sc_module {
public:
    sc_in_clk i_clk;
    sc_in<bool> i_rst;
    sc_in<bool> i_en_median;
    sc_in<bool> i_en_mean;
    sc_fifo_in<unsigned char> i_r;
    sc_fifo_in<unsigned char> i_g;
    sc_fifo_in<unsigned char> i_b;
    sc_fifo_out<int> o_result;

    SC_HAS_PROCESS(MedianMeanFilter);
    MedianMeanFilter(sc_module_name n);
    ~MedianMeanFilter() = default;

private:
    void do_median_filter();
    void do_mean_filter();
    int val[MASK_X * MASK_Y];   // make the size sufficient to store pixels for median filter
};
#endif
