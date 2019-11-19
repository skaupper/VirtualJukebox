cmake_minimum_required(VERSION 3.0)

project(JukeBox)

set(SRC_DIR src/)
file(GLOB SOURCES ${SRC_DIR}/*.cpp)

# add the executable
add_executable(JukeBox ${SOURCES})
