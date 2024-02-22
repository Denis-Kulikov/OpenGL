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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/prog/OpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/prog/OpenGL

# Include any dependencies generated for this target.
include src/objects/CMakeFiles/lib-object.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/objects/CMakeFiles/lib-object.dir/compiler_depend.make

# Include the progress variables for this target.
include src/objects/CMakeFiles/lib-object.dir/progress.make

# Include the compile flags for this target's objects.
include src/objects/CMakeFiles/lib-object.dir/flags.make

src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.o: src/objects/CMakeFiles/lib-object.dir/flags.make
src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.o: src/objects/ObjectTransform.cpp
src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.o: src/objects/CMakeFiles/lib-object.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/prog/OpenGL/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.o"
	cd /mnt/c/prog/OpenGL/src/objects && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.o -MF CMakeFiles/lib-object.dir/ObjectTransform.cpp.o.d -o CMakeFiles/lib-object.dir/ObjectTransform.cpp.o -c /mnt/c/prog/OpenGL/src/objects/ObjectTransform.cpp

src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lib-object.dir/ObjectTransform.cpp.i"
	cd /mnt/c/prog/OpenGL/src/objects && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/prog/OpenGL/src/objects/ObjectTransform.cpp > CMakeFiles/lib-object.dir/ObjectTransform.cpp.i

src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lib-object.dir/ObjectTransform.cpp.s"
	cd /mnt/c/prog/OpenGL/src/objects && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/prog/OpenGL/src/objects/ObjectTransform.cpp -o CMakeFiles/lib-object.dir/ObjectTransform.cpp.s

# Object files for target lib-object
lib__object_OBJECTS = \
"CMakeFiles/lib-object.dir/ObjectTransform.cpp.o"

# External object files for target lib-object
lib__object_EXTERNAL_OBJECTS =

src/objects/liblib-object.a: src/objects/CMakeFiles/lib-object.dir/ObjectTransform.cpp.o
src/objects/liblib-object.a: src/objects/CMakeFiles/lib-object.dir/build.make
src/objects/liblib-object.a: src/objects/CMakeFiles/lib-object.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/prog/OpenGL/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblib-object.a"
	cd /mnt/c/prog/OpenGL/src/objects && $(CMAKE_COMMAND) -P CMakeFiles/lib-object.dir/cmake_clean_target.cmake
	cd /mnt/c/prog/OpenGL/src/objects && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib-object.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/objects/CMakeFiles/lib-object.dir/build: src/objects/liblib-object.a
.PHONY : src/objects/CMakeFiles/lib-object.dir/build

src/objects/CMakeFiles/lib-object.dir/clean:
	cd /mnt/c/prog/OpenGL/src/objects && $(CMAKE_COMMAND) -P CMakeFiles/lib-object.dir/cmake_clean.cmake
.PHONY : src/objects/CMakeFiles/lib-object.dir/clean

src/objects/CMakeFiles/lib-object.dir/depend:
	cd /mnt/c/prog/OpenGL && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/prog/OpenGL /mnt/c/prog/OpenGL/src/objects /mnt/c/prog/OpenGL /mnt/c/prog/OpenGL/src/objects /mnt/c/prog/OpenGL/src/objects/CMakeFiles/lib-object.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/objects/CMakeFiles/lib-object.dir/depend

