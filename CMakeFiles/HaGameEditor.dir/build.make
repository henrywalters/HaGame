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
include CMakeFiles/HaGameEditor.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HaGameEditor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HaGameEditor.dir/flags.make

CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.o: CMakeFiles/HaGameEditor.dir/flags.make
CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.o: Editor/Editor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/HaGame/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.o -c /home/henry/HaGame/Editor/Editor.cpp

CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/HaGame/Editor/Editor.cpp > CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.i

CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/HaGame/Editor/Editor.cpp -o CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.s

# Object files for target HaGameEditor
HaGameEditor_OBJECTS = \
"CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.o"

# External object files for target HaGameEditor
HaGameEditor_EXTERNAL_OBJECTS =

HaGameEditor: CMakeFiles/HaGameEditor.dir/Editor/Editor.cpp.o
HaGameEditor: CMakeFiles/HaGameEditor.dir/build.make
HaGameEditor: /usr/lib/x86_64-linux-gnu/libGL.so
HaGameEditor: /usr/lib/x86_64-linux-gnu/libGLU.so
HaGameEditor: CMakeFiles/HaGameEditor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/henry/HaGame/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HaGameEditor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HaGameEditor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HaGameEditor.dir/build: HaGameEditor

.PHONY : CMakeFiles/HaGameEditor.dir/build

CMakeFiles/HaGameEditor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HaGameEditor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HaGameEditor.dir/clean

CMakeFiles/HaGameEditor.dir/depend:
	cd /home/henry/HaGame && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/henry/HaGame /home/henry/HaGame /home/henry/HaGame /home/henry/HaGame /home/henry/HaGame/CMakeFiles/HaGameEditor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HaGameEditor.dir/depend

