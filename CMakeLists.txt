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
# find_package(SDL2_mixer REQUIRED)

find_package(OpenGL REQUIRED)

message(STATUS "OpenGL_FOUND:= ${OpenGL_FOUND}")
 
# find_package(GLM REQUIRED)
 
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/glew-2.1.0)

find_package(GLEW REQUIRED)

#find_package(GLUT REQUIRED)

message(STATUS "OpenGL LIBS: ${OPENGL_INCLUDE_DIR}")  
message(STATUS "GLEW_LIBS: ${GLEW_LIBRARIES}")
message(STATUS "GLUT_LIBS: ${GLUT_INCLUDE_DIRS}")
message(STATUS "SDL2_DIR = ${SDL2_INCLUDE_DIRS}")
message(STATUS "GLM_INCLUDE_DIR = ${GLM_INCLUDE_DIRS}")
message(STATUS "SDL2_LIB = ${SDL2_LIB}")

message(STATUS "GLEW_FOUND:= ${GLEW_FOUND}")

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIRS})


set(LIBS ${SDL2_LIBS} ${SDL2_MIXER_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

add_executable(HaGame HaGame/HaGame.cpp)

target_link_libraries(HaGame ${LIBS})

message(STATUS "LIBS: ${LIBS} ")

add_executable(HaGameEditor Editor/Editor.cpp "Editor/Editor.h" "Editor/Systems/CameraController.h" "Editor/Systems/Renderer.h" "Editor/Systems/MousePicker.h" "Editor/Systems/UI.h")
target_link_libraries(HaGameEditor ${LIBS})

add_executable(Demos Demos/Demo.cpp "Demos/Demo.cpp" "Demos/FPS/FPS.hpp" "Demos/Demo.h" "Demos/MiniJam/OneRoom.hpp" "Demos/MiniJam/ResourceSystem.h" "Demos/MiniJam/DrawingSystem.h" "Demos/MiniJam/MovementSystem.h" "Demos/MiniJam/ViewportSystem.h" "Demos/MiniJam/Pathfinding.hpp" "Demos/SpriteSheetHelper/SpriteSheetHelper.h")
target_link_libraries(Demos ${LIBS})

add_executable(Goom2 Goom2/Goom2.cpp "Goom2/Goom2.cpp" "Goom2/Goom2.h"   "Goom2/Systems/ResourceSystem.h" "Goom2/Scenes/MainMenu.h" "Goom2/Systems/PlayerSystem.h" "Goom2/Components/PlayerController.h" "Goom2/Scenes/MapBuilder.h" "Goom2/Components/PlayerRenderer.h" "Goom2/Components/Bullet.h"  "Goom2/Components/Weapon.h" "Goom2/Components/Inventory.h" "Goom2/Components/Projectile.h" "Goom2/Systems/WeaponSystem.h" "Goom2/Components/Target.h" "Goom2/Systems/EnemySystem.h" "Goom2/Components/Health.h" "Goom2/Components/Enemy.h" "Goom2/Components/Enemies/Turret.h" "Goom2/Components/Actor.h" "Goom2/Systems/ActorSystem.h")
target_link_libraries(Goom2 ${LIBS})

add_executable(ImSim ImSim/ImSim.cpp)
target_link_libraries(ImSim ${LIBS})

target_compile_options(ImSim PRIVATE "/bigobj")

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(HaGame PRIVATE "/std:c++17")
        target_compile_options(HaGameEditor PRIVATE "/std:c++17")
        target_compile_options(Demos PRIVATE "/std:c++17")
        target_compile_options(Goom2 PRIVATE "/std:c++17")
        target_compile_options(ImSim PRIVATE "/std:c++17")
    endif()
else()
    set_property(TARGET HaGame PROPERTY CXX_STANDARD 17)
    set_property(TARGET HaGameEditor PROPERTY CXX_STANDARD 17)
    set_property(TARGET Demos PROPERTY CXX_STANDARD 17)
    set_property(TARGET Goom2 PROPERTY CXX_STANDARD 17)
    set_property(TARGET ImSim PROPERTY CXX_STANDARD 17)
endif() 

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET HaGame POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:HaGame>)
    endforeach()
endif()