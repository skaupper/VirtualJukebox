/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAuthorization definition
 */

#ifndef SPOTIFYAUTHORIZATION_H_INCLUDED
#define SPOTIFYAUTHORIZATION_H_INCLUDED

#include "SpotifyAPI.h"
#include "SpotifyAPITypes.h"
#include "Types/Result.h"
#include "httpserver.hpp"

namespace SpotifyApi {

class SpotifyAuthorization : public httpserver::http_resource {
 public:
  bool startServer(void);
  std::string const &getRefreshToken(void);
  std::string const &getAccessToken(void);
  TResultOpt refreshAccessToken(void);
  __int64_t getExpiresAt(void);
  void setScopes(std::string const &scopes);
  std::string getScopes(void);

 private:
  Token mToken;
  __int64_t mTokenReceiveTime = 0;
  std::string mClientID = "f589b31542ca45a98c076460a021e086";
  std::string mScopes =
      "user-read-private%20user-read-email%20app-remote-control%20user-modify-"
      "playback-state";
  std::string mRedirectUri = "http%3A%2F%2Flocalhost%3A8080%2FspotifyCallback";
  std::string mClientSecret = "0a82fbaa5fae43f0904776d6f695a7ad";

  const std::shared_ptr<httpserver::http_response> render(
      httpserver::http_request const &request);

  const std::shared_ptr<httpserver::http_response> loginHandler(
      httpserver::http_request const &request);
  const std::shared_ptr<httpserver::http_response> callbackHandler(
      httpserver::http_request const &request);

  std::string generateRandomString(size_t length);
  std::string getFromQueryString(std::string const &query,
                                 std::string const &key);
};

}  // namespace SpotifyApi

#endif  // SPOTIFYAUTHORIZATION_H_INCLUDED
