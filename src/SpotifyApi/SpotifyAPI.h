/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAPI definition
 */

#ifndef SPOTIFYAPI_H_INCLUDED
#define SPOTIFYAPI_H_INCLUDED

#include "Result.h"
#include "SpotifyAPITypes.h"

namespace SpotifyApi {

class SpotifyAPI {
  TResult<Token> getAccessToken(GrantType grantType,
                                std::string const &code,
                                std::string const &redirectUri,
                                std::string clientID,
                                std::string clientSecret);

  TResult<std::vector<Device>> getAvailableDevices(
      std::string const &accessToken);

  TResult<Playback> getCurrentPlayback(std::string const &accessToken,
                                       std::string const &market);
};

}  // namespace SpotifyApi

#endif  // SPOTIFYAPI_H_INCLUDED
