#include <cmath>
#include <iomanip>

#include "MedianMeanFilter.h"

MedianMeanFilter::MedianMeanFilter(sc_module_name n)
    : sc_module(n), t_skt("t_skt"), base_offset(0) {
    SC_THREAD(do_median_filter);
    SC_THREAD(do_mean_filter);

    t_skt.register_b_transport(this, &MedianMeanFilter::blocking_transport);
}

void MedianMeanFilter::do_median_filter() {
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
                        unsigned char grey = (i_r_median.read() + i_g_median.read() + i_b_median.read()) / 3;
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
                    unsigned char grey = (i_r_median.read() + i_g_median.read() + i_b_median.read()) / 3;
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
                unsigned char grey = (i_r_median.read() + i_g_median.read() + i_b_median.read()) / 3;
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
        o_result_median.write(median);

        // move to next column
        col = (col + 1) % 512;
    }
}

const int mask[MASK_X][MASK_Y] = {{1, 1, 1}, {1, 2, 1}, {1, 1, 1}};

void MedianMeanFilter::do_mean_filter() {
    bool first_row = true;
    int col = 0;    // to record the column of current pixel
    while (true) {
        val_mean = 0; // flush val
        // get all pixels in the filter
        if (col == 0) { // first column
            if (first_row) {
                first_row = false;
                // buffer all grey values
                for (unsigned int v = 0; v < MASK_Y; ++v) {
                    for (unsigned int u = 0; u < MASK_X; ++u) {
                        unsigned char grey = (i_r_mean.read() + i_g_mean.read() + i_b_mean.read()) / 3;
                        buffer[0][v][u] = grey;
                        buffer[1][v][u] = grey; // for the first column of next row
                        val_mean += grey * mask[v][u];
                    }
                }
            } else {
                // get pixels from buffer, and the bottom most pixels from input
                for (unsigned int v = 0; v < MASK_Y-1; ++v) {
                    for (unsigned int u = 0; u < MASK_X; ++u) {
                        buffer[1][v][u] = buffer[1][v+1][u];
                        buffer[0][v][u] = buffer[1][v][u];  // for the first column of next row
                        val_mean += buffer[1][v][u] * mask[v][u];
                    }
                }
                for (unsigned int u = 0; u < MASK_X; ++u) {
                    unsigned char grey = (i_r_mean.read() + i_g_mean.read() + i_b_mean.read()) / 3;
                    buffer[1][MASK_Y-1][u] = grey;
                    buffer[0][MASK_Y-1][u] = grey;  // for the first column of next row
                    val_mean += grey * mask[MASK_Y-1][u];
                }
            }
        } else {    // other columns
            // get pixels from buffer, and the right most pixels from input
            for (unsigned int v = 0; v < MASK_Y; ++v) {
                for (unsigned int u = 0; u < MASK_X-1; ++u) {
                    buffer[0][v][u] = buffer[0][v][u+1];
                    val_mean += buffer[0][v][u] * mask[v][u];
                }
                unsigned char grey = (i_r_mean.read() + i_g_mean.read() + i_b_mean.read()) / 3;
                buffer[0][v][MASK_X-1] = grey;
                val_mean += grey * mask[v][MASK_X-1];
            }
        }

        // output mean
        int mean = val_mean / 10;
        o_result_mean.write(mean);

        // move to next column
        col = (col + 1) % 512;
    }
}

void MedianMeanFilter::blocking_transport(tlm::tlm_generic_payload &payload,
                                          sc_core::sc_time &delay) {
    sc_dt::uint64 addr = payload.get_address();
    addr = addr - base_offset;
    unsigned char *mask_ptr = payload.get_byte_enable_ptr();
    unsigned char *data_ptr = payload.get_data_ptr();
    word buffer;
    switch (payload.get_command()) {
    case tlm::TLM_READ_COMMAND:
        switch (addr) {
        case MEDIAN_FILTER_RESULT_ADDR:
            buffer.uint = o_result_median.read();
            // model the delay of the filter
            wait(10 * CLOCK_PERIOD, SC_NS);
            break;
        case MEDIAN_FILTER_CHECK_ADDR:
            buffer.uint = o_result_median.num_available();
            break;
        case MEAN_FILTER_RESULT_ADDR:
            buffer.uint = o_result_mean.read();
            // model the delay of the filter
            wait(10 * CLOCK_PERIOD, SC_NS);
            break;
        case MEAN_FILTER_CHECK_ADDR:
            buffer.uint = o_result_mean.num_available();
            break;
        default:
            std::cerr << "Error! MedianMeanFilter::blocking_transport: address 0x"
                        << std::setfill('0') << std::setw(8) << std::hex << addr
                        << std::dec << " is not valid" << std::endl;
            break;
        }
        data_ptr[0] = buffer.uc[0];
        data_ptr[1] = buffer.uc[1];
        data_ptr[2] = buffer.uc[2];
        data_ptr[3] = buffer.uc[3];
        break;

    case tlm::TLM_WRITE_COMMAND:
        switch (addr) {
        case MEDIAN_FILTER_R_ADDR:
            if (mask_ptr[0] == 0xff) {
                i_r_median.write(data_ptr[0]);
            }
            if (mask_ptr[1] == 0xff) {
                i_g_median.write(data_ptr[1]);
            }
            if (mask_ptr[2] == 0xff) {
                i_b_median.write(data_ptr[2]);
            }
            break;
        case MEAN_FILTER_R_ADDR:
            if (mask_ptr[0] == 0xff) {
                i_r_mean.write(data_ptr[0]);
            }
            if (mask_ptr[1] == 0xff) {
                i_g_mean.write(data_ptr[1]);
            }
            if (mask_ptr[2] == 0xff) {
                i_b_mean.write(data_ptr[2]);
            }
            break;
        default:
            std::cerr << "Error! MedianMeanFilter::blocking_transport: address 0x"
                        << std::setfill('0') << std::setw(8) << std::hex << addr
                        << std::dec << " is not valid" << std::endl;
            break;
        }
        // model the delay of writing data
        wait(1 * CLOCK_PERIOD, SC_NS);
        break;

    case tlm::TLM_IGNORE_COMMAND:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
    default:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
    }
    payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}
