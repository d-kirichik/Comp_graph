# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /glfw-3.1.2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /glfw-3.1.2

# Include any dependencies generated for this target.
include my_proj/lab3/CMakeFiles/lab3.dir/depend.make

# Include the progress variables for this target.
include my_proj/lab3/CMakeFiles/lab3.dir/progress.make

# Include the compile flags for this target's objects.
include my_proj/lab3/CMakeFiles/lab3.dir/flags.make

my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o: my_proj/lab3/CMakeFiles/lab3.dir/flags.make
my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o: my_proj/lab3/lab3.c
	$(CMAKE_COMMAND) -E cmake_progress_report /glfw-3.1.2/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o"
	cd /glfw-3.1.2/my_proj/lab3 && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lab3.dir/lab3.c.o   -c /glfw-3.1.2/my_proj/lab3/lab3.c

my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab3.dir/lab3.c.i"
	cd /glfw-3.1.2/my_proj/lab3 && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /glfw-3.1.2/my_proj/lab3/lab3.c > CMakeFiles/lab3.dir/lab3.c.i

my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab3.dir/lab3.c.s"
	cd /glfw-3.1.2/my_proj/lab3 && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /glfw-3.1.2/my_proj/lab3/lab3.c -o CMakeFiles/lab3.dir/lab3.c.s

my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.requires:
.PHONY : my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.requires

my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.provides: my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.requires
	$(MAKE) -f my_proj/lab3/CMakeFiles/lab3.dir/build.make my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.provides.build
.PHONY : my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.provides

my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.provides.build: my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o

# Object files for target lab3
lab3_OBJECTS = \
"CMakeFiles/lab3.dir/lab3.c.o"

# External object files for target lab3
lab3_EXTERNAL_OBJECTS =

my_proj/lab3/lab3: my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o
my_proj/lab3/lab3: my_proj/lab3/CMakeFiles/lab3.dir/build.make
my_proj/lab3/lab3: src/libglfw3.a
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/librt.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libm.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libX11.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXrandr.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXinerama.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXi.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXcursor.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libGL.so
my_proj/lab3/lab3: src/libglfw3.a
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/librt.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libm.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libX11.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXrandr.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXinerama.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXi.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libXcursor.so
my_proj/lab3/lab3: /usr/lib/x86_64-linux-gnu/libGL.so
my_proj/lab3/lab3: my_proj/lab3/CMakeFiles/lab3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable lab3"
	cd /glfw-3.1.2/my_proj/lab3 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
my_proj/lab3/CMakeFiles/lab3.dir/build: my_proj/lab3/lab3
.PHONY : my_proj/lab3/CMakeFiles/lab3.dir/build

my_proj/lab3/CMakeFiles/lab3.dir/requires: my_proj/lab3/CMakeFiles/lab3.dir/lab3.c.o.requires
.PHONY : my_proj/lab3/CMakeFiles/lab3.dir/requires

my_proj/lab3/CMakeFiles/lab3.dir/clean:
	cd /glfw-3.1.2/my_proj/lab3 && $(CMAKE_COMMAND) -P CMakeFiles/lab3.dir/cmake_clean.cmake
.PHONY : my_proj/lab3/CMakeFiles/lab3.dir/clean

my_proj/lab3/CMakeFiles/lab3.dir/depend:
	cd /glfw-3.1.2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /glfw-3.1.2 /glfw-3.1.2/my_proj/lab3 /glfw-3.1.2 /glfw-3.1.2/my_proj/lab3 /glfw-3.1.2/my_proj/lab3/CMakeFiles/lab3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : my_proj/lab3/CMakeFiles/lab3.dir/depend

