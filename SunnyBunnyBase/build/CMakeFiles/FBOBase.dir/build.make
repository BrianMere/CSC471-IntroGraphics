# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build

# Include any dependencies generated for this target.
include CMakeFiles/FBOBase.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/FBOBase.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/FBOBase.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FBOBase.dir/flags.make

CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/glad/src/glad.c
CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o -MF CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o.d -o CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/glad/src/glad.c

CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/glad/src/glad.c > CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.i

CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/glad/src/glad.c -o CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.s

CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/tiny_obj_loader/tiny_obj_loader.cpp
CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o -MF CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.d -o CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/tiny_obj_loader/tiny_obj_loader.cpp

CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/tiny_obj_loader/tiny_obj_loader.cpp > CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.i

CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/ext/tiny_obj_loader/tiny_obj_loader.cpp -o CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.s

CMakeFiles/FBOBase.dir/src/GLSL.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/GLSL.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLSL.cpp
CMakeFiles/FBOBase.dir/src/GLSL.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/FBOBase.dir/src/GLSL.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/GLSL.cpp.o -MF CMakeFiles/FBOBase.dir/src/GLSL.cpp.o.d -o CMakeFiles/FBOBase.dir/src/GLSL.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLSL.cpp

CMakeFiles/FBOBase.dir/src/GLSL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/GLSL.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLSL.cpp > CMakeFiles/FBOBase.dir/src/GLSL.cpp.i

CMakeFiles/FBOBase.dir/src/GLSL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/GLSL.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLSL.cpp -o CMakeFiles/FBOBase.dir/src/GLSL.cpp.s

CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLTextureWriter.cpp
CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o -MF CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o.d -o CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLTextureWriter.cpp

CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLTextureWriter.cpp > CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.i

CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/GLTextureWriter.cpp -o CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.s

CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/MatrixStack.cpp
CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o -MF CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o.d -o CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/MatrixStack.cpp

CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/MatrixStack.cpp > CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.i

CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/MatrixStack.cpp -o CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.s

CMakeFiles/FBOBase.dir/src/Program.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/Program.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Program.cpp
CMakeFiles/FBOBase.dir/src/Program.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/FBOBase.dir/src/Program.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/Program.cpp.o -MF CMakeFiles/FBOBase.dir/src/Program.cpp.o.d -o CMakeFiles/FBOBase.dir/src/Program.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Program.cpp

CMakeFiles/FBOBase.dir/src/Program.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/Program.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Program.cpp > CMakeFiles/FBOBase.dir/src/Program.cpp.i

CMakeFiles/FBOBase.dir/src/Program.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/Program.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Program.cpp -o CMakeFiles/FBOBase.dir/src/Program.cpp.s

CMakeFiles/FBOBase.dir/src/Shape.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/Shape.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Shape.cpp
CMakeFiles/FBOBase.dir/src/Shape.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/FBOBase.dir/src/Shape.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/Shape.cpp.o -MF CMakeFiles/FBOBase.dir/src/Shape.cpp.o.d -o CMakeFiles/FBOBase.dir/src/Shape.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Shape.cpp

CMakeFiles/FBOBase.dir/src/Shape.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/Shape.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Shape.cpp > CMakeFiles/FBOBase.dir/src/Shape.cpp.i

CMakeFiles/FBOBase.dir/src/Shape.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/Shape.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Shape.cpp -o CMakeFiles/FBOBase.dir/src/Shape.cpp.s

CMakeFiles/FBOBase.dir/src/Texture.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/Texture.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Texture.cpp
CMakeFiles/FBOBase.dir/src/Texture.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/FBOBase.dir/src/Texture.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/Texture.cpp.o -MF CMakeFiles/FBOBase.dir/src/Texture.cpp.o.d -o CMakeFiles/FBOBase.dir/src/Texture.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Texture.cpp

CMakeFiles/FBOBase.dir/src/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/Texture.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Texture.cpp > CMakeFiles/FBOBase.dir/src/Texture.cpp.i

CMakeFiles/FBOBase.dir/src/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/Texture.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/Texture.cpp -o CMakeFiles/FBOBase.dir/src/Texture.cpp.s

CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/WindowManager.cpp
CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o -MF CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o.d -o CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/WindowManager.cpp

CMakeFiles/FBOBase.dir/src/WindowManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/WindowManager.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/WindowManager.cpp > CMakeFiles/FBOBase.dir/src/WindowManager.cpp.i

CMakeFiles/FBOBase.dir/src/WindowManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/WindowManager.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/WindowManager.cpp -o CMakeFiles/FBOBase.dir/src/WindowManager.cpp.s

CMakeFiles/FBOBase.dir/src/main.cpp.o: CMakeFiles/FBOBase.dir/flags.make
CMakeFiles/FBOBase.dir/src/main.cpp.o: /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/main.cpp
CMakeFiles/FBOBase.dir/src/main.cpp.o: CMakeFiles/FBOBase.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/FBOBase.dir/src/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FBOBase.dir/src/main.cpp.o -MF CMakeFiles/FBOBase.dir/src/main.cpp.o.d -o CMakeFiles/FBOBase.dir/src/main.cpp.o -c /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/main.cpp

CMakeFiles/FBOBase.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FBOBase.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/main.cpp > CMakeFiles/FBOBase.dir/src/main.cpp.i

CMakeFiles/FBOBase.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FBOBase.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/src/main.cpp -o CMakeFiles/FBOBase.dir/src/main.cpp.s

# Object files for target FBOBase
FBOBase_OBJECTS = \
"CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o" \
"CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o" \
"CMakeFiles/FBOBase.dir/src/GLSL.cpp.o" \
"CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o" \
"CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o" \
"CMakeFiles/FBOBase.dir/src/Program.cpp.o" \
"CMakeFiles/FBOBase.dir/src/Shape.cpp.o" \
"CMakeFiles/FBOBase.dir/src/Texture.cpp.o" \
"CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o" \
"CMakeFiles/FBOBase.dir/src/main.cpp.o"

# External object files for target FBOBase
FBOBase_EXTERNAL_OBJECTS =

FBOBase: CMakeFiles/FBOBase.dir/ext/glad/src/glad.c.o
FBOBase: CMakeFiles/FBOBase.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/GLSL.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/GLTextureWriter.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/MatrixStack.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/Program.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/Shape.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/Texture.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/WindowManager.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/src/main.cpp.o
FBOBase: CMakeFiles/FBOBase.dir/build.make
FBOBase: CMakeFiles/FBOBase.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable FBOBase"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FBOBase.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FBOBase.dir/build: FBOBase
.PHONY : CMakeFiles/FBOBase.dir/build

CMakeFiles/FBOBase.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FBOBase.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FBOBase.dir/clean

CMakeFiles/FBOBase.dir/depend:
	cd /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build /home/brianm/Documents/Repositories/CSC471-IntroGraphics/SunnyBunnyBase/build/CMakeFiles/FBOBase.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/FBOBase.dir/depend

