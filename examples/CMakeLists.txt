################################################################################
# Build examples
################################################################################

# Interesting variables:
# - EXAMPLE_APP_OBJECTS     : object library which contains all app sources (as objects)
# - EXAMPLE_APP_LIBRARIES   : libraries which are used by the app
# - EXAMPLE_APP_INCLUDE_DIRS: include directories used by the app

include_directories(${EXAMPLE_APP_INCLUDE_DIRS})

#
# libhttpserver example
#
add_executable(hello_libhttpserver hello_libhttpserver.cpp $<TARGET_OBJECTS:${EXAMPLE_APP_OBJECTS}>)
target_link_libraries(hello_libhttpserver ${EXAMPLE_APP_LIBRARIES})

#
# empty_network_listener example
#
add_executable(empty_network_listener empty_network_listener.cpp $<TARGET_OBJECTS:${EXAMPLE_APP_OBJECTS}>)
target_link_libraries(empty_network_listener ${EXAMPLE_APP_LIBRARIES})

#
# simple_spotifyClientServer example
#
add_executable(simple_spotifyClientServer simple_spotifyClientServer.cpp $<TARGET_OBJECTS:${EXAMPLE_APP_OBJECTS}>)
target_link_libraries(simple_spotifyClientServer ${EXAMPLE_APP_LIBRARIES})