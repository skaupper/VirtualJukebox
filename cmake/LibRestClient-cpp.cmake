# - Find libmicrohttpd

if(LIBRESTCLIENT_INCLUDE_DIRS AND LIBRESTCLIENT_LIBRARIES)
  set(LIBRESTCLIENT_FOUND TRUE)
else()
  find_path(LIBRESTCLIENT_INCLUDE_DIRS restclient.h
    /usr/include
    /usr/include/restclient-cpp
    /usr/local/include/
    /usr/local/include/restclient-cpp
  )

  find_library(LIBRESTCLIENT_LIBRARIES restclient-cpp
    /usr/lib
    /usr/local/lib
    /opt/local/lib
  )

  if(LIBRESTCLIENT_INCLUDE_DIRS AND LIBRESTCLIENT_LIBRARIES)
    set(LIBRESTCLIENT_FOUND TRUE)
    message(STATUS "Found librestclient-cpp: ${LIBRESTCLIENT_LIBRARIES}")
  else()
    set(LIBRESTCLIENT_FOUND FALSE)
  endif()

  mark_as_advanced(LIBRESTCLIENT_INCLUDE_DIRS LIBRESTCLIENT_LIBRARIES)
endif()

if(NOT LIBRESTCLIENT_FOUND)
  message(FATAL_ERROR "librestclient not found. Consider using the build script \
                       with ` ../scripts/install_librestclient-cpp.sh` to install the library on your system")
endif()
