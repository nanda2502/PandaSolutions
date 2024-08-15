cmake_minimum_required(VERSION 3.28)

project(PandaSolutions)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(PandaSolutions ${SOURCES})