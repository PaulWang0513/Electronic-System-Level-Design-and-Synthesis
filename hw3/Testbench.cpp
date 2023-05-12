// #include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"

unsigned char header[54] = {
    0x42,          // identity : B
    0x4d,          // identity : M
    0,    0, 0, 0, // file size
    0,    0,       // reserved1
    0,    0,       // reserved2
    54,   0, 0, 0, // RGB data offset
    40,   0, 0, 0, // struct BITMAPINFOHEADER size
    0,    0, 0, 0, // bmp width
    0,    0, 0, 0, // bmp height
    1,    0,       // planes
    24,   0,       // bit per pixel
    0,    0, 0, 0, // compression
    0,    0, 0, 0, // data size
    0,    0, 0, 0, // h resolution
    0,    0, 0, 0, // v resolution
    0,    0, 0, 0, // used colors
    0,    0, 0, 0  // important colors
};

Testbench::Testbench(sc_module_name n)
    : sc_module(n), output_rgb_raw_data_offset(54) {
    SC_THREAD(feed_rgb);
    sensitive << i_clk.pos();
    dont_initialize();
    SC_THREAD(fetch_result);
    sensitive << i_clk.pos();
    dont_initialize();
}

Testbench::~Testbench() {
	//cout<< "Max txn time = " << max_txn_time << endl;
	//cout<< "Min txn time = " << min_txn_time << endl;
	//cout<< "Avg txn time = " << total_txn_time/n_txn << endl;
	cout << "Total run time = " << total_run_time << endl;
}

int Testbench::read_bmp(string infile_name) {
    FILE *fp_s = NULL; // source file handler
    fp_s = fopen(infile_name.c_str(), "rb");
    if (fp_s == NULL) {
        printf("fopen %s error\n", infile_name.c_str());
        return -1;
    }
    // move offset to 10 to find rgb raw data offset
    fseek(fp_s, 10, SEEK_SET);
    fread(&input_rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s);

    // move offset to 18 to get width & height;
    fseek(fp_s, 18, SEEK_SET);
    fread(&width, sizeof(unsigned int), 1, fp_s);
    fread(&height, sizeof(unsigned int), 1, fp_s);

    // get bit per pixel
    fseek(fp_s, 28, SEEK_SET);
    fread(&bits_per_pixel, sizeof(unsigned short), 1, fp_s);
    bytes_per_pixel = bits_per_pixel / 8;

    // move offset to input_rgb_raw_data_offset to get RGB raw data
    fseek(fp_s, input_rgb_raw_data_offset, SEEK_SET);

    source_bitmap =
        (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
    if (source_bitmap == NULL) {
        printf("malloc images_s error\n");
        return -1;
    }

    target_bitmap =
        (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
    if (target_bitmap == NULL) {
        printf("malloc target_bitmap error\n");
        return -1;
    }

    fread(source_bitmap, sizeof(unsigned char),
            (size_t)(long)width * height * bytes_per_pixel, fp_s);
    fclose(fp_s);
    return 0;
}

int Testbench::write_bmp(string outfile_name) {
    FILE *fp_t = NULL;      // target file handler
    unsigned int file_size; // file size

    fp_t = fopen(outfile_name.c_str(), "wb");
    if (fp_t == NULL) {
        printf("fopen %s error\n", outfile_name.c_str());
        return -1;
    }

    // file size
    file_size = width * height * bytes_per_pixel + output_rgb_raw_data_offset;
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    // width
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    // height
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    // bit per pixel
    header[28] = bits_per_pixel;

    // write header
    fwrite(header, sizeof(unsigned char), output_rgb_raw_data_offset, fp_t);

    // write image
    fwrite(target_bitmap, sizeof(unsigned char),
            (size_t)(long)width * height * bytes_per_pixel, fp_t);

    fclose(fp_t);
    return 0;
}

void Testbench::feed_rgb() {
    int cnt = 0;    // count the number of pixel transfer
    int x, y, v, u;        // for loop counter
    unsigned char R, G, B; // color of R, G, B
    int adjustX, adjustY, xBound, yBound;
    int total;
    n_txn = 0;
	max_txn_time = SC_ZERO_TIME;
	min_txn_time = SC_ZERO_TIME;
	total_txn_time = SC_ZERO_TIME;

    adjustX = (MASK_X % 2) ? 1 : 0; // 1
    adjustY = (MASK_Y % 2) ? 1 : 0; // 1
    xBound = MASK_X / 2;            // 1
    yBound = MASK_Y / 2;            // 1
    
    o_rgb_median.reset();
    // o_rgb_mean.reset();

    o_rst.write(false);
    cout << "reset..." << endl;
    wait(5);
    o_rst.write(true);
    wait(1);

    total_start_time = sc_time_stamp();

    cout << "feeding data to median filter..." << endl;

    // for each pixel
    for (y = 0; y != height; ++y) {
        // first column
        x = 0;

        if (y == 0) {
            // send all pixels in the mask to the filter
            for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
                for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
                    if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
                        R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
                        G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
                        B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
                    } else {
                        R = 0;
                        G = 0;
                        B = 0;
                    }
                    sc_dt::sc_uint<24> rgb;
                    rgb.range(7, 0) = R;
					rgb.range(15, 8) = G;
					rgb.range(23, 16) = B;
                    o_rgb_median.put(rgb);
                    cnt++;
                }
            }
        } else {
            // send the bottom most pixels in mask to the filter
            v = 1;
            for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
                if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
                    R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
                    G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
                    B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
                } else {
                    R = 0;
                    G = 0;
                    B = 0;
                }
                sc_dt::sc_uint<24> rgb;
                rgb.range(7, 0) = R;
                rgb.range(15, 8) = G;
                rgb.range(23, 16) = B;
                o_rgb_median.put(rgb);
                cnt++;
            }
        }
        
        // for the remaining columns
        for (x = 1; x != width; ++x) {
            // send right most pixels in mask to the filter
            for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
                u = 1;
                if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
                    R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
                    G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
                    B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
                } else {
                    R = 0;
                    G = 0;
                    B = 0;
                }
                sc_dt::sc_uint<24> rgb;
                rgb.range(7, 0) = R;
                rgb.range(15, 8) = G;
                rgb.range(23, 16) = B;
                o_rgb_median.put(rgb);
                cnt++;
            }
        }
    }

    {
    // // Median filter end here
    // printf("Median filter done\n");

    // // copy from target_bitmap to source_bitmap
    // for (y = 0; y != height; ++y) {
    //     for (x = 0; x != width; ++x) {
    //         *(source_bitmap + bytes_per_pixel * (width * y + x) + 2) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 2);
    //         *(source_bitmap + bytes_per_pixel * (width * y + x) + 1) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 1);
    //         *(source_bitmap + bytes_per_pixel * (width * y + x) + 0) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 0);
    //     }
    // }

    // // Mean filter start here
    // printf("Mean filter start ...\n");
  
    // // for each pixel
    // for (y = 0; y != height; ++y) {
    //     // first column
    //     x = 0;

    //     if (y == 0) {
    //         // send all pixels in the mask to the filter
    //         for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
    //             for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
    //                 if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
    //                     R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
    //                     G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
    //                     B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
    //                 } else {
    //                     R = 0;
    //                     G = 0;
    //                     B = 0;
    //                 }
    //                 o_r_mean.write(R);
    //                 o_g_mean.write(G);
    //                 o_b_mean.write(B);
    //                 cnt++;
    //                 wait(1); //emulate channel delay
    //             }
    //         }
    //     } else {
    //         // send the bottom most pixels in mask to the filter
    //         v = 1;
    //         for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
    //             if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
    //                 R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
    //                 G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
    //                 B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
    //             } else {
    //                 R = 0;
    //                 G = 0;
    //                 B = 0;
    //             }
    //             o_r_mean.write(R);
    //             o_g_mean.write(G);
    //             o_b_mean.write(B);
    //             cnt++;
    //             wait(1); //emulate channel delay
    //         }
    //     }
    //     // get result from filter
    //     if(i_result_mean.num_available()==0) wait(i_result_mean.data_written_event());
    //     total = i_result_mean.read();

    //     *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
    //     *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
    //     *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        
    //     // for the remaining columns
    //     for (x = 1; x != width; ++x) {
    //         // send right most pixels in mask to the filter
    //         for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
    //             u = 1;
    //             if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
    //                 R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
    //                 G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
    //                 B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
    //             } else {
    //                 R = 0;
    //                 G = 0;
    //                 B = 0;
    //             }
    //             o_r_mean.write(R);
    //             o_g_mean.write(G);
    //             o_b_mean.write(B);
    //             cnt++;
    //             wait(1); //emulate channel delay
    //         }

    //         // get result from filter
    //         if(i_result_mean.num_available()==0) wait(i_result_mean.data_written_event());
    //         total = i_result_mean.read();

    //         *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
    //         *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
    //         *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
    //     }
    // }

    // // Mean filter end here
    // printf("Mean filter done\n");

    // printf("Total pixel transfer: %d\n", cnt);
    // sc_stop();
    }
}

void Testbench::fetch_result() {
    unsigned int x, y; // for loop counter
    int total;
    i_result_median.reset();
    // i_result_mean.reset();

    wait(5);
    wait(1);

    for (y = 0; y != height; ++y) {
        for (x = 0; x != width; ++x) {
            total = i_result_median.get();
            *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
            *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
            *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        }
        cout << "[Median] Row " << y << " done" << endl;
    }

    cout << "Median filter done" << endl;

    total_run_time = sc_time_stamp() - total_start_time;
    sc_stop();
}

// void Testbench::do_median_mean_filter() {
//     int cnt = 0;    // count the number of pixel transfer
//     int x, y, v, u;        // for loop counter
//     unsigned char R, G, B; // color of R, G, B
//     int adjustX, adjustY, xBound, yBound;
//     int total;

//     adjustX = (MASK_X % 2) ? 1 : 0; // 1
//     adjustY = (MASK_Y % 2) ? 1 : 0; // 1
//     xBound = MASK_X / 2;            // 1
//     yBound = MASK_Y / 2;            // 1
    
//     o_rst.write(false);
//     cout << "after reset" << endl;
//     wait(1);
//     o_rst.write(true);
//     wait(1);

//     // Median filter start here
//     printf("Median filter start ...\n");

//     // for each pixel
//     for (y = 0; y != height; ++y) {
//         // first column
//         x = 0;

//         if (y == 0) {
//             // send all pixels in the mask to the filter
//             for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
//                 for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
//                     if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
//                         R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
//                         G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
//                         B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
//                     } else {
//                         R = 0;
//                         G = 0;
//                         B = 0;
//                     }
//                     o_r_median.write(R);
//                     o_g_median.write(G);
//                     o_b_median.write(B);
//                     cnt++;
//                     wait(1); //emulate channel delay
//                 }
//             }
//         } else {
//             // send the bottom most pixels in mask to the filter
//             v = 1;
//             for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
//                 if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
//                     R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
//                     G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
//                     B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
//                 } else {
//                     R = 0;
//                     G = 0;
//                     B = 0;
//                 }
//                 o_r_median.write(R);
//                 o_g_median.write(G);
//                 o_b_median.write(B);
//                 cnt++;
//                 wait(1); //emulate channel delay
//             }
//         }
//         // get result from filter
//         if(i_result_median.num_available()==0) wait(i_result_median.data_written_event());
//         total = i_result_median.read();

//         *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
//         *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
//         *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        
//         // for the remaining columns
//         for (x = 1; x != width; ++x) {
//             // send right most pixels in mask to the filter
//             for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
//                 u = 1;
//                 if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
//                     R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
//                     G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
//                     B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
//                 } else {
//                     R = 0;
//                     G = 0;
//                     B = 0;
//                 }
//                 o_r_median.write(R);
//                 o_g_median.write(G);
//                 o_b_median.write(B);
//                 cnt++;
//                 wait(1); //emulate channel delay
//             }

//             // get result from filter
//             if(i_result_median.num_available()==0) wait(i_result_median.data_written_event());
//             total = i_result_median.read();

//             *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
//             *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
//             *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
//         }
//     }

//     // Median filter end here
//     printf("Median filter done\n");

//     // copy from target_bitmap to source_bitmap
//     for (y = 0; y != height; ++y) {
//         for (x = 0; x != width; ++x) {
//             *(source_bitmap + bytes_per_pixel * (width * y + x) + 2) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 2);
//             *(source_bitmap + bytes_per_pixel * (width * y + x) + 1) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 1);
//             *(source_bitmap + bytes_per_pixel * (width * y + x) + 0) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 0);
//         }
//     }

//     // Mean filter start here
//     printf("Mean filter start ...\n");
  
//     // for each pixel
//     for (y = 0; y != height; ++y) {
//         // first column
//         x = 0;

//         if (y == 0) {
//             // send all pixels in the mask to the filter
//             for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
//                 for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
//                     if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
//                         R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
//                         G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
//                         B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
//                     } else {
//                         R = 0;
//                         G = 0;
//                         B = 0;
//                     }
//                     o_r_mean.write(R);
//                     o_g_mean.write(G);
//                     o_b_mean.write(B);
//                     cnt++;
//                     wait(1); //emulate channel delay
//                 }
//             }
//         } else {
//             // send the bottom most pixels in mask to the filter
//             v = 1;
//             for (u = -xBound; u != xBound + adjustX; ++u) { //-1, 0, 1
//                 if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
//                     R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
//                     G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
//                     B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
//                 } else {
//                     R = 0;
//                     G = 0;
//                     B = 0;
//                 }
//                 o_r_mean.write(R);
//                 o_g_mean.write(G);
//                 o_b_mean.write(B);
//                 cnt++;
//                 wait(1); //emulate channel delay
//             }
//         }
//         // get result from filter
//         if(i_result_mean.num_available()==0) wait(i_result_mean.data_written_event());
//         total = i_result_mean.read();

//         *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
//         *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
//         *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        
//         // for the remaining columns
//         for (x = 1; x != width; ++x) {
//             // send right most pixels in mask to the filter
//             for (v = -yBound; v != yBound + adjustY; ++v) {   //-1, 0, 1
//                 u = 1;
//                 if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
//                     R = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 2);
//                     G = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 1);
//                     B = *(source_bitmap + bytes_per_pixel * (width * (y + v) + (x + u)) + 0);
//                 } else {
//                     R = 0;
//                     G = 0;
//                     B = 0;
//                 }
//                 o_r_mean.write(R);
//                 o_g_mean.write(G);
//                 o_b_mean.write(B);
//                 cnt++;
//                 wait(1); //emulate channel delay
//             }

//             // get result from filter
//             if(i_result_mean.num_available()==0) wait(i_result_mean.data_written_event());
//             total = i_result_mean.read();

//             *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
//             *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
//             *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
//         }
//     }

//     // Mean filter end here
//     printf("Mean filter done\n");

//     printf("Total pixel transfer: %d\n", cnt);
//     sc_stop();
// }
