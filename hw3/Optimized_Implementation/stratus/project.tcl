#*******************************************************************************
# Copyright 2015 Cadence Design Systems, Inc.
# All Rights Reserved.
#
#*******************************************************************************
#
# Stratus Project File
#
############################################################
# Project Parameters
############################################################
#
# Technology Libraries
#
set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
set LIB_LEAF "slow_vdd1v2_basicCells.lib"
use_tech_lib    "$LIB_PATH/$LIB_LEAF"

#
# Global synthesis attributes.
#
set_attr clock_period           10.0
set_attr message_detail         3 
#set_attr default_input_delay    0.1
#set_attr sched_aggressive_1 on
#set_attr unroll_loops on
#set_attr flatten_arrays all 
#set_attr timing_aggression 0
#set_attr default_protocol true

#
# Simulation Options
#
### 1. You may add your own options for C++ compilation here.
set_attr cc_options             "-DCLOCK_PERIOD=10.0 -g"
#enable_waveform_logging -vcd
set_attr end_of_sim_command "make saySimPassed"

#
# Testbench or System Level Modules
#
### 2. Add your testbench source files here.
define_system_module ../main.cpp
define_system_module ../Testbench.cpp
define_system_module ../System.cpp

#
# SC_MODULEs to be synthesized
#
### 3. Add your design source files here (to be high-level synthesized).
define_hls_module MedianMeanFilter ../MedianMeanFilter.cpp

### 4. Define your HLS configuration (arbitrary names, BASIC and DPA in this example). 
define_hls_config MedianMeanFilter BASIC
define_hls_config MedianMeanFilter OPTIMIZE --flatten_arrays=all --unroll_loops=on --dpopt_auto=all -post_elab_tcl {
    map_to_reg_bank [find -array "buffer"]
}
define_hls_config MedianMeanFilter OPTIMIZE_2 --clock_period=2.0 --flatten_arrays=all --unroll_loops=on --dpopt_auto=all -post_elab_tcl {
    map_to_reg_bank [find -array "buffer"]
}
define_hls_config MedianMeanFilter OPTIMIZE_3 --clock_period=1.0 --flatten_arrays=all --unroll_loops=on --dpopt_auto=all -post_elab_tcl {
    map_to_reg_bank [find -array "buffer"]
}

set IMAGE_DIR           "../../noise_images"
set IN_FILE_NAME        "${IMAGE_DIR}/lake_noise.bmp"
set OUT_FILE_NAME				"../output/out.bmp"

### 5. Define simulation configuration for each HLS configuration
### 5.1 The behavioral simulation (C++ only).
define_sim_config B -argv "$IN_FILE_NAME $OUT_FILE_NAME"
define_sim_config B_lake -argv "${IMAGE_DIR}/lake_noise.bmp $OUT_FILE_NAME"
define_sim_config B_jetplane -argv "${IMAGE_DIR}/jetplane_noise.bmp $OUT_FILE_NAME"

### 5.2 The Verilog simulation for HLS config "BASIC". 
define_sim_config V_BASIC "MedianMeanFilter RTL_V BASIC" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
define_sim_config V_BASIC_lake "MedianMeanFilter RTL_V BASIC" -argv "${IMAGE_DIR}/lake_noise.bmp $OUT_FILE_NAME"
define_sim_config V_BASIC_jetplane "MedianMeanFilter RTL_V BASIC" -argv "${IMAGE_DIR}/jetplane_noise.bmp $OUT_FILE_NAME"

### 5.3 The Verilog simulation for HLS config "OPTIMIZE". 
define_sim_config V_OPTIMIZE "MedianMeanFilter RTL_V OPTIMIZE" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
define_sim_config V_OPTIMIZE_lake "MedianMeanFilter RTL_V OPTIMIZE" -argv "${IMAGE_DIR}/lake_noise.bmp $OUT_FILE_NAME"
define_sim_config V_OPTIMIZE_jetplane "MedianMeanFilter RTL_V OPTIMIZE" -argv "${IMAGE_DIR}/jetplane_noise.bmp $OUT_FILE_NAME"

### 5.4 The Verilog simulation for HLS config "OPTIMIZE_2". 
define_sim_config V_OPTIMIZE_2 "MedianMeanFilter RTL_V OPTIMIZE_2" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
define_sim_config V_OPTIMIZE_2_lake "MedianMeanFilter RTL_V OPTIMIZE_2" -argv "${IMAGE_DIR}/lake_noise.bmp $OUT_FILE_NAME"
define_sim_config V_OPTIMIZE_2_jetplane "MedianMeanFilter RTL_V OPTIMIZE_2" -argv "${IMAGE_DIR}/jetplane_noise.bmp $OUT_FILE_NAME"

### 5.5 The Verilog simulation for HLS config "OPTIMIZE_3". 
define_sim_config V_OPTIMIZE_3 "MedianMeanFilter RTL_V OPTIMIZE_3" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
define_sim_config V_OPTIMIZE_3_lake "MedianMeanFilter RTL_V OPTIMIZE_3" -argv "${IMAGE_DIR}/lake_noise.bmp $OUT_FILE_NAME"
define_sim_config V_OPTIMIZE_3_jetplane "MedianMeanFilter RTL_V OPTIMIZE_3" -argv "${IMAGE_DIR}/jetplane_noise.bmp $OUT_FILE_NAME"