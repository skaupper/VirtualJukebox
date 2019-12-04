# VirtualJukebox

[![Build Status][1]]([2])

## Introduction

This project is developed as part of an university lecture regarding software projects.

The main purpose of __VirtualJukebox__ is to provide a server application which manages a music playlist shared by
multiple client. The clients are capable of adding tracks to the user playlist and voting on tracks to play next. The track
with the highest vote count is played next.
Additionally, a client can authenticate as administrator who is then capable of adding tracks to the admin playlist
whose tracks are always played before any track from the user playlist. Besides that, the administrator has some further
capabilities like controlling the playback behaviour of the music player which includes
pausing/resuming/stopping/skipping the current track as well as adjusting the playback volume.

This repository contains the sources for the server as well as instructions on how to install and test it.

## Dependencies

- CMake (minimum version 3.9)
- Google Test
- Doxygen
- clang-format-6.0
- libcurl-dev
- libmicrohttpd-dev
- libhttpserver-dev
- librestclient-cpp-dev

Check your distros package manager if there are proper packages available. For some of the dependencies there are
usually no packages available, so they got bundled in this repository including proper install scripts.

### Bundled dependencies

Some dependencies are bundled with this repository as submodules to allow a less painful installation process.
For these dependencies there are scripts provided which clone, build and install the libraries for your system.
Note that these scripts require root privileges in order to install the library files and the corresponding headers
to your system directories (the final step gets executed using `sudo`)!

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

## Examples

For testing and showcasing purposes this repository also contains some examples. Each of these examples do not use
the whole VirtualJukebox server but a subcomponent. This can be useful for debugging purposes or if you want to replace
a subcomponent with a custom one to show how the submodule. In the latter case the examples show how a component has to
communicate with its environment (see also the test in `test/`).

## How to use

TODO

[1]: https://travis-ci.com/skaupper/VirtualJukebox.svg?branch=master
[2]: https://travis-ci.com/skaupper/VirtualJukebox
