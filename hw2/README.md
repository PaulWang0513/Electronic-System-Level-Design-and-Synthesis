# hw2
- This folder contains all files for homework 2 (Median and Mean Filter with TLM).
- [filter_seperate_channel](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw2/filter_seperate_channel): the folder that contains all the files implementing the filter module **without** TLM interface.
  - It is a modified version of filter_with_filter in hw1.
  - Please checkout the README inside the folder to see how to run the modules.
- [filter_with_tlm](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw2/filter_with_tlm): the folder that contains all the files implementing the filter module with **TLM interface**.
  - Please checkout the README inside the folder to see how to run the modules.
- [filter_with_tlm_qk](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw2/filter_with_tlm_qk): the folder that contains all the files implementing the filter module with **TLM interface and quantum keeper**.
  - Please checkout the README inside the folder to see how to run the modules.
- [filter_with_tlm_qk_bus](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw2/filter_with_tlm_qk_bus): the folder that contains all the files implementing the filter module with **TLM interface, quantum keeper, and bus**.
  - Please checkout the README inside the folder to see how to run the modules.
- [noise_images](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw2/noise_images): the folder that contains images with noise.
- simulation_time.csv: the csv file recording the experimental results.

## Quick Run
- Enter the `hw2/filter_with_tlm/build/` folder, then run `make run` to see the result of [jetplane.bmp](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/blob/main/hw2/noise_images/jetplane_noise.bmp) as out.bmp in `hw2/filter_with_tlm/output/` folder.
- Enter the `hw2/filter_with_tlm_qk/build/` folder, then run `make run` to see the result of [jetplane.bmp](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/blob/main/hw2/noise_images/jetplane_noise.bmp) as out.bmp in `hw2/filter_with_tlm_qk/output/` folder.
- Enter the `hw2/filter_with_tlm_qk_bus/build/` folder, then run `make run` to see the result of [jetplane.bmp](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/blob/main/hw2/noise_images/jetplane_noise.bmp) as out.bmp in `hw2/filter_with_tlm_qk_bus/output/` folder.
