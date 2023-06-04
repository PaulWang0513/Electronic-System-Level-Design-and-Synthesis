# Final Project
- This folder contains all files for final project
- [data](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/final_project/data): the folder that contains the test data, golden data, and the python script to generate them.
- [memlib](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/final_project/memlib): the memory library for HLS implementation part. Three memories are avalible: RAM_1000X8, RAM_500X8, RAM_250X8.
- [risv-vp](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/final_project/risv-vp): the folder that contains the code for riscv-vp implementation part. Both 1core and 2core version are included.
- [source](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/final_project/source): the folder that contains the source code for HLS implementation part.
- [stratus](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/final_project/stratus): the folder that contains project.tcl to run the HLS implementation on Stratus HLS.




## Quick Run
- Assum the directory structure is as follows:
  - ee6470/
    - hw4/
    - riscv-vp/
- You can choose the test image by modifying the `test_image` variable in `hw4\riscv-vp\sw\basic-sobel\main.cpp`
  - Available test image name: `lena_std_short`, `lake_noise`, `jetplane_noise` (default)
- Run the following commands:
  - `cd ee6470`
  - `cp -r hw4/riscv-vp/vp/src/platform/* riscv-vp/vp/src/platform`
  - `cp -r hw4/riscv-vp/sw/* riscv-vp/sw`
  - `cd ee6470/riscv-vp/vp/build`
  - `cmake ..`
  - `make install`
  - `cd ee6470/riscv-vp/sw/basic-sobel`
  - `make`
  - `make sim`


## HLS result recording

| Implemantation | clock period (ns) | Area   | Avg. Latency (cycles) | Total Runtime (ns) | Description |
| -------------- | ----------------- | ------ | --------------------- | ------------------ | ----------- | 
| BASIC          | 10                | 3843.2 | 2002                  | 20040010           | 1000 signals in RAM_1000X8 | 
| MEM_1000X2     | 10                | 4749.2 | 1002                  | 10040010           | using 2 RAM_1000X8, both storing 1000 signal |
| MEM_500X2      | 10                | 5675.3 |  752                  |  7540030           | using 2 RAM_500X8, both storing 500 signal |
| MEM_250X4      | 10                | 9969.6 |  377                  |  3790070           | using 4 RAM_250X8, all storing 250 signal (4 loops) |
| MEM_250X4_AREA | 10                | 7999.7 |  380                  |  3812570           | using 4 RAM_250X8, all storing 250 signal (2 loops only) |