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
CMAKE_BINARY_DIR = /home/henry/HaGame

# Include any dependencies generated for this target.
include CMakeFiles/HaGame.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HaGame.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HaGame.dir/flags.make

CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.o: CMakeFiles/HaGame.dir/flags.make
CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.o: HaGame/HaGame.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/HaGame/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.o -c /home/henry/HaGame/HaGame/HaGame.cpp

CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/HaGame/HaGame/HaGame.cpp > CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.i

CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/HaGame/HaGame/HaGame.cpp -o CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.s

# Object files for target HaGame
HaGame_OBJECTS = \
"CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.o"

# External object files for target HaGame
HaGame_EXTERNAL_OBJECTS =

HaGame: CMakeFiles/HaGame.dir/HaGame/HaGame.cpp.o
HaGame: CMakeFiles/HaGame.dir/build.make
HaGame: /usr/lib/x86_64-linux-gnu/libGL.so
HaGame: /usr/lib/x86_64-linux-gnu/libGLU.so
HaGame: CMakeFiles/HaGame.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/henry/HaGame/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HaGame"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HaGame.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HaGame.dir/build: HaGame

.PHONY : CMakeFiles/HaGame.dir/build

CMakeFiles/HaGame.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HaGame.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HaGame.dir/clean

CMakeFiles/HaGame.dir/depend:
	cd /home/henry/HaGame && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/henry/HaGame /home/henry/HaGame /home/henry/HaGame /home/henry/HaGame /home/henry/HaGame/CMakeFiles/HaGame.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HaGame.dir/depend

