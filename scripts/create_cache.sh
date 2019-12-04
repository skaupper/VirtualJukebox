#!/usr/bin/env bash

echo "### Create library cache."

# create cache dir
mkdir -p cached_libs/lib/ cached_libs/include/

# copy all necessary files to the cache
cp    /usr/local/lib/librestclient-cpp.so cached_libs/lib/
cp    /usr/local/lib/libhttpserver.so     cached_libs/lib/
cp -r /usr/local/include/restclient-cpp/  cached_libs/include/
cp -r /usr/local/include/httpserver/      cached_libs/include/
cp    /usr/local/include/httpserverpp     cached_libs/include/
cp    /usr/local/include/httpserver.hpp   cached_libs/include/

find cached_libs
