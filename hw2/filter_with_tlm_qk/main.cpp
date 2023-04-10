#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "MedianMeanFilter.h"
#include "Testbench.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv) {
    if ((argc < 3) || (argc > 4)) {
        cout << "No arguments for the executable : " << argv[0] << endl;
        cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name" << endl;
        return 0;
    }
    
    Testbench tb("tb");
    MedianMeanFilter median_mean_filter("median_mean_filter");
    tb.initiator.i_skt(median_mean_filter.t_skt);

    std::cout << "input file name : " << argv[1] << endl;
    tb.read_bmp(argv[1]);

    gettimeofday(&start_time, NULL);
    sc_start();
    gettimeofday(&end_time, NULL);

    std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
    tb.write_bmp(argv[2]);

    std::cout << "==========" << std::endl;
    std::cout << "Simultation time == " << (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec) << " us" << std::endl;
    
    return 0;
}
