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
  valgrind --leak-check=full --show-leak-kinds=all ./testVirtualJukebox
fi

if [ "$JOB_ENV" == "linting" ]; then
  echo "### Checking source code linting."
  ./scripts/clang_format.sh
fi
