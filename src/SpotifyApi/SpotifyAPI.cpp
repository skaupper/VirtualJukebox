/**
 * @file    SpotifyAPI.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAPI definition
 */

#include "SpotifyAPI.h"

using namespace SpotifyApi;

TResult<Token> SpotifyAPI::getAccessToken(GrantType grantType,
                                          std::string const &code,
                                          std::string const &redirectUri,
                                          std::string clientID,
                                          std::string clientSecret) {
  Token token;
  return token;
}

TResult<std::vector<Device>> getAvailableDevices(
    std::string const &accessToken) {
  std::vector<Device> devices;
  devices.emplace_back(Device{});

  return std::move(devices);
}

TResult<Playback> getCurrentPlayback(std::string const &accessToken,
                                     std::string const &market) {
  Playback playback;

  return playback;
}