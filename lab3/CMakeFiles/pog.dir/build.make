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
include my_proj/lab3/CMakeFiles/pog.dir/depend.make

# Include the progress variables for this target.
include my_proj/lab3/CMakeFiles/pog.dir/progress.make

# Include the compile flags for this target's objects.
include my_proj/lab3/CMakeFiles/pog.dir/flags.make

my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o: my_proj/lab3/CMakeFiles/pog.dir/flags.make
my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o: my_proj/lab3/chernovik.c
	$(CMAKE_COMMAND) -E cmake_progress_report /glfw-3.1.2/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o"
	cd /glfw-3.1.2/my_proj/lab3 && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/pog.dir/chernovik.c.o   -c /glfw-3.1.2/my_proj/lab3/chernovik.c

my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pog.dir/chernovik.c.i"
	cd /glfw-3.1.2/my_proj/lab3 && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /glfw-3.1.2/my_proj/lab3/chernovik.c > CMakeFiles/pog.dir/chernovik.c.i

my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pog.dir/chernovik.c.s"
	cd /glfw-3.1.2/my_proj/lab3 && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /glfw-3.1.2/my_proj/lab3/chernovik.c -o CMakeFiles/pog.dir/chernovik.c.s

my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.requires:
.PHONY : my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.requires

my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.provides: my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.requires
	$(MAKE) -f my_proj/lab3/CMakeFiles/pog.dir/build.make my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.provides.build
.PHONY : my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.provides

my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.provides.build: my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o

# Object files for target pog
pog_OBJECTS = \
"CMakeFiles/pog.dir/chernovik.c.o"

# External object files for target pog
pog_EXTERNAL_OBJECTS =

my_proj/lab3/pog: my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o
my_proj/lab3/pog: my_proj/lab3/CMakeFiles/pog.dir/build.make
my_proj/lab3/pog: src/libglfw3.a
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/librt.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libm.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libX11.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXrandr.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXinerama.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXi.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXcursor.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libGL.so
my_proj/lab3/pog: src/libglfw3.a
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/librt.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libm.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libX11.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXrandr.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXinerama.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXi.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libXcursor.so
my_proj/lab3/pog: /usr/lib/x86_64-linux-gnu/libGL.so
my_proj/lab3/pog: my_proj/lab3/CMakeFiles/pog.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable pog"
	cd /glfw-3.1.2/my_proj/lab3 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pog.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
my_proj/lab3/CMakeFiles/pog.dir/build: my_proj/lab3/pog
.PHONY : my_proj/lab3/CMakeFiles/pog.dir/build

my_proj/lab3/CMakeFiles/pog.dir/requires: my_proj/lab3/CMakeFiles/pog.dir/chernovik.c.o.requires
.PHONY : my_proj/lab3/CMakeFiles/pog.dir/requires

my_proj/lab3/CMakeFiles/pog.dir/clean:
	cd /glfw-3.1.2/my_proj/lab3 && $(CMAKE_COMMAND) -P CMakeFiles/pog.dir/cmake_clean.cmake
.PHONY : my_proj/lab3/CMakeFiles/pog.dir/clean

my_proj/lab3/CMakeFiles/pog.dir/depend:
	cd /glfw-3.1.2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /glfw-3.1.2 /glfw-3.1.2/my_proj/lab3 /glfw-3.1.2 /glfw-3.1.2/my_proj/lab3 /glfw-3.1.2/my_proj/lab3/CMakeFiles/pog.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : my_proj/lab3/CMakeFiles/pog.dir/depend

