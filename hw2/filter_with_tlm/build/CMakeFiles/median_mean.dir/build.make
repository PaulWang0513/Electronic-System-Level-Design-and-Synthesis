# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build

# Include any dependencies generated for this target.
include CMakeFiles/median_mean.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/median_mean.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/median_mean.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/median_mean.dir/flags.make

CMakeFiles/median_mean.dir/Initiator.cpp.o: CMakeFiles/median_mean.dir/flags.make
CMakeFiles/median_mean.dir/Initiator.cpp.o: ../Initiator.cpp
CMakeFiles/median_mean.dir/Initiator.cpp.o: CMakeFiles/median_mean.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/median_mean.dir/Initiator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/median_mean.dir/Initiator.cpp.o -MF CMakeFiles/median_mean.dir/Initiator.cpp.o.d -o CMakeFiles/median_mean.dir/Initiator.cpp.o -c /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/Initiator.cpp

CMakeFiles/median_mean.dir/Initiator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/median_mean.dir/Initiator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/Initiator.cpp > CMakeFiles/median_mean.dir/Initiator.cpp.i

CMakeFiles/median_mean.dir/Initiator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/median_mean.dir/Initiator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/Initiator.cpp -o CMakeFiles/median_mean.dir/Initiator.cpp.s

CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o: CMakeFiles/median_mean.dir/flags.make
CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o: ../MedianMeanFilter.cpp
CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o: CMakeFiles/median_mean.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o -MF CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o.d -o CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o -c /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/MedianMeanFilter.cpp

CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/MedianMeanFilter.cpp > CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.i

CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/MedianMeanFilter.cpp -o CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.s

CMakeFiles/median_mean.dir/Testbench.cpp.o: CMakeFiles/median_mean.dir/flags.make
CMakeFiles/median_mean.dir/Testbench.cpp.o: ../Testbench.cpp
CMakeFiles/median_mean.dir/Testbench.cpp.o: CMakeFiles/median_mean.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/median_mean.dir/Testbench.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/median_mean.dir/Testbench.cpp.o -MF CMakeFiles/median_mean.dir/Testbench.cpp.o.d -o CMakeFiles/median_mean.dir/Testbench.cpp.o -c /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/Testbench.cpp

CMakeFiles/median_mean.dir/Testbench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/median_mean.dir/Testbench.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/Testbench.cpp > CMakeFiles/median_mean.dir/Testbench.cpp.i

CMakeFiles/median_mean.dir/Testbench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/median_mean.dir/Testbench.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/Testbench.cpp -o CMakeFiles/median_mean.dir/Testbench.cpp.s

CMakeFiles/median_mean.dir/main.cpp.o: CMakeFiles/median_mean.dir/flags.make
CMakeFiles/median_mean.dir/main.cpp.o: ../main.cpp
CMakeFiles/median_mean.dir/main.cpp.o: CMakeFiles/median_mean.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/median_mean.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/median_mean.dir/main.cpp.o -MF CMakeFiles/median_mean.dir/main.cpp.o.d -o CMakeFiles/median_mean.dir/main.cpp.o -c /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/main.cpp

CMakeFiles/median_mean.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/median_mean.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/main.cpp > CMakeFiles/median_mean.dir/main.cpp.i

CMakeFiles/median_mean.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/median_mean.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/main.cpp -o CMakeFiles/median_mean.dir/main.cpp.s

# Object files for target median_mean
median_mean_OBJECTS = \
"CMakeFiles/median_mean.dir/Initiator.cpp.o" \
"CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o" \
"CMakeFiles/median_mean.dir/Testbench.cpp.o" \
"CMakeFiles/median_mean.dir/main.cpp.o"

# External object files for target median_mean
median_mean_EXTERNAL_OBJECTS =

median_mean: CMakeFiles/median_mean.dir/Initiator.cpp.o
median_mean: CMakeFiles/median_mean.dir/MedianMeanFilter.cpp.o
median_mean: CMakeFiles/median_mean.dir/Testbench.cpp.o
median_mean: CMakeFiles/median_mean.dir/main.cpp.o
median_mean: CMakeFiles/median_mean.dir/build.make
median_mean: /opt/systemc/lib/libsystemc.so.2.3.3
median_mean: CMakeFiles/median_mean.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable median_mean"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/median_mean.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/median_mean.dir/build: median_mean
.PHONY : CMakeFiles/median_mean.dir/build

CMakeFiles/median_mean.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/median_mean.dir/cmake_clean.cmake
.PHONY : CMakeFiles/median_mean.dir/clean

CMakeFiles/median_mean.dir/depend:
	cd /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build /home/user/ee6470/Electronic-System-Level-Design-and-Synthesis/hw2/filter_with_tlm/build/CMakeFiles/median_mean.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/median_mean.dir/depend

