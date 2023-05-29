#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

#define CLOCK_PERIOD 10


#define MAX_IMAGE_BUFFER_LENTH 1024

const int MASK_N = 2;
const int MASK_X = 3;
const int MASK_Y = 3;
const int DMA_TRANS = 64;

// MedianMean Filter inner transport addresses
// Used between blocking_transport() & do_filter()
const int MEDIAN_FILTER_R_ADDR = 0x00000000;
const int MEDIAN_FILTER_RESULT_ADDR = 0x00000004;
const int MEAN_FILTER_R_ADDR = 0x00000008;
const int MEAN_FILTER_RESULT_ADDR = 0x0000000C;

union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};

#endif
