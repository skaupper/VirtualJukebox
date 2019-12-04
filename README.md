# JukeBox

[![Build Status][1]]([2])

## Dependencies

- CMake (minimum version 3.9)
- Google Test
- Doxygen
- clang-format-6.0
- curl

### Bundled dependencies

Some dependencies are bundled with this repository as submodules to allow a less painful installation process.
For these dependencies there are scripts provided which clone, build and install the libraries for your system.
Note that these scripts require root privileges in order to install the library files and the corresponding headers
to your system directories!

Bundled dependencies are:

- libmicrohttpd
- libhttpserver
- librestclient-cpp

### Installation of dependencies on Ubuntu 18.04

Since installing the dependencies is not always straight-forward, the following commands can be used to install all
listed dependencies.

- `sudo apt-get install build-essential cmake doxygen clang-format-6.0`
- `sudo apt-get install libmicrohttpd-dev libcurl4-gnutls-dev libgtest-dev`
- `sudo apt-get install automake libtool`
- `./scripts/install_libhttpserver.sh`
- `./scripts/install_restclient-cpp.sh`
- `cd /usr/src/googletest; sudo cmake .; sudo make install`

## Installation

1. Create a build directory: `mkdir build`
2. Change to the build directory and invoke CMake: `cd build; cmake ..`
3. A Makefile was created. Use `make` to compile the application and the tests
4. The application can be started using `./VirtualJukebox`
5. The tests can be executed using:
    - `./testVirtualJukebox`: invokes the program directly
    - `make test`: using the CTest integration of CMake
6. To create the documentation, use `make doc`

[1]: https://travis-ci.com/skaupper/VirtualJukebox.svg?branch=master
[2]: https://travis-ci.com/skaupper/VirtualJukebox
