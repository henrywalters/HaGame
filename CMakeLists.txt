# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0074 NEW)

project (HaGame)
project (HaGameEditor C CXX)

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

add_executable(HaGame HaGame/HaGame.cpp "HaGame/Graphics/Color.h"  "HaGame/Input/Device.h" "HaGame/Input/Devices/Gamepad.h" "HaGame/Graphics/Window.cpp" "HaGame/Graphics/Shader.cpp" "HaGame/Utils/FileSystem.cpp" "HaGame/Graphics/ShaderProgram.cpp" "HaGame/Utils/Clock.h" "HaGame/Graphics/Mesh.h" "HaGame/Utils/Aliases.h" "HaGame/Graphics/Routines.h" "HaGame/Graphics/Routines.cpp" "HaGame/Graphics/Camera.h"  "HaGame/Core/Transform.h" "HaGame/Core/Transform.cpp" "HaGame/Graphics/Primitives.h" "HaGame/Graphics/Texture.h" "HaGame/Vendor/stb/stb_image.cpp" "HaGame/Graphics/Texture.cpp" "HaGame/Math/Functions.h" "HaGame/Input/Devices/KeyboardMouse.h" "HaGame/Graphics/Monitors.h"  "HaGame/Math/Hypercube.h" "HaGame/Graphics/Sprite.h"  "HaGame/Graphics/Particle.h" "HaGame/Core/ResourceManager.h" "HaGame/Core/ResourceManager.cpp" "HaGame/Core/Game.h" "HaGame/Core/Game.cpp" "HaGame/Demos/Breakout.hpp"   "HaGame/Graphics/OpenGL.h" "HaGame/Graphics/OpenGL.cpp" "HaGame/Utils/String.h" "HaGame/Utils/File.h" "HaGame/Utils/File.cpp" "HaGame/Utils/String.cpp" "HaGame/Physics/Collisions.h" "HaGame/Physics/Collisions.cpp" "HaGame/Demos/ECS.hpp" "HaGame/Utils/ObjectManager.h" "HaGame/Utils/Reflection.h" "HaGame/Core/ECS/Entity.h" "HaGame/Core/ECS/Component.h" "HaGame/Core/ECS/System.h" "HaGame/Utils/Logger.h" "HaGame/Utils/Loggers/ConsoleLogger.h" "HaGame/Utils/Loggers/FileLogger.h" "HaGame/Math/Quaternion.h" "HaGame/Math/Matrix.h"  "HaGame/Graphics/Material.h" "HaGame/Graphics/Line.h" "HaGame/Physics/RigidBody.h" "HaGame/Vendor/entt/entt.hpp" "HaGame/Utils/StateManager.h" "HaGame/Core/Scene.h" "HaGame/Core/ECS/EntityManager.h" "HaGame/Input/InputManager.h" "HaGame/Graphics/PerspectiveCamera.hpp" "HaGame/Core/ECS/ECS.h" "HaGame/Core/ECS/SystemManager.h")

set(LIBS ${SDL2_LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBS} )
message(STATUS "LIBS: ${LIBS} ")

target_link_libraries(HaGame ${LIBS})

add_executable(HaGameEditor Editor/Editor.cpp "Editor/Editor.h" "Editor/Systems/CameraController.h" "Editor/Systems/Renderer.h" "Editor/Systems/MousePicker.h" "Editor/Systems/UI.h")
target_link_libraries(HaGameEditor ${LIBS})

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(HaGame PRIVATE "/std:c++latest")
        target_compile_options(HaGameEditor PRIVATE "/std:c++latest")
    endif()
endif()

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET HaGame POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:HaGame>)
    endforeach()
endif()