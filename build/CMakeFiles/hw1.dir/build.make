# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build

# Include any dependencies generated for this target.
include CMakeFiles/hw1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hw1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw1.dir/flags.make

CMakeFiles/hw1.dir/main.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hw1.dir/main.c.obj"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hw1.dir/main.c.obj   -c C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/main.c

CMakeFiles/hw1.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/main.c.i"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/main.c > CMakeFiles/hw1.dir/main.c.i

CMakeFiles/hw1.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/main.c.s"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/main.c -o CMakeFiles/hw1.dir/main.c.s

CMakeFiles/hw1.dir/mtm_map/map.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/mtm_map/map.c.obj: ../mtm_map/map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/hw1.dir/mtm_map/map.c.obj"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hw1.dir/mtm_map/map.c.obj   -c C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/mtm_map/map.c

CMakeFiles/hw1.dir/mtm_map/map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/mtm_map/map.c.i"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/mtm_map/map.c > CMakeFiles/hw1.dir/mtm_map/map.c.i

CMakeFiles/hw1.dir/mtm_map/map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/mtm_map/map.c.s"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/mtm_map/map.c -o CMakeFiles/hw1.dir/mtm_map/map.c.s

CMakeFiles/hw1.dir/mtm_map/node.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/mtm_map/node.c.obj: ../mtm_map/node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/hw1.dir/mtm_map/node.c.obj"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hw1.dir/mtm_map/node.c.obj   -c C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/mtm_map/node.c

CMakeFiles/hw1.dir/mtm_map/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/mtm_map/node.c.i"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/mtm_map/node.c > CMakeFiles/hw1.dir/mtm_map/node.c.i

CMakeFiles/hw1.dir/mtm_map/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/mtm_map/node.c.s"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/mtm_map/node.c -o CMakeFiles/hw1.dir/mtm_map/node.c.s

CMakeFiles/hw1.dir/election.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/election.c.obj: ../election.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/hw1.dir/election.c.obj"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hw1.dir/election.c.obj   -c C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/election.c

CMakeFiles/hw1.dir/election.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/election.c.i"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/election.c > CMakeFiles/hw1.dir/election.c.i

CMakeFiles/hw1.dir/election.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/election.c.s"
	C:/TDM-GCC-64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/election.c -o CMakeFiles/hw1.dir/election.c.s

# Object files for target hw1
hw1_OBJECTS = \
"CMakeFiles/hw1.dir/main.c.obj" \
"CMakeFiles/hw1.dir/mtm_map/map.c.obj" \
"CMakeFiles/hw1.dir/mtm_map/node.c.obj" \
"CMakeFiles/hw1.dir/election.c.obj"

# External object files for target hw1
hw1_EXTERNAL_OBJECTS =

hw1.exe: CMakeFiles/hw1.dir/main.c.obj
hw1.exe: CMakeFiles/hw1.dir/mtm_map/map.c.obj
hw1.exe: CMakeFiles/hw1.dir/mtm_map/node.c.obj
hw1.exe: CMakeFiles/hw1.dir/election.c.obj
hw1.exe: CMakeFiles/hw1.dir/build.make
hw1.exe: CMakeFiles/hw1.dir/linklibs.rsp
hw1.exe: CMakeFiles/hw1.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable hw1.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/hw1.dir/objects.a
	C:/TDM-GCC-64/bin/ar.exe cr CMakeFiles/hw1.dir/objects.a @CMakeFiles/hw1.dir/objects1.rsp
	C:/TDM-GCC-64/bin/gcc.exe -std=c99 --pedantic-errors -Wall -Werror -g -g   -Wl,--whole-archive CMakeFiles/hw1.dir/objects.a -Wl,--no-whole-archive  -o hw1.exe -Wl,--out-implib,libhw1.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/hw1.dir/linklibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/hw1.dir/build: hw1.exe

.PHONY : CMakeFiles/hw1.dir/build

CMakeFiles/hw1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw1.dir/clean

CMakeFiles/hw1.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1 C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1 C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build C:/Users/ielia/Desktop/Technion/MATAM/homework/ex1/build/CMakeFiles/hw1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw1.dir/depend

