# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.1.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.1.3\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hw1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hw1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw1.dir/flags.make

CMakeFiles/hw1.dir/main.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hw1.dir/main.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw1.dir\main.c.obj   -c C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\main.c

CMakeFiles/hw1.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/main.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\main.c > CMakeFiles\hw1.dir\main.c.i

CMakeFiles/hw1.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/main.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\main.c -o CMakeFiles\hw1.dir\main.c.s

CMakeFiles/hw1.dir/mtm_map/map.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/mtm_map/map.c.obj: ../mtm_map/map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/hw1.dir/mtm_map/map.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw1.dir\mtm_map\map.c.obj   -c C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\mtm_map\map.c

CMakeFiles/hw1.dir/mtm_map/map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/mtm_map/map.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\mtm_map\map.c > CMakeFiles\hw1.dir\mtm_map\map.c.i

CMakeFiles/hw1.dir/mtm_map/map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/mtm_map/map.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\mtm_map\map.c -o CMakeFiles\hw1.dir\mtm_map\map.c.s

CMakeFiles/hw1.dir/mtm_map/node.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/mtm_map/node.c.obj: ../mtm_map/node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/hw1.dir/mtm_map/node.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw1.dir\mtm_map\node.c.obj   -c C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\mtm_map\node.c

CMakeFiles/hw1.dir/mtm_map/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/mtm_map/node.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\mtm_map\node.c > CMakeFiles\hw1.dir\mtm_map\node.c.i

CMakeFiles/hw1.dir/mtm_map/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/mtm_map/node.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\mtm_map\node.c -o CMakeFiles\hw1.dir\mtm_map\node.c.s

CMakeFiles/hw1.dir/election.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/election.c.obj: ../election.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/hw1.dir/election.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw1.dir\election.c.obj   -c C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\election.c

CMakeFiles/hw1.dir/election.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/election.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\election.c > CMakeFiles\hw1.dir\election.c.i

CMakeFiles/hw1.dir/election.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/election.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\election.c -o CMakeFiles\hw1.dir\election.c.s

CMakeFiles/hw1.dir/map_serializer.c.obj: CMakeFiles/hw1.dir/flags.make
CMakeFiles/hw1.dir/map_serializer.c.obj: ../map_serializer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/hw1.dir/map_serializer.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw1.dir\map_serializer.c.obj   -c C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\map_serializer.c

CMakeFiles/hw1.dir/map_serializer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw1.dir/map_serializer.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\map_serializer.c > CMakeFiles\hw1.dir\map_serializer.c.i

CMakeFiles/hw1.dir/map_serializer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw1.dir/map_serializer.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\map_serializer.c -o CMakeFiles\hw1.dir\map_serializer.c.s

# Object files for target hw1
hw1_OBJECTS = \
"CMakeFiles/hw1.dir/main.c.obj" \
"CMakeFiles/hw1.dir/mtm_map/map.c.obj" \
"CMakeFiles/hw1.dir/mtm_map/node.c.obj" \
"CMakeFiles/hw1.dir/election.c.obj" \
"CMakeFiles/hw1.dir/map_serializer.c.obj"

# External object files for target hw1
hw1_EXTERNAL_OBJECTS =

hw1.exe: CMakeFiles/hw1.dir/main.c.obj
hw1.exe: CMakeFiles/hw1.dir/mtm_map/map.c.obj
hw1.exe: CMakeFiles/hw1.dir/mtm_map/node.c.obj
hw1.exe: CMakeFiles/hw1.dir/election.c.obj
hw1.exe: CMakeFiles/hw1.dir/map_serializer.c.obj
hw1.exe: CMakeFiles/hw1.dir/build.make
hw1.exe: CMakeFiles/hw1.dir/linklibs.rsp
hw1.exe: CMakeFiles/hw1.dir/objects1.rsp
hw1.exe: CMakeFiles/hw1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable hw1.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\hw1.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw1.dir/build: hw1.exe

.PHONY : CMakeFiles/hw1.dir/build

CMakeFiles/hw1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\hw1.dir\cmake_clean.cmake
.PHONY : CMakeFiles/hw1.dir/clean

CMakeFiles/hw1.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1 C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1 C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug C:\Users\ielia\Desktop\Technion\MATAM\homework\ex1\cmake-build-debug\CMakeFiles\hw1.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw1.dir/depend

