# - Find LibHttpServer (https://github.com/etr/libhttpserver/blob/master/cmakemodule/FindLibHttpServer.cmake)

if(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)
  set(LIBHTTPSERVER_FOUND TRUE)
else()
  find_path(LIBHTTPSERVER_INCLUDE_DIRS httpserverpp
    /usr/include
    /usr/include/httpserver
    /usr/local/include/
    /usr/local/include/httpserver
  )

  find_library(LIBHTTPSERVER_LIBRARIES httpserver
    /usr/lib
    /usr/local/lib
    /opt/local/lib
  )

  if(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)
    set(LIBHTTPSERVER_FOUND TRUE)
  else()
    set(LIBHTTPSERVER_FOUND FALSE)
  endif()

  mark_as_advanced(LIBHTTPSERVER_INCLUDE_DIRS LIBHTTPSERVER_LIBRARIES)
endif()

if(LIBHTTPSERVER_FOUND)
  message(STATUS "Found libhttpserver: ${LIBHTTPSERVER_LIBRARIES}")
else()
  message(FATAL_ERROR "libhttpserver not found. Consider using the build script \
                       with `sudo ../scripts/install_libhttpserver.sh` to install the library on your system")
endif()
