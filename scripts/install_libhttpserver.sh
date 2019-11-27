#!/usr/bin/env bash
set -euo pipefail
git submodule update --init --recursive

# change working directory
SCRIPT_PATH=$(dirname ${BASH_SOURCE})
cd $SCRIPT_PATH

# go to library directory and build library
cd ../lib/libhttpserver/
./bootstrap
mkdir -p build
cd build
../configure
make

# install library files
make install
