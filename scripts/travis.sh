#!/usr/bin/env bash

set -euo pipefail

if [ "$JOB_ENV" == "unittest" ]; then
  echo "### Compiling all targets."
  mkdir -p build;
  cd build;
  cmake ..
  make

  echo "### Running test."
  ./testVirtualJukebox
fi

if [ "$JOB_ENV" == "valgrind" ]; then
  echo "### Compiling all targets."
  mkdir -p build;
  cd build;
  cmake ..
  make

  echo "### Running valgrind on test executable."
  valgrind ./testVirtualJukebox
fi
