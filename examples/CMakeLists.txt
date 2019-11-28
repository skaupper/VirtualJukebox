################################################################################
# Build examples
################################################################################

# Used variables:
# - EXAMPLE_LIBRARIES: libraries to link against all examples
# - EXAMPLE_INCLUDE_DIRS: additional include directories to use


if(BUILD_EXAMPLES)
  include_directories(${EXAMPLE_INCLUDE_DIRS})

  # libhttpserver example
  add_executable(hello_libhttpserver hello_libhttpserver.cpp)
  target_link_libraries(hello_libhttpserver ${EXAMPLE_LIBRARIES})

  # ...

endif()
