# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0074 NEW)

project (HaGame3D_v1) 

message("CMAKE_MODULE_PATH = ${CMAKE_SOURCE_DIR}/modules")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
 
find_package(SDL2 REQUIRED)

find_package(OpenGL REQUIRED)

message(STATUS "OpenGL_FOUND:= ${OpenGL_FOUND}")
 
find_package(GLM REQUIRED)
 
#list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/glew-2.1.0)

find_package(GLEW REQUIRED)

#find_package(GLUT REQUIRED)

message(STATUS "OpenGL LIBS: ${OPENGL_INCLUDE_DIR}") 
message(STATUS "GLEW_LIBS: ${GLEW_LIBS}")
message(STATUS "GLUT_LIBS: ${GLUT_INCLUDE_DIRS}")
message(STATUS "SDL2_DIR = ${SDL2_INCLUDE_DIRS}")
message(STATUS "GLM_INCLUDE_DIR = ${GLM_INCLUDE_DIRS}")
message(STATUS "SDL2_LIB = ${SDL2_LIBS}")

include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIRS})

add_executable(HaGame3D HaGame3D_v1/HaGame3D_v1.cpp "HaGame3D_v1/Graphics/Color.h"  "HaGame3D_v1/Input/Device.h" "HaGame3D_v1/Input/Devices/Gamepad.h" "HaGame3D_v1/Graphics/Window.cpp" "HaGame3D_v1/Graphics/Shader.cpp" "HaGame3D_v1/Utils/FileSystem.cpp" "HaGame3D_v1/Graphics/ShaderProgram.cpp" "HaGame3D_v1/Utils/Clock.h" "HaGame3D_v1/Graphics/Mesh.h" "HaGame3D_v1/Utils/Aliases.h" "HaGame3D_v1/Graphics/Routines.h" "HaGame3D_v1/Graphics/Routines.cpp" "HaGame3D_v1/Graphics/Camera.h" "HaGame3D_v1/Graphics/Camera.cpp" "HaGame3D_v1/Core/Transform.h" "HaGame3D_v1/Core/Transform.cpp" "HaGame3D_v1/Graphics/Primitives.h" "HaGame3D_v1/Graphics/Texture.h" "HaGame3D_v1/Vendor/stb/stb_image.cpp" "HaGame3D_v1/Graphics/Texture.cpp" "HaGame3D_v1/Math/Functions.h" "HaGame3D_v1/Input/Devices/KeyboardMouse.h" "HaGame3D_v1/Graphics/Monitors.h"  "HaGame3D_v1/Math/Hypercube.h" "HaGame3D_v1/Graphics/Sprite.h"  "HaGame3D_v1/Graphics/Particle.h" "HaGame3D_v1/Core/ResourceManager.h" "HaGame3D_v1/Core/ResourceManager.cpp" "HaGame3D_v1/Core/Game.h" "HaGame3D_v1/Core/Game.cpp" "HaGame3D_v1/Demos/Breakout.hpp"   "HaGame3D_v1/Graphics/OpenGL.h" "HaGame3D_v1/Graphics/OpenGL.cpp" "HaGame3D_v1/Utils/String.h" "HaGame3D_v1/Utils/File.h" "HaGame3D_v1/Utils/File.cpp" "HaGame3D_v1/Utils/String.cpp" "HaGame3D_v1/Physics/Collisions.h" "HaGame3D_v1/Physics/Collisions.cpp")

set(LIBS ${SDL2_LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBS})

message(STATUS "LIBS: ${LIBS} ")

target_link_libraries(HaGame3D ${LIBS})

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(HaGame3D PRIVATE "/std:c++latest")
    endif()
endif()

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET HaGame3D POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:HaGame3D>)
    endforeach()
endif()