# - Find libmicrohttpd

if(LIBMICROHTTPD_INCLUDE_DIRS AND LIBMICROHTTPD_LIBRARIES)
  set(LIBMICROHTTPD_FOUND TRUE)
else()
  find_path(LIBMICROHTTPD_INCLUDE_DIRS microhttpd.h
    /usr/include
    /usr/include/microhttpd
    /usr/local/include/
    /usr/local/include/microhttpd
  )

  find_library(LIBMICROHTTPD_LIBRARIES microhttpd
    /usr/lib
    /usr/local/lib
    /opt/local/lib
  )

  if(LIBMICROHTTPD_INCLUDE_DIRS AND LIBMICROHTTPD_LIBRARIES)
    set(LIBMICROHTTPD_FOUND TRUE)
    message(STATUS "Found libmicrohttpd: ${LIBMICROHTTPD_LIBRARIES}")
  else()
    set(LIBMICROHTTPD_FOUND FALSE)
  endif()

  mark_as_advanced(LIBMICROHTTPD_INCLUDE_DIRS LIBMICROHTTPD_LIBRARIES)
endif()

if(NOT LIBMICROHTTPD_FOUND)
  message(FATAL_ERROR "libmicrohttpd not found. Consider using the build script \
                       with `sudo ../scripts/install_libmicrohttpd.sh` to install the library on your system")
endif()
