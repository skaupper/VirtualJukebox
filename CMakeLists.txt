cmake_minimum_required(VERSION 3.9)

project(VirtualJukebox)

# needed for Doxygen targets
cmake_policy(SET CMP0054 NEW)

#
# Compiler settings
#
# Build debug version by default, unless specified otherwise
IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

set (CMAKE_CXX_STANDARD 17)

#
# Sourcen for application and tests
#

# Source file containing the main entrypoint
set(ENTRYPOINT_SOURCE   src/main.cpp)
# All source file (excluding main) for the application
set(APP_SOURCES         src/JukeBox.cpp )
# All source files containing test cases
set(TEST_CASE_SOURCES   test/helloworld.cpp)

# Sources separated in application and test sources
set(SOURCES      ${ENTRYPOINT_SOURCE} ${APP_SOURCES})
set(TEST_SOURCES ${APP_SOURCES} ${TEST_CASE_SOURCES})

include(cmake/ClangFormat.cmake)

#
# Documentation sources
#
set(DOC_SOURCES         docs/src/main.md)



# TODO: add compile time dependencies here

#
# Build application
#

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# TODO: add library dependencies here


#
# Build unit tests
#
set(TEST_TARGET test${PROJECT_NAME})

# enable testing and look for gtest installation
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# add test executable and link to gtest libs
add_executable(${TEST_TARGET} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} ${GTEST_BOTH_LIBRARIES} pthread)

# add ctest target, so tests can be executed using `make test`
add_test(${TEST_TARGET} ${TEST_TARGET})


#
# Add Doxygen documentation target
#
find_package(Doxygen REQUIRED)

# TODO: set required doxygen configurations
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/)
doxygen_add_docs(doc ${DOC_SOURCES} ${ENTRYPOINT_SOURCE} ${APP_SOURCES} ${APP_HEADER} ${TEST_CASE_SOURCES})
