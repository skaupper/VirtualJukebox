cmake_minimum_required(VERSION 3.9)

project(VirtualJukebox)

cmake_policy(SET CMP0054 NEW)

################################################################################
# Compiler settings
################################################################################

# Build debug version by default, unless specified otherwise
IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(SRC_COMPILER_OPTIONS
        -Wall
        -Wextra)

################################################################################
# Include other CMake files
################################################################################

include(cmake/ClangFormat.cmake)
include(cmake/LibMicroHttpd.cmake)
include(cmake/LibHttpServer.cmake)

################################################################################
# Find packages with built-in support
################################################################################

find_package(GTest REQUIRED)
find_package(Doxygen REQUIRED)

################################################################################
# Source files
################################################################################

# Application and tests
#-------------------------------------------------------------------------------

# Source file containing the main entrypoint
set(ENTRYPOINT_SOURCE   src/main.cpp)
# All source file (excluding main) for the application
set(APP_SOURCES         src/JukeBox.cpp
                        src/Track.cpp
                        src/Queue.cpp
                        src/LoggingHandler.cpp
                        src/ConfigHandler.cpp
                        src/PlaybackTrack.cpp
                        src/SpotifyBackend.cpp)


set(APP_HEADER          src/JukeBox.h
                        src/Track.h
                        src/Queue.h
                        src/LoggingHandler.h
                        src/ConfigHandler.h
                        src/NetworkListener.h
                        src/Result.h
                        src/GlobalTypes.h
                        src/MusicBackend.h
                        src/PlaybackTrack.h
                        src/SpotifyBackend.h)

# Libraries and include directories of dependencies used by the application
set(APP_LIBRARIES       ${LIBHTTPSERVER_LIBRARIES}
                        ${LIBMICROHTTPD_LIBRARIES})
set(APP_INCLUDE_DIRS    ${LIBHTTPSERVER_INCLUDE_DIRS}
                        ${LIBMICROHTTPD_INCLUDE_DIRS})

# All source files containing test cases
set(TEST_CASE_SOURCES   test/helloworld.cpp)
# Libraries and include directories of dependencies used by the tests
set(TEST_LIBRARIES      ${GTEST_BOTH_LIBRARIES}
                        pthread)
set(TEST_INCLUDE_DIRS   ${GTEST_INCLUDE_DIRS})

# All source files containing test cases
set(TEST_CASE_SOURCES   test/Test_ConfigHandler.cpp)

# Sources separated in application and test sources
set(SOURCES      ${ENTRYPOINT_SOURCE} ${APP_SOURCES})
set(TEST_SOURCES ${APP_SOURCES} ${TEST_CASE_SOURCES})

# Documentation
#-------------------------------------------------------------------------------
set(DOC_SOURCES         docs/src/main.md
                        docs/src/RESTInterface.md
                        docs/src/ClassDiagram.md)

################################################################################
# Targets
################################################################################

# Main program executable
#-------------------------------------------------------------------------------
include_directories(${APP_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${APP_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${SRC_COMPILER_OPTIONS})

# Test program, running unit tests
#-------------------------------------------------------------------------------
set(TEST_TARGET test${PROJECT_NAME})

enable_testing()

# Add test executable and link to required libs
include_directories(${TEST_INCLUDE_DIRS})
add_executable(${TEST_TARGET} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} ${TEST_LIBRARIES})

# Add test target, so tests can be executed using `make test`
add_test(${TEST_TARGET} ${TEST_TARGET})

# Other
#-------------------------------------------------------------------------------

clang_format_add_target()

################################################################################
# Doxygen
################################################################################

# TODO: set required doxygen configurations
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/)
doxygen_add_docs(doc ${DOC_SOURCES} ${ENTRYPOINT_SOURCE} ${APP_SOURCES} ${APP_HEADER} ${TEST_CASE_SOURCES})


################################################################################
# Examples
################################################################################
# Build examples, unless specified otherwise
if(NOT BUILD_EXAMPLES)
  add_subdirectory(examples/)
endif()
