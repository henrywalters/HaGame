# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.1)
cmake_policy(SET CMP0074 NEW)

project (HaGame3D_v1)

message("CMAKE_MODULE_PATH = ${CMAKE_SOURCE_DIR}/modules")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
 
find_package(SDL2 REQUIRED)

message("SDL2_DIR = ${SDL2_INCLUDE_DIRS}")
message("SDL2_LIB = ${SDL2_LIBS}")

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(HaGame3D_v1 HaGame3D_v1/HaGame3D_v1.cpp)
target_link_libraries(HaGame3D_v1 ${SDL2_LIBS})    

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET HaGame3D_v1 POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:HaGame3D_v1>)
    endforeach()
endif()