#!/usr/bin/env bash

set -euo pipefail

echo "This is the travis.sh build script!"

if [ "$JOB_ENV" == "4711" ]; then
  echo "Compiling all targets."
  mkdir -p build;
  cd build;
  cmake ..
  make

  ./VirtualJukebox
  ./testVirtualJukebox
fi
