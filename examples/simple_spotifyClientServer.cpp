/**
 * @file    simple_spotifyClientServer.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Minimal example using the spotify api
 */

#include <iostream>

#include "SpotifyApi/SpotifyAPITypes.h"
#include "SpotifyApi/SpotifyAuthorization.h"

using namespace std;

int main(void) {
  SpotifyApi::SpotifyAuthorization auth;
  auth.startServer();

  return 0;
}