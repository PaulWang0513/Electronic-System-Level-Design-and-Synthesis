// #include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#include "Testbench.h"

#include <queue>
static std::queue<sc_time> time_queue;

Testbench::Testbench(sc_module_name n)
    : sc_module(n) {
    SC_THREAD(feed_data);
    sensitive << i_clk.pos();
    dont_initialize();
    SC_THREAD(fetch_result);
    sensitive << i_clk.pos();
    dont_initialize();
}

Testbench::~Testbench() {
    while (!time_queue.empty()) {
        time_queue.pop();
    }
    cout << "===============================================" << endl;
    cout << "=============== Simulation Done ===============" << endl;
    cout << "===============================================" << endl;
    validate();
	cout << "Total run time = " << total_run_time << endl;
}

void Testbench::feed_data() {

    o_data.reset();

    // reset
    o_rst.write(false);
    wait(5);
    o_rst.write(true);
    wait(1);

    total_start_time = sc_time_stamp();

    cout << "feeding data..." << endl;

    for (int i=0; i<SIGNAL_LEN; i++) {
        o_data.put(input_data[i]);
    }

    cout << "feeding data done" << endl;
    time_queue.push(sc_time_stamp());
}

void Testbench::fetch_result() {
    i_result.reset();

    wait(5);
    wait(1);

    unsigned int latency_sum = 0;

    for (int i=0; i<SIGNAL_LEN; i++) {
        result_data[i] = i_result.get();

        sc_time sent_time( time_queue.front() );
        time_queue.pop();
        unsigned long latency = clock_cycle( sc_time_stamp() - sent_time );
        latency_sum += latency;
        cout << "result[" << i << "] = " << result_data[i] << "\t latency = " << latency << endl;
        time_queue.push(sc_time_stamp());
    }

    total_run_time = sc_time_stamp() - total_start_time;
    sc_stop();
}

void Testbench::read_input_and_golden(string input_file_name, string golden_file_name)
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

void Testbench::validate()
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

// Convert a time in simulation time units to clock cycles
int Testbench::clock_cycle( sc_time time )
{
    sc_clock * clk_p = dynamic_cast < sc_clock * >( i_clk.get_interface() );
    sc_time clock_period = clk_p->period(); // get period from the sc_clock object.
    return ( int )( time / clock_period );
}
