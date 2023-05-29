# hw4
- This folder contains all files for homework 4
- [riscv-vp/sw/basic-sobel](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw4/riscv-vp/sw/basic-sobel): the folder that contains the sw files for simulation.
- [riscv-vp/vp/src/platform/basic-acc](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw4/riscv-vp/vp/src/platform/basic-acc): the folder that contains the files to build the basic-acc platform.

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
