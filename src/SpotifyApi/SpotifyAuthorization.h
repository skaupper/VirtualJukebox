/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAuthorization definition
 */

#ifndef SPOTIFYAUTHORIZATION_H_INCLUDED
#define SPOTIFYAUTHORIZATION_H_INCLUDED

#include "Result.h"
#include "SpotifyAPITypes.h"

namespace SpotifyApi {

class SpotifyAuthorization {
 public:
  void startServer(void);
  std::string const &getRefreshToken(void);
  std::string const &getAccessToken(void);
  TResultOpt refreshAccessToken(void);
  int getExpiresAt(void);
  void setScopes(std::vector<std::string> const &scopes);
  std::vector<std::string> getScopes(void);

 private:
};

}  // namespace SpotifyApi

#endif  // SPOTIFYAUTHORIZATION_H_INCLUDED
