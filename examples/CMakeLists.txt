################################################################################
# Build examples
################################################################################

# Interesting variables:
# - APP_LIBRARIES: libraries which are by the app (examples normally do not have further libs)
# - APP_INCLUDE_DIRS: include directories used by the app

include_directories(${APP_INCLUDE_DIRS})

# libhttpserver example
add_executable(hello_libhttpserver hello_libhttpserver.cpp)
target_link_libraries(hello_libhttpserver ${APP_LIBRARIES})

# ...
