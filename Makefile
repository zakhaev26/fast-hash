# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/soubhik/codes/fast-hash

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/soubhik/codes/fast-hash

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/soubhik/codes/fast-hash/CMakeFiles /home/soubhik/codes/fast-hash//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/soubhik/codes/fast-hash/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named FastHash

# Build rule for target.
FastHash: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 FastHash
.PHONY : FastHash

# fast build rule for target.
FastHash/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/build
.PHONY : FastHash/fast

src/core/command.o: src/core/command.cpp.o
.PHONY : src/core/command.o

# target to build an object file
src/core/command.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/command.cpp.o
.PHONY : src/core/command.cpp.o

src/core/command.i: src/core/command.cpp.i
.PHONY : src/core/command.i

# target to preprocess a source file
src/core/command.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/command.cpp.i
.PHONY : src/core/command.cpp.i

src/core/command.s: src/core/command.cpp.s
.PHONY : src/core/command.s

# target to generate assembly for a file
src/core/command.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/command.cpp.s
.PHONY : src/core/command.cpp.s

src/core/fast-hash.o: src/core/fast-hash.cpp.o
.PHONY : src/core/fast-hash.o

# target to build an object file
src/core/fast-hash.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/fast-hash.cpp.o
.PHONY : src/core/fast-hash.cpp.o

src/core/fast-hash.i: src/core/fast-hash.cpp.i
.PHONY : src/core/fast-hash.i

# target to preprocess a source file
src/core/fast-hash.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/fast-hash.cpp.i
.PHONY : src/core/fast-hash.cpp.i

src/core/fast-hash.s: src/core/fast-hash.cpp.s
.PHONY : src/core/fast-hash.s

# target to generate assembly for a file
src/core/fast-hash.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/fast-hash.cpp.s
.PHONY : src/core/fast-hash.cpp.s

src/core/parser.o: src/core/parser.cpp.o
.PHONY : src/core/parser.o

# target to build an object file
src/core/parser.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/parser.cpp.o
.PHONY : src/core/parser.cpp.o

src/core/parser.i: src/core/parser.cpp.i
.PHONY : src/core/parser.i

# target to preprocess a source file
src/core/parser.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/parser.cpp.i
.PHONY : src/core/parser.cpp.i

src/core/parser.s: src/core/parser.cpp.s
.PHONY : src/core/parser.s

# target to generate assembly for a file
src/core/parser.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/parser.cpp.s
.PHONY : src/core/parser.cpp.s

src/core/ttl_manager.o: src/core/ttl_manager.cpp.o
.PHONY : src/core/ttl_manager.o

# target to build an object file
src/core/ttl_manager.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/ttl_manager.cpp.o
.PHONY : src/core/ttl_manager.cpp.o

src/core/ttl_manager.i: src/core/ttl_manager.cpp.i
.PHONY : src/core/ttl_manager.i

# target to preprocess a source file
src/core/ttl_manager.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/ttl_manager.cpp.i
.PHONY : src/core/ttl_manager.cpp.i

src/core/ttl_manager.s: src/core/ttl_manager.cpp.s
.PHONY : src/core/ttl_manager.s

# target to generate assembly for a file
src/core/ttl_manager.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/core/ttl_manager.cpp.s
.PHONY : src/core/ttl_manager.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/persistence/aof/aof.o: src/persistence/aof/aof.cpp.o
.PHONY : src/persistence/aof/aof.o

# target to build an object file
src/persistence/aof/aof.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/persistence/aof/aof.cpp.o
.PHONY : src/persistence/aof/aof.cpp.o

src/persistence/aof/aof.i: src/persistence/aof/aof.cpp.i
.PHONY : src/persistence/aof/aof.i

# target to preprocess a source file
src/persistence/aof/aof.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/persistence/aof/aof.cpp.i
.PHONY : src/persistence/aof/aof.cpp.i

src/persistence/aof/aof.s: src/persistence/aof/aof.cpp.s
.PHONY : src/persistence/aof/aof.s

# target to generate assembly for a file
src/persistence/aof/aof.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FastHash.dir/build.make CMakeFiles/FastHash.dir/src/persistence/aof/aof.cpp.s
.PHONY : src/persistence/aof/aof.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... FastHash"
	@echo "... src/core/command.o"
	@echo "... src/core/command.i"
	@echo "... src/core/command.s"
	@echo "... src/core/fast-hash.o"
	@echo "... src/core/fast-hash.i"
	@echo "... src/core/fast-hash.s"
	@echo "... src/core/parser.o"
	@echo "... src/core/parser.i"
	@echo "... src/core/parser.s"
	@echo "... src/core/ttl_manager.o"
	@echo "... src/core/ttl_manager.i"
	@echo "... src/core/ttl_manager.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/persistence/aof/aof.o"
	@echo "... src/persistence/aof/aof.i"
	@echo "... src/persistence/aof/aof.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

