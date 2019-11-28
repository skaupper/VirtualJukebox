# JukeBox

TODO

## Dependencies

- CMake (minimum version 3.9)
- Google Test
- Doxygen
- clang-format-6.0

### Bundled dependencies

Some dependencies are bundled with this repository as submodules to allow a less painful installation process.
For these dependencies there are scripts provided which clone, build and install the libraries for your system.
Note that these scripts require root privileges in order to install the library files and the corresponding headers
to your system directories!

Bundled dependencies are:

- libmicrohttpd
- libhttpserver

## Installation

1. Create a build directory: `mkdir build`
2. Change to the build directory and invoke CMake: `cd build; cmake ..`
3. A Makefile was created. Use `make` to compile the application and the tests
4. The application can be started using `./VirtualJukebox`
5. The tests can be executed using:
    - `./testVirtualJukebox`: invokes the program directly
    - `make test`: using the CTest integration of CMake
6. To create the documentation, use `make doc`
