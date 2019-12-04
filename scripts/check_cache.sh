#!/usr/bin/env bash

echo "### Check library cache."

if [[ "`ls -A cached_libs/`" ]]; then
    echo "Cache found, do not build libraries."
    sudo cp -r cached_libs/* /usr/local/;
else
    echo "No cache found, build libraries.";
fi

if [[ -f /usr/local/lib/libhttpserver.so ]]; then
    echo "### Build libhttpserver."
    ./scripts/install_libhttpserver.sh;
fi

if [[ -f /usr/local/lib/librestclient-cpp.so ]]; then
    echo "### Build librestclient-cpp."
    ./scripts/install_librestclient-cpp.sh;
fi
