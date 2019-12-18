# VirtualJukebox

Changed this line to open an example PR for the presentation.

[![Build Status][1]][2]

## Introduction

This project is developed as part of an university lecture regarding software projects at the University of Applied Sciences Hagenberg, in the program Embedded Systems Design.

The main purpose of __VirtualJukebox__ is to provide a server application which manages a music playlist shared by
multiple clients. The clients are capable of adding tracks to a user playlist and voting on tracks to play next. The track
with the highest vote count is played next.
Additionally, a client can authenticate as an administrator who is then capable of adding tracks to an admin playlist
whose tracks are always played before any track from the user playlist. Besides that, the administrator has some further
capabilities like controlling the playback behaviour of the music player. This includes
pausing, resuming, stopping, skipping the current track as well as adjusting the playback volume.

While tracks in the admin queue are always played in order of insertion (FIFO principle), the tracks in the user queue can be reordered using the voting system. Each user has the ability to vote for tracks in the user queue. The top most track (i.e. the track which gets played next) is the one with the most votes.

This repository contains the sources for the server as well as instructions on how to install and test and use it.

## Dependencies

- CMake (minimum version 3.9)
- Google Test
- Google Log
- Doxygen
- clang-format-6.0
- libcurl-dev
- libmicrohttpd-dev
- libhttpserver-dev
- librestclient-cpp-dev

Check your Linux distributions' package manager if there are proper packages available. For some of the dependencies there are
usually no packages available, so they got bundled in this repository including proper install scripts.

### Bundled dependencies

Some dependencies are bundled with this repository as submodules to allow a less painful installation process.
Scripts are provided which clone, build and install the libraries for your system. Note that these scripts require root privileges in order to install the library files and the corresponding headers
to your system directories (the final step gets executed using `sudo`)!

NOTE: Install dependencies in below listed order.

Bundled dependencies are:

- libhttpserver
- librestclient-cpp

### Example installation of dependencies on Ubuntu 18.04

Since installing the dependencies is not always straight-forward, the following commands can be used to install all
listed dependencies, that are required to install manually.

- `sudo apt-get install build-essential cmake doxygen clang-format-6.0`
- `sudo apt-get install libmicrohttpd-dev libcurl4-gnutls-dev libgoogle-glog-dev`
- `sudo apt-get install automake libtool`
- `./scripts/install_libhttpserver.sh`
- `./scripts/install_librestclient-cpp.sh`

## Compile and test

1. Create a build directory: `mkdir build`
2. Change to the build directory and invoke CMake: `cd build; cmake ..`
3. A Makefile was created. Use `make` to compile the application and the tests
4. The application can be started using `./VirtualJukebox`
    - To use the verbose logging feature (debug logs) you can set the environment variable `GLOG_v` to the desired verbosity level. Since these levels are not specified you have to dig through the source code for calls to `VLOG(...)` to get the desired level.
5. The tests can be executed using:
    - `./testVirtualJukebox`: invokes the program directly
    - `make test`: using the CTest integration of CMake
6. To create the documentation, use `make doc`

## Examples

For testing and showcasing purposes this repository also contains some examples. Each of these examples do not use
the whole VirtualJukebox server but a subcomponent. This can be useful for debugging purposes or if you want to replace
a subcomponent with a custom one to show how the submodule. In the latter case the examples show how a component has to
communicate with its environment (see also the test in `test/`).

## Spotify Setup

In order to use this server properly, one needs to setup Spotify correctly. This requires a Spotify Premium account.

This section describes this setup process step by step.

1. **Create a Spotify application**\
Log into your Spotify account on the [Spotify Dashboard](https://developer.spotify.com/dashboard/login).

2. **Create an application and receive client ID and client secret**\
After you have created the application, the client ID and client secret will be shown. Replace the respective key values in the file `jukebox_config.ini` .

3. **Whitelist a redirect URI**\
The authorization process redirects to a specific URI that is set in the configuration file. This URI must be whitelisted in Spotify.\
\
The URI in the configuration file needs to be adapted to the IP address of the machine the VirtualJukeBox server program is running on. Of course, `localhost` can be used as well. The port number can be adapted too, of course.\
\
To whitelist the URI, open the created application from the dashboard and click 'Edit Settings'. Paste the URI from the configuration file into the respective field 'Redirect URIs' in the pop-up window and click 'Add'.


## How to use

If all software dependencies are installed and the Spotify dashboard application has been created, you can run the server. The application takes the path to the configuration file as first (and only) parameter. It falls back to `../jukebox_config.ini` if none is given.

### Authorization

This needs to be done on every server program start.

Before users can do any interactions with the server you have to authorize against Spotify and receive an access token.

To do this the server provides a HTTP endpoint which can be reached at the URL `http://<server_ip>:<port>/spotifyLogin`. You need to replace the `server_ip` with the IP address of the server machine. The required value for `port` can be found in the configuration file in section `[Spotify]` with the key `port`.

Copy & paste this URL into your favourite browser and open it.

If you used the correct URL you get redirected to the Spotify authorization site where you need to log in using your Spotify account. This is only required the first time. Following attempts will only show an empty page.

After authorizing the application you get redirected to the URL specified in the configuration file in section `[Spotify]` key `redirectUri`. Make sure that this URI was [whitelisted on the Spotify dashboard](#spotify-setup).

If everything was successful the server should print `Access token acquired successfully` to the terminal.

### Using the client endpoints

If an access token has already been acquired the clients can start using the REST interface described [here](./docs/html/rest_interface.html).

Since REST is a stateless protocol the first request has to be to the endpoint `/generateSession`, which generates a session ID the client can use to pass to other endpoints. Using this endpoint you can also authenticate yourself as the server administator.

An example for a usual interaction with the server could be:

1. Generating a user session (`/generateSession`).
2. Listing available tracks (`/queryTracks`).
3. Adding a track to the user queue (`/addTrackToQueue`).
4. Voting for other tracks, so they will get played earlier (`/voteTrack`)).
5. Revoking a vote again because you decided otherwise (also `/voteTrack`).

Administrators may have different interactions (additionally to the ones of a normal user):

1. Generating an admin session (also `/generateSession`).
2. Removing some tracks from any queue (they may be inappropriate or simply not wanted) (`/removeTrack`).
3. Adding tracks to the admin queue which get always played before any track of the normal queue (`/addTrackToQueue`).
4. Control the behaviour of the player (i.e. pause/resume/skip/stop the playback of the current track or adjusting the volume) (`/controlPlayer`).

## Troubleshooting

### _No devices for playing the track available_

This happens if no device, linked to the account which was used to get the access token, was found.
To solve it, start Spotify on any device with that specific account.\
\
_**NOTE:** Missing an instruction step above, or anything is unclear in the documentation? Feel free to open an issue ticket [here](https://github.com/skaupper/VirtualJukebox/issues)._


[1]: https://img.shields.io/travis/com/skaupper/virtualjukebox/master?label=Travis%20Build%20Status&logo=travis
[2]: https://travis-ci.com/skaupper/VirtualJukebox
