/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAuthorization definition
 */

#ifndef SPOTIFYAUTHORIZATION_H_INCLUDED
#define SPOTIFYAUTHORIZATION_H_INCLUDED

#include <mutex>
#include <thread>

#include "SpotifyAPI.h"
#include "SpotifyAPITypes.h"
#include "Types/Result.h"
#include "httpserver.hpp"

namespace SpotifyApi {

/**
 * @brief handles the 3 parties user authentication
 */
class SpotifyAuthorization : public httpserver::http_resource {
 public:
  ~SpotifyAuthorization();
  /**
   * @brief starts the server, on which the user can connect
   * @return if failed Error object gets returned and no server has been
   * created..
   */
  TResultOpt startServer(void);

  /**
   * @brief stops the server
   */
  void stopServer(void);

  /**
   * @brief returns refresh token
   * @return refresh token string
   */
  std::string const &getRefreshToken(void);

  /**
   * @brief returns access token
   * @return refresh access string
   */
  std::string const &getAccessToken(void);

  /**
   * @brief refreshes the access token (!! not implemented yet!!)
   * @return on failer Error object
   */
  TResultOpt refreshAccessToken(void);

  /**
   * @brief returns when the token expires
   * @return time when the token expires (in epoch format and seconds)
   */
  __int64_t getExpiresAt(void);

  /**
   * @brief sets the scopes to ask on the permission
   * @param scopes scopes
   * @details see
   * https://developer.spotify.com/documentation/general/guides/scopes/ for
   * valid scopes
   */
  void setScopes(std::string const &scopes);

  /**
   * @brief return scopes
   * @return scopes
   */
  std::string getScopes(void);

 private:
  Token mToken;
  __int64_t mTokenReceiveTime = 0;
  std::string mClientID = "";
  std::string mScopes = "";
  std::string mRedirectUri = "";
  std::string mClientSecret = "";
  int mPort = 8080;
  std::string const cSectionKey = "Spotify";
  std::string const cClientIDKey = "clientID";
  std::string const cClientSecretKey = "clientSecret";
  std::string const cPortKey = "port";
  std::string const cRedirectUriKey = "redirectUri";
  std::string const cScopesKey = "scopes";
  std::unique_ptr<std::thread> mServerThread;
  bool shutdownServer = false;

  const std::shared_ptr<httpserver::http_response> render(
      httpserver::http_request const &request);

  const std::shared_ptr<httpserver::http_response> loginHandler(
      httpserver::http_request const &request);
  const std::shared_ptr<httpserver::http_response> callbackHandler(
      httpserver::http_request const &request);

  void startServerThread();
  TResultOpt setupConfigParams();
  std::string generateRandomString(size_t length);
  std::string getFromQueryString(std::string const &query,
                                 std::string const &key);
};

}  // namespace SpotifyApi

#endif  // SPOTIFYAUTHORIZATION_H_INCLUDED
