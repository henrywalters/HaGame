# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/henry/HaGame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/henry/HaGame/build

# Include any dependencies generated for this target.
include CMakeFiles/ImSim.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ImSim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ImSim.dir/flags.make

CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.o: CMakeFiles/ImSim.dir/flags.make
CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.o: ../ImSim/ImSim.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/HaGame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.o"
	/bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.o -c /home/henry/HaGame/ImSim/ImSim.cpp

CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.i"
	/bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/HaGame/ImSim/ImSim.cpp > CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.i

CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.s"
	/bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/HaGame/ImSim/ImSim.cpp -o CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.s

# Object files for target ImSim
ImSim_OBJECTS = \
"CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.o"

# External object files for target ImSim
ImSim_EXTERNAL_OBJECTS =

ImSim: CMakeFiles/ImSim.dir/ImSim/ImSim.cpp.o
ImSim: CMakeFiles/ImSim.dir/build.make
ImSim: /usr/lib/x86_64-linux-gnu/libGL.so
ImSim: /usr/lib/x86_64-linux-gnu/libGLU.so
ImSim: CMakeFiles/ImSim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/henry/HaGame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ImSim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ImSim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ImSim.dir/build: ImSim

.PHONY : CMakeFiles/ImSim.dir/build

CMakeFiles/ImSim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ImSim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ImSim.dir/clean

CMakeFiles/ImSim.dir/depend:
	cd /home/henry/HaGame/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/henry/HaGame /home/henry/HaGame /home/henry/HaGame/build /home/henry/HaGame/build /home/henry/HaGame/build/CMakeFiles/ImSim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ImSim.dir/depend

