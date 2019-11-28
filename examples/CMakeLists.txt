################################################################################
# Build examples
################################################################################

# Interesting variables:
# - EXAMPLE_APP_SOURCES     : source files of the app using absolute paths
# - EXAMPLE_APP_LIBRARIES   : libraries which are used by the app
# - EXAMPLE_APP_INCLUDE_DIRS: include directories used by the app

include_directories(${EXAMPLE_APP_INCLUDE_DIRS})

# libhttpserver example
add_executable(hello_libhttpserver hello_libhttpserver.cpp $<TARGET_OBJECTS:${EXAMPLE_APP_OBJECTS}>)
target_link_libraries(hello_libhttpserver ${EXAMPLE_APP_LIBRARIES})

# empty_network_listener example
add_executable(empty_network_listener empty_network_listener.cpp $<TARGET_OBJECTS:${EXAMPLE_APP_OBJECTS}>)
target_link_libraries(empty_network_listener ${EXAMPLE_APP_LIBRARIES})

# ...
