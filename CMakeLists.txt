cmake_minimum_required(VERSION 3.10.2)

project(VirtualJukebox)

cmake_policy(SET CMP0054 NEW)

################################################################################
# Compiler settings
################################################################################

# Build debug version by default, unless specified otherwise
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

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
find_package(Doxygen)

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
                        src/SpotifyBackend.cpp
                        src/NetworkAPI.cpp
                        src/RestAPI.cpp
                        src/RestEndpointHandler.cpp)


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
                        src/SpotifyBackend.h
                        src/NetworkAPI.h
                        src/RestAPI.h
                        src/RestEndpointHandler.h)

# Libraries and include directories of dependencies used by the application
set(APP_LIBRARIES       ${LIBHTTPSERVER_LIBRARIES}
                        ${LIBMICROHTTPD_LIBRARIES})
set(APP_INCLUDE_DIRS    src/
                        ${LIBHTTPSERVER_INCLUDE_DIRS}
                        ${LIBMICROHTTPD_INCLUDE_DIRS})

# All source files containing test cases
set(TEST_SOURCES        test/Test_ConfigHandler.cpp)
# Libraries and include directories of dependencies used by the tests
set(TEST_LIBRARIES      ${APP_LIBRARIES}
                        ${GTEST_BOTH_LIBRARIES}
                        pthread)
set(TEST_INCLUDE_DIRS   ${GTEST_INCLUDE_DIRS})


# Specify sources for formatting
set(FORMATTING_SOURCES ${ENTRYPOINT_SOURCE} ${APP_SOURCES} ${APP_HEADER} ${TEST_SOURCES})

# Documentation
#-------------------------------------------------------------------------------
set(DOC_SOURCES         docs/src/main.md
                        docs/src/RESTInterface.md
                        docs/src/ClassDiagram.md)

################################################################################
# Targets
################################################################################

# Compile the sources of the main application into an OBJECT library
#-------------------------------------------------------------------------------
set(APP_OBJECTS app_object_library)
add_library(${APP_OBJECTS} OBJECT ${APP_SOURCES} ${APP_HEADER})


# Main program executable
#-------------------------------------------------------------------------------
include_directories(${APP_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${ENTRYPOINT_SOURCE} $<TARGET_OBJECTS:${APP_OBJECTS}>)
target_link_libraries(${PROJECT_NAME} ${APP_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${SRC_COMPILER_OPTIONS})

# Test program, running unit tests
#-------------------------------------------------------------------------------
set(TEST_TARGET test${PROJECT_NAME})

enable_testing()

# Add test executable and link to required libs
include_directories(${TEST_INCLUDE_DIRS})
add_executable(${TEST_TARGET} ${TEST_SOURCES} $<TARGET_OBJECTS:${APP_OBJECTS}>)
target_link_libraries(${TEST_TARGET} ${TEST_LIBRARIES})

# Add test target, so tests can be executed using `make test`
add_test(${TEST_TARGET} ${TEST_TARGET})

# Other
#-------------------------------------------------------------------------------
clang_format_add_target(${FORMATTING_SOURCES})

################################################################################
# Doxygen
################################################################################
if(DOXYGEN_FOUND)
  message(STATUS "Add Doxygen target")
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/)
  doxygen_add_docs(doc ${DOC_SOURCES} ${ENTRYPOINT_SOURCE} ${APP_SOURCES} ${APP_HEADER} ${TEST_SOURCES})
else()
  message(STATUS "Do not add Doxygen target (Doxygen was not found)")
endif()


################################################################################
# Examples
################################################################################
# Build examples, unless specified otherwise
if(NOT DEFINED BUILD_EXAMPLES)
  set(BUILD_EXAMPLES TRUE)
endif()

if(BUILD_EXAMPLES)
  message(STATUS "Build examples")

  # prepare some variables so the examples can use them easily
  set(EXAMPLE_APP_LIBRARIES ${APP_LIBRARIES})
  set(EXAMPLE_APP_INCLUDE_DIRS ${APP_INCLUDE_DIRS})
  set(EXAMPLE_APP_OBJECTS ${APP_OBJECTS})

  add_subdirectory(examples/)
else()
  message(STATUS "Do not build examples")
endif()
