use_hls_lib "../memlib"
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
define_system_module ../source/main.cpp
define_system_module ../source/Testbench.cpp
define_system_module ../source/System.cpp

#
# SC_MODULEs to be synthesized
#
### 3. Add your design source files here (to be high-level synthesized).
define_hls_module AutocorrelationFunction ../source/AutocorrelationFunction.cpp

### 4. Define your HLS configuration 
define_hls_config AutocorrelationFunction BASIC --dpopt_auto=op,expr --post_elab_tcl {
    map_to_memory -mem_type "RAM_1000X8" [find -array "mem"]
}
define_hls_config AutocorrelationFunction MEM_1000X2 --dpopt_auto=op,expr --post_elab_tcl {
    map_to_memory -mem_type "RAM_1000X8" [find -array "mem0"]
    map_to_memory -mem_type "RAM_1000X8" [find -array "mem1"]
}
define_hls_config AutocorrelationFunction MEM_500X2 --dpopt_auto=op,expr --post_elab_tcl {
    map_to_memory -mem_type "RAM_500X8" [find -array "mem0"]
    map_to_memory -mem_type "RAM_500X8" [find -array "mem1"]
}
define_hls_config AutocorrelationFunction MEM_250X4 --dpopt_auto=op,expr --post_elab_tcl {
    map_to_memory -mem_type "RAM_250X8" [find -array "mem0"]
    map_to_memory -mem_type "RAM_250X8" [find -array "mem1"]
    map_to_memory -mem_type "RAM_250X8" [find -array "mem2"]
    map_to_memory -mem_type "RAM_250X8" [find -array "mem3"]
}
define_hls_config AutocorrelationFunction MEM_250X4_AREA --dpopt_auto=op,expr --post_elab_tcl {
    map_to_memory -mem_type "RAM_250X8" [find -array "mem0"]
    map_to_memory -mem_type "RAM_250X8" [find -array "mem1"]
    map_to_memory -mem_type "RAM_250X8" [find -array "mem2"]
    map_to_memory -mem_type "RAM_250X8" [find -array "mem3"]
}

set TEST_FILE           "sin_wave"
set DATA_DIR            "../data"
set INPUT_FILE_NAME     "${DATA_DIR}/${TEST_FILE}.dat"
set GOLDEN_FILE_NAME	"${DATA_DIR}/${TEST_FILE}_golden.dat"

### 5. Define simulation configuration for each HLS configuration
### 5.1 The behavioral simulation (C++ only).
define_sim_config BASIC_B -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_1000X2_B -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_500X2_B -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_250X4_B -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_250X4_AREA_B -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"

### 5.2 The Verilog simulation (RTL).
define_sim_config BASIC_V "AutocorrelationFunction RTL_V BASIC" -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_1000X2_V "AutocorrelationFunction RTL_V MEM_1000X2" -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_500X2_V "AutocorrelationFunction RTL_V MEM_500X2" -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_250X4_V "AutocorrelationFunction RTL_V MEM_250X4" -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
define_sim_config MEM_250X4_AREA_V "AutocorrelationFunction RTL_V MEM_250X4_AREA" -argv "$INPUT_FILE_NAME $GOLDEN_FILE_NAME"
