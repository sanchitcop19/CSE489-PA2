# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/gbn.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gbn.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gbn.dir/flags.make

CMakeFiles/gbn.dir/src/gbn.cpp.o: CMakeFiles/gbn.dir/flags.make
CMakeFiles/gbn.dir/src/gbn.cpp.o: ../src/gbn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gbn.dir/src/gbn.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gbn.dir/src/gbn.cpp.o -c /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/src/gbn.cpp

CMakeFiles/gbn.dir/src/gbn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gbn.dir/src/gbn.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/src/gbn.cpp > CMakeFiles/gbn.dir/src/gbn.cpp.i

CMakeFiles/gbn.dir/src/gbn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gbn.dir/src/gbn.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/src/gbn.cpp -o CMakeFiles/gbn.dir/src/gbn.cpp.s

CMakeFiles/gbn.dir/src/simulator.cpp.o: CMakeFiles/gbn.dir/flags.make
CMakeFiles/gbn.dir/src/simulator.cpp.o: ../src/simulator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gbn.dir/src/simulator.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gbn.dir/src/simulator.cpp.o -c /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/src/simulator.cpp

CMakeFiles/gbn.dir/src/simulator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gbn.dir/src/simulator.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/src/simulator.cpp > CMakeFiles/gbn.dir/src/simulator.cpp.i

CMakeFiles/gbn.dir/src/simulator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gbn.dir/src/simulator.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/src/simulator.cpp -o CMakeFiles/gbn.dir/src/simulator.cpp.s

# Object files for target gbn
gbn_OBJECTS = \
"CMakeFiles/gbn.dir/src/gbn.cpp.o" \
"CMakeFiles/gbn.dir/src/simulator.cpp.o"

# External object files for target gbn
gbn_EXTERNAL_OBJECTS =

gbn: CMakeFiles/gbn.dir/src/gbn.cpp.o
gbn: CMakeFiles/gbn.dir/src/simulator.cpp.o
gbn: CMakeFiles/gbn.dir/build.make
gbn: CMakeFiles/gbn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable gbn"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gbn.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gbn.dir/build: gbn

.PHONY : CMakeFiles/gbn.dir/build

CMakeFiles/gbn.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gbn.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gbn.dir/clean

CMakeFiles/gbn.dir/depend:
	cd /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug /Users/SanchitBatra/Desktop/Projects/CSE489/cse489a2/sanchitb/cmake-build-debug/CMakeFiles/gbn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gbn.dir/depend

