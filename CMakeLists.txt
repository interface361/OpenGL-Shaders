cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(CMAKE_BUILD_TYPE Release)

# add extra include directories
include_directories(/usr/local/include)
include_directories(Dependencies/glad/glad.h)
# add extra lib directories from the library folder
link_directories(/usr/local/lib)

project(Output)
add_executable (Output Source/main.cpp Dependencies/src/glad.c)

#find_package(glfw3 3.3 REQUIRED)
target_link_libraries(Output glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(Output OpenGL::GL)