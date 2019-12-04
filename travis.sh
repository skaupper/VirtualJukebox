#!/usr/bin/env bash

set -euo pipefail

if [ "$JOB_ENV" == "BuildAndTests" ]; then
  echo "### Compiling all targets."
  mkdir -p build;
  cd build;
  cmake ..
  make

  echo "### Running test."
  ./testVirtualJukebox
fi
