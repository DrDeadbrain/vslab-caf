# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /home/baal/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/baal/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/baal/Dokumente/Repos/vslab-caf

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/vslab.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/vslab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vslab.dir/flags.make

CMakeFiles/vslab.dir/src/int512_serialization.cpp.o: CMakeFiles/vslab.dir/flags.make
CMakeFiles/vslab.dir/src/int512_serialization.cpp.o: ../src/int512_serialization.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/vslab.dir/src/int512_serialization.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vslab.dir/src/int512_serialization.cpp.o -c /home/baal/Dokumente/Repos/vslab-caf/src/int512_serialization.cpp

CMakeFiles/vslab.dir/src/int512_serialization.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vslab.dir/src/int512_serialization.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baal/Dokumente/Repos/vslab-caf/src/int512_serialization.cpp > CMakeFiles/vslab.dir/src/int512_serialization.cpp.i

CMakeFiles/vslab.dir/src/int512_serialization.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vslab.dir/src/int512_serialization.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baal/Dokumente/Repos/vslab-caf/src/int512_serialization.cpp -o CMakeFiles/vslab.dir/src/int512_serialization.cpp.s

CMakeFiles/vslab.dir/src/main.cpp.o: CMakeFiles/vslab.dir/flags.make
CMakeFiles/vslab.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/vslab.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vslab.dir/src/main.cpp.o -c /home/baal/Dokumente/Repos/vslab-caf/src/main.cpp

CMakeFiles/vslab.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vslab.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baal/Dokumente/Repos/vslab-caf/src/main.cpp > CMakeFiles/vslab.dir/src/main.cpp.i

CMakeFiles/vslab.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vslab.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baal/Dokumente/Repos/vslab-caf/src/main.cpp -o CMakeFiles/vslab.dir/src/main.cpp.s

CMakeFiles/vslab.dir/src/calc/calculator.cpp.o: CMakeFiles/vslab.dir/flags.make
CMakeFiles/vslab.dir/src/calc/calculator.cpp.o: ../src/calc/calculator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/vslab.dir/src/calc/calculator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vslab.dir/src/calc/calculator.cpp.o -c /home/baal/Dokumente/Repos/vslab-caf/src/calc/calculator.cpp

CMakeFiles/vslab.dir/src/calc/calculator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vslab.dir/src/calc/calculator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baal/Dokumente/Repos/vslab-caf/src/calc/calculator.cpp > CMakeFiles/vslab.dir/src/calc/calculator.cpp.i

CMakeFiles/vslab.dir/src/calc/calculator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vslab.dir/src/calc/calculator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baal/Dokumente/Repos/vslab-caf/src/calc/calculator.cpp -o CMakeFiles/vslab.dir/src/calc/calculator.cpp.s

# Object files for target vslab
vslab_OBJECTS = \
"CMakeFiles/vslab.dir/src/int512_serialization.cpp.o" \
"CMakeFiles/vslab.dir/src/main.cpp.o" \
"CMakeFiles/vslab.dir/src/calc/calculator.cpp.o"

# External object files for target vslab
vslab_EXTERNAL_OBJECTS =

vslab: CMakeFiles/vslab.dir/src/int512_serialization.cpp.o
vslab: CMakeFiles/vslab.dir/src/main.cpp.o
vslab: CMakeFiles/vslab.dir/src/calc/calculator.cpp.o
vslab: CMakeFiles/vslab.dir/build.make
vslab: _deps/actor_framework-build/libcaf_io/libcaf_io.so.0.18.0
vslab: _deps/actor_framework-build/libcaf_core/libcaf_core.so.0.18.0
vslab: CMakeFiles/vslab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable vslab"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vslab.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vslab.dir/build: vslab
.PHONY : CMakeFiles/vslab.dir/build

CMakeFiles/vslab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vslab.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vslab.dir/clean

CMakeFiles/vslab.dir/depend:
	cd /home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/baal/Dokumente/Repos/vslab-caf /home/baal/Dokumente/Repos/vslab-caf /home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug /home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug /home/baal/Dokumente/Repos/vslab-caf/cmake-build-debug/CMakeFiles/vslab.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vslab.dir/depend

