#!/usr/bin/env bash
set -euo pipefail
git submodule update --init --recursive

# change working directory
SCRIPT_PATH=$(dirname ${BASH_SOURCE})
cd $SCRIPT_PATH

# go to library directory and build library
cd ../lib/restclient-cpp/
./autogen.sh
./configure
mkdir build
cd build
cmake ..
make

# install library files (needs root privileges)
sudo make install
