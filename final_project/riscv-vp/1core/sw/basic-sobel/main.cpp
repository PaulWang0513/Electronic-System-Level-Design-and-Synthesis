#include "string"
#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <iostream>
#include <fstream>
using namespace std;

const int SIGNAL_LEN = 1000;

union word {
    int sint;
    unsigned int uint;
    unsigned char uc[4];
};

// ACF ACC
static char* const ACF_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const ACF_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = true;
unsigned char *input_data;
unsigned int *golden_data;
unsigned int *result_data;

void read_input_and_golden(std::string input_file_name, std::string golden_file_name)
{
    input_data = new unsigned char[SIGNAL_LEN];
    golden_data = new unsigned int[SIGNAL_LEN];
    result_data = new unsigned int[SIGNAL_LEN];

    ifstream input_file(input_file_name, ios::in);
    if (!input_file.is_open()) {
        cout << "Error opening input file: " << input_file_name << endl;
        exit(-1);
    }
    cout << "Reading input data from " << input_file_name << "..." << endl;
    for (int i = 0; i < SIGNAL_LEN; i++) {
        string temp;
        input_file >> temp;
        input_data[i] = (unsigned char)stoi(temp);
    }
    input_file.close();

    ifstream golden_file(golden_file_name, ios::in);
    if (!golden_file.is_open()) {
        cout << "Error opening golden file: " << golden_file_name << endl;
        exit(-1);
    }
    cout << "Reading golden data from " << golden_file_name << "..." << endl;
    for (int i = 0; i < SIGNAL_LEN; i++) {
        string temp;
        golden_file >> temp;
        golden_data[i] = (unsigned int)stoi(temp);
    }
    golden_file.close();
}

void validate()
{
    cout << "Validating..." << endl;
    int error = 0;
    for (int i = 0; i < SIGNAL_LEN; i++) {
        if (result_data[i] != golden_data[i]) {
            error++;
            cout << "[ERROR] " << "idx: " << i << "\t result = " << result_data[i] << "\t golden = " << golden_data[i] << endl;
        }
    }
    if (error == 0)
        cout << "Validation [PASS]" << endl;
    else
        cout << "Validation [FAIL] " << error << " errors found!" << endl;
}

void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len){
    cout << "write..." << endl;
    if(_is_using_dma){  
        // Using DMA 
        *DMA_SRC_ADDR = (uint32_t)(buffer);
        *DMA_DST_ADDR = (uint32_t)(ADDR);
        *DMA_LEN_ADDR = len;
        *DMA_OP_ADDR  = DMA_OP_MEMCPY;
    }else{
        // Directly Send
        memcpy(ADDR, buffer, sizeof(unsigned char)*len);
    }
}

void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
    cout << "reading..." << endl;
    if(_is_using_dma){
        // Using DMA 
        *DMA_SRC_ADDR = (uint32_t)(ADDR);
        *DMA_DST_ADDR = (uint32_t)(buffer);
        *DMA_LEN_ADDR = len;
        *DMA_OP_ADDR  = DMA_OP_MEMCPY;
    }else{
        // Directly Read
        memcpy(buffer, ADDR, sizeof(unsigned char)*len);
    }
}

int main(int argc, char *argv[]) {
    if (_is_using_dma)
        cout << "_is_using_dma = true" << endl;
    else
        cout << "_is_using_dma = false" << endl;
    read_input_and_golden("./data/sin_wave.dat", "./data/sin_wave_golden.dat");

    unsigned char  buffer[4] = {0};
    word data;
    int total;

    for (int i=0; i<SIGNAL_LEN; i++) {
        buffer[0] = input_data[i];
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        write_data_to_ACC(ACF_START_ADDR, buffer, 4);
    }
    for (int i=0; i<SIGNAL_LEN; i++) {
        read_data_from_ACC(ACF_READ_ADDR, buffer, 4);
        memcpy(data.uc, buffer, 4);
        result_data[i] = (data).sint;
    }

    validate();
}
