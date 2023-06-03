#ifndef DEF_H_
#define DEF_H_

#define CLOCK_PERIOD 10


#define MAX_IMAGE_BUFFER_LENTH 1024

const int SIGNAL_LEN = 1000;
// ACF function inner transport addresses
// Used between blocking_transport() & do_filter()
const int ACF_R_ADDR = 0x00000000;
const int ACF_RESULT_ADDR = 0x00000004;

union word {
    int sint;
    unsigned int uint;
    unsigned char uc[4];
};

#endif
