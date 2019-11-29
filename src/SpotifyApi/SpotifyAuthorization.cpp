/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAuthorization implementation
 */

#include "SpotifyAuthorization.h"

using namespace SpotifyApi;

void SpotifyAuthorization::startServer(void) {
}
std::string const &getRefreshToken(void) {
  return std::string("");
}
std::string const &getAccessToken(void) {
  return std::string("");
}
TResultOpt refreshAccessToken(void) {
  return Error(ErrorCode::AccessDenied, "Test");
}
int getExpiresAt(void) {
  return 0;
}
void setScopes(std::vector<std::string> const &scopes) {
}
std::vector<std::string> getScopes(void) {
  return std::vector<std::string>();
}