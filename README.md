# JukeBox

TODO

## Dependencies

- CMake (minimum version 3.9)
- Google Test
- Doxygen

## Installation

1. Create a build directory: `mkdir build`
2. Change to the build directory and invoke CMake: `cd build; cmake ..`
3. A Makefile was created. Use `make` to compile the application and the tests
4. The application can be started using `./JukeBox`
5. The tests can be executed using:
    - `./testJukeBox`: invokes the program directly
    - `make test`: using the CTest integration of CMake
6. To create the documentation, use `make doc`
