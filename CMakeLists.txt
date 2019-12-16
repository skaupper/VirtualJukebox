cmake_minimum_required(VERSION 3.10.2)

project(VirtualJukebox)

cmake_policy(SET CMP0054 NEW)

################################################################################
# Set default values for input parameter
################################################################################

# Build examples, unless specified otherwise
if(NOT DEFINED BUILD_EXAMPLES)
  set(BUILD_EXAMPLES TRUE)
endif()

# Build debug version by default, unless specified otherwise
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

################################################################################
# Compiler settings
################################################################################

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
include(cmake/LibRestClient-cpp.cmake)
include(cmake/GoogleTest.cmake)

################################################################################
# Find packages with built-in support
################################################################################

find_package(Doxygen)
find_package(Threads)
include(cmake/FindGlog.cmake)

################################################################################
# Source files
################################################################################

# Application and tests
#-------------------------------------------------------------------------------

# Source file containing the main entrypoint
set(ENTRYPOINT_SOURCE   src/main.cpp)
# All source file (excluding main) for the application
set(APP_SOURCES         src/JukeBox.cpp
                        src/Types/Tracks.cpp
                        src/Types/Queue.cpp
                        src/Utils/LoggingHandler.cpp
                        src/Utils/ConfigHandler.cpp
                        src/Utils/Serializer.cpp
                        src/Utils/SimpleScheduler.cpp
                        src/Spotify/SpotifyBackend.cpp
                        src/Spotify/SpotifyAPITypes.cpp
                        src/Spotify/SpotifyAPI.cpp
                        src/Spotify/SpotifyAuthorization.cpp
                        src/Network/NetworkAPI.cpp
                        src/Network/RestAPI.cpp
                        src/Network/RestRequestHandler.cpp
                        src/Network/RestEndpointHandlers.cpp
                        src/Datastore/RAMDataStore.cpp)

set(APP_HEADER          src/JukeBox.h
                        src/MusicBackend.h
                        src/NetworkListener.h
                        src/Types/Tracks.h
                        src/Types/Queue.h
                        src/Types/Result.h
                        src/Types/GlobalTypes.h
                        src/Utils/LoggingHandler.h
                        src/Utils/ConfigHandler.h
                        src/Utils/Serializer.h
                        src/Utils/SimpleScheduler.h
                        src/Spotify/SpotifyBackend.h
                        src/Spotify/SpotifyAPITypes.h
                        src/Spotify/SpotifyAPI.h
                        src/Spotify/SpotifyAuthorization.h
                        src/Network/NetworkAPI.h
                        src/Network/RestAPI.h
                        src/Network/RestRequestHandler.h
                        src/Network/RestEndpointHandlers.h
                        src/Network/RequestInformation.h
                        src/Datastore/DataStore.h
                        src/Datastore/RAMDataStore.h)

# Libraries and include directories of dependencies used by the application
set(APP_LIBRARIES       ${LIBHTTPSERVER_LIBRARIES}
                        ${LIBMICROHTTPD_LIBRARIES}
                        ${LIBRESTCLIENT_LIBRARIES}
                        ${CMAKE_THREAD_LIBS_INIT}
                        ${GLOG_LIBRARY})
set(APP_INCLUDE_DIRS    src/
                        lib/
                        ${LIBHTTPSERVER_INCLUDE_DIRS}
                        ${LIBMICROHTTPD_INCLUDE_DIRS}
                        ${LIBRESTCLIENT_INCLUDE_DIRS}
                        ${GLOG_INCLUDE_DIRS})

# All source files containing test cases
set(TEST_SOURCES        test/Test_ConfigHandler.cpp
                        test/Test_DataStore.cpp
                        test/Test_SpotifyAPI.cpp
                        test/Test_RestAPI.cpp
                        test/fixtures/RestAPIFixture.cpp
                        test/mocks/MockNetworkListener.cpp
                        test/helpers/NetworkListenerHelper.cpp
                        test/helpers/TrackGenerator.cpp
                        test/Test_JukeBox.cpp)

set(TEST_HEADER         test/fixtures/RestAPIFixture.h
                        test/mocks/MockNetworkListener.h
                        test/helpers/NetworkListenerHelper.h)

# Libraries and include directories of dependencies used by the tests
set(TEST_LIBRARIES      ${APP_LIBRARIES}
                        gtest)

set(TEST_INCLUDE_DIRS   test/
                        test/fixtures/
                        test/mocks/
                        test/helpers/)


# Specify sources for formatting
set(FORMATTING_SOURCES ${ENTRYPOINT_SOURCE} ${APP_SOURCES} ${APP_HEADER} ${TEST_SOURCES} ${TEST_HEADER})

if(BUILD_EXAMPLES)
  file(GLOB EXAMPLE_FORMATTING_SOURCES examples/*.cpp examples/*.h)
  set(FORMATTING_SOURCES ${FORMATTING_SOURCES} ${EXAMPLE_FORMATTING_SOURCES})
endif()

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
target_compile_options(${APP_OBJECTS} PRIVATE ${SRC_COMPILER_OPTIONS})


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
target_compile_options(${TEST_TARGET} PRIVATE ${SRC_COMPILER_OPTIONS})

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
