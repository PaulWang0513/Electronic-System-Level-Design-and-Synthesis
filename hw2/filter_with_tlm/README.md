# filter_with_tlm
- Files in this folder are used to implement the filter module with **TLM interface**.
- There are already two images in the [output folder](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw2/filter_with_tlm/output), both are from [noise_images folder](https://github.com/PaulWang0513/Electronic-System-Level-Design-and-Synthesis/tree/main/hw2/noise_images) and are processed by the filter module in this folder.
- To run the filter module, please follow the steps below:
    1. Enter the build folder: `cd build`
    2. Run the module `./median_mean ../../noise_images/<input file name> ../output/<output file name>`
        - For example, `./median_mean ../../noise_images/lake_noise.bmp ../output/lake_out.bmp` will process the image "lake_noise.bmp" from the noise_images folder and output the result as "lake_out.bmp" to output folder.
        - Currently, only "lake_noise.bmp" and "jetplane_noise.bmp" are available.
    3. Or you can simply run `make run` to run the module with default input (jetplane_noise.bmp) and output (out.bmp).