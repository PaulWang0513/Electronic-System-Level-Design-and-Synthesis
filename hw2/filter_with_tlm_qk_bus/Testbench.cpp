#include <cassert>
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
    : sc_module(n), initiator("initiator"), output_rgb_raw_data_offset(54) {
    SC_THREAD(do_median_mean_filter);

    // set quantum
    m_qk.set_global_quantum(sc_time(30 * CLOCK_PERIOD, SC_NS));
    m_qk.reset();
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
    assert(fread(&input_rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s));

    // move offset to 18 to get width & height;
    fseek(fp_s, 18, SEEK_SET);
    assert(fread(&width, sizeof(unsigned int), 1, fp_s));
    assert(fread(&height, sizeof(unsigned int), 1, fp_s));

    // get bit per pixel
    fseek(fp_s, 28, SEEK_SET);
    assert(fread(&bits_per_pixel, sizeof(unsigned short), 1, fp_s));
    bytes_per_pixel = bits_per_pixel / 8;

    // move offset to input_rgb_raw_data_offset to get RGB raw data
    fseek(fp_s, input_rgb_raw_data_offset, SEEK_SET);

    source_bitmap = (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
    if (source_bitmap == NULL) {
        printf("malloc images_s error\n");
        return -1;
    }

    target_bitmap = (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
    if (target_bitmap == NULL) {
        printf("malloc target_bitmap error\n");
        return -1;
    }

    printf("Image width=%d, height=%d\n", width, height);
    assert(fread(source_bitmap, sizeof(unsigned char), (size_t)(long)width * height * bytes_per_pixel, fp_s));
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

void Testbench::do_median_mean_filter() {
    int x, y, v, u;        // for loop counter
    unsigned char R, G, B; // color of R, G, B
    int adjustX, adjustY, xBound, yBound;
    int total;

    word data;
    unsigned char mask[4];
    bool done = false;
    int output_num = 0;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    adjustX = (MASK_X % 2) ? 1 : 0; // 1
    adjustY = (MASK_Y % 2) ? 1 : 0; // 1
    xBound = MASK_X / 2;            // 1
    yBound = MASK_Y / 2;            // 1
    
    // Median filter start here
    printf("Median filter start ...\n");

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
                    data.uc[0] = R;
                    data.uc[1] = G;
                    data.uc[2] = B;
                    mask[0] = 0xff;
                    mask[1] = 0xff;
                    mask[2] = 0xff;
                    mask[3] = 0;
                    delay = m_qk.get_local_time();
                    initiator.write_to_socket(MEDIAN_MEAN_MM_BASE + MEDIAN_FILTER_R_ADDR, mask, data.uc, 4, delay);
                    m_qk.inc(delay);
                    if (m_qk.need_sync()) m_qk.sync();
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
                data.uc[0] = R;
                data.uc[1] = G;
                data.uc[2] = B;
                mask[0] = 0xff;
                mask[1] = 0xff;
                mask[2] = 0xff;
                mask[3] = 0;
                delay = m_qk.get_local_time();
                initiator.write_to_socket(MEDIAN_MEAN_MM_BASE + MEDIAN_FILTER_R_ADDR, mask, data.uc, 4, delay);
                m_qk.inc(delay);
                if (m_qk.need_sync()) m_qk.sync();
            }
        }
        // get result from filter
        done = false;
        output_num = 0;
        while(!done){
            delay = m_qk.get_local_time();
            initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEDIAN_FILTER_CHECK_ADDR, mask, data.uc, 4, delay);
            m_qk.inc(delay);
            if (m_qk.need_sync()) m_qk.sync();
            output_num = data.sint;
            if(output_num>0) done=true;
        }
        delay = m_qk.get_local_time();
        initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEDIAN_FILTER_RESULT_ADDR, mask, data.uc, 4, delay);
        m_qk.inc(delay);
        if (m_qk.need_sync()) m_qk.sync();
        total = data.sint;
        //debug
        //cout << "Now at " << sc_time_stamp() << endl; //print current sc_time

        *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
        *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
        *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        
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
                data.uc[0] = R;
                data.uc[1] = G;
                data.uc[2] = B;
                mask[0] = 0xff;
                mask[1] = 0xff;
                mask[2] = 0xff;
                mask[3] = 0;
                delay = m_qk.get_local_time();
                initiator.write_to_socket(MEDIAN_MEAN_MM_BASE + MEDIAN_FILTER_R_ADDR, mask, data.uc, 4, delay);
                m_qk.inc(delay);
                if (m_qk.need_sync()) m_qk.sync();
            }

            // get result from filter
            done = false;
            output_num = 0;
            while(!done){
                delay = m_qk.get_local_time();
                initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEDIAN_FILTER_CHECK_ADDR, mask, data.uc, 4, delay);
                m_qk.inc(delay);
                if (m_qk.need_sync()) m_qk.sync();
                output_num = data.sint;
                if(output_num>0) done=true;
            }
            delay = m_qk.get_local_time();
            initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEDIAN_FILTER_RESULT_ADDR, mask, data.uc, 4, delay);
            m_qk.inc(delay);
            if (m_qk.need_sync()) m_qk.sync();
            total = data.sint;
            //debug
            //cout << "Now at " << sc_time_stamp() << endl; //print current sc_time

            *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
            *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
            *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        }
    }

    // Median filter end here
    printf("Median filter done\n");


    // copy from target_bitmap to source_bitmap
    for (y = 0; y != height; ++y) {
        for (x = 0; x != width; ++x) {
            *(source_bitmap + bytes_per_pixel * (width * y + x) + 2) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 2);
            *(source_bitmap + bytes_per_pixel * (width * y + x) + 1) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 1);
            *(source_bitmap + bytes_per_pixel * (width * y + x) + 0) = *(target_bitmap + bytes_per_pixel * (width * y + x) + 0);
        }
    }


    // Mean filter start here
    printf("Mean filter start ...\n");

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
                    data.uc[0] = R;
                    data.uc[1] = G;
                    data.uc[2] = B;
                    mask[0] = 0xff;
                    mask[1] = 0xff;
                    mask[2] = 0xff;
                    mask[3] = 0;
                    delay = m_qk.get_local_time();
                    initiator.write_to_socket(MEDIAN_MEAN_MM_BASE + MEAN_FILTER_R_ADDR, mask, data.uc, 4, delay);
                    m_qk.inc(delay);
                    if (m_qk.need_sync()) m_qk.sync();
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
                data.uc[0] = R;
                data.uc[1] = G;
                data.uc[2] = B;
                mask[0] = 0xff;
                mask[1] = 0xff;
                mask[2] = 0xff;
                mask[3] = 0;
                delay = m_qk.get_local_time();
                initiator.write_to_socket(MEDIAN_MEAN_MM_BASE + MEAN_FILTER_R_ADDR, mask, data.uc, 4, delay);
                m_qk.inc(delay);
                if (m_qk.need_sync()) m_qk.sync();
            }
        }
        // get result from filter
        done = false;
        output_num = 0;
        while(!done){
            delay = m_qk.get_local_time();
            initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEAN_FILTER_CHECK_ADDR, mask, data.uc, 4, delay);
            m_qk.inc(delay);
            if (m_qk.need_sync()) m_qk.sync();
            output_num = data.sint;
            if(output_num>0) done=true;
        }
        delay = m_qk.get_local_time();
        initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEAN_FILTER_RESULT_ADDR, mask, data.uc, 4, delay);
        m_qk.inc(delay);
        if (m_qk.need_sync()) m_qk.sync();
        total = data.sint;
        //debug
        //cout << "Now at " << sc_time_stamp() << endl; //print current sc_time

        *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
        *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
        *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        
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
                data.uc[0] = R;
                data.uc[1] = G;
                data.uc[2] = B;
                mask[0] = 0xff;
                mask[1] = 0xff;
                mask[2] = 0xff;
                mask[3] = 0;
                delay = m_qk.get_local_time();
                initiator.write_to_socket(MEDIAN_MEAN_MM_BASE + MEAN_FILTER_R_ADDR, mask, data.uc, 4, delay);
                m_qk.inc(delay);
                if (m_qk.need_sync()) m_qk.sync();
            }

            // get result from filter
            done = false;
            output_num = 0;
            while(!done){
                delay = m_qk.get_local_time();
                initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEAN_FILTER_CHECK_ADDR, mask, data.uc, 4, delay);
                m_qk.inc(delay);
                if (m_qk.need_sync()) m_qk.sync();
                output_num = data.sint;
                if(output_num>0) done=true;
            }
            delay = m_qk.get_local_time();
            initiator.read_from_socket(MEDIAN_MEAN_MM_BASE + MEAN_FILTER_RESULT_ADDR, mask, data.uc, 4, delay);
            m_qk.inc(delay);
            if (m_qk.need_sync()) m_qk.sync();
            total = data.sint;
            //debug
            //cout << "Now at " << sc_time_stamp() << endl; //print current sc_time

            *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = total;
            *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = total;
            *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = total;
        }
    }

    // Mean filter end here
    printf("Mean filter done\n");

    sc_stop();
}
