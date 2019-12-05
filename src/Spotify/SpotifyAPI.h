/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAPI definition
 */

#ifndef SPOTIFYAPI_H_INCLUDED
#define SPOTIFYAPI_H_INCLUDED

#include "SpotifyAPITypes.h"
#include "Types/Result.h"

namespace SpotifyApi {

/**
 * @brief handles the calls with the spotify web api
 */
class SpotifyAPI {
 public:
  /**
   * @brief posts data to web api to get a Token
   * @param grantType which authorization type to use
   * @param code authentification code
   * @param redirectUri redirection url (only for comparing)
   * @param clientID    clientID (from spotify developers dashboard)
   * @param clientSecret clientSecret (from spotify developers dashboard)
   * @return Token class
   */
  TResult<Token> getAccessToken(GrantType grantType,
                                std::string const &code,
                                std::string const &redirectUri,
                                std::string clientID,
                                std::string clientSecret);

  TResult<Token> refreshAccessToken(std::string const &refreshToken);

  /**
   * @brief returns vector of available spotify devices
   * @param accessToken
   * @return
   */
  TResult<std::vector<Device>> getAvailableDevices(
      std::string const &accessToken);

  TResult<Playback> getCurrentPlayback(std::string const &accessToken,
                                       std::string const &market = "AT");

 private:
  Error errorParser(SpotifyError const &error);
  std::string const cSpotifyAuthUrl = "https://accounts.spotify.com";
  std::string const cSpotifyAPIUrl = "https://api.spotify.com";
  size_t const cRequestTimeout = 5;

  int const cHTTPTimeout = 408;
  int const cHTTPUnouthorized = 401;
  int const cHTTPOK = 200;
  int const cNoContent = 204;
};

}  // namespace SpotifyApi

#endif  // SPOTIFYAPI_H_INCLUDED
