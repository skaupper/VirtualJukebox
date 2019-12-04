/**
 * @file    SpotifyAPITypes.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Contains Structures for the Spotify Api implementation
 * @details These Structures are implemented as described the Spotify Object
 * Model Link:
 * https://developer.spotify.com/documentation/web-api/reference/object-model/
 *          Some structures doesn't contain the whole information given in the
 * object model
 */

#include "SpotifyAPITypes.h"

#include <iostream>

using namespace SpotifyApi;

Token::Token(const nlohmann::json &tokenJson) {
  std::cout << "Token: " << tokenJson.dump(4) << std::endl;
  for (auto &[key, value] : tokenJson.items()) {
    if (key == "access_token") {
      mAccesToken = value;
    } else if (key == "token_type") {
      mTokenType = value;
    } else if (key == "scope") {
      mScope = value;
    } else if (key == "expires_in") {
      mExpiresIn = value;
    } else if (key == "refresh_token") {
      mRefreshToken = value;
    }
  }
}

std::string Token::getAccessToken() {
  return mAccesToken;
}

size_t Token::getExpiresIn() {
  return mExpiresIn;
}

std::string Token::getRefreshToken() {
  return mRefreshToken;
}

std::string Token::getScope() {
  return mScope;
}

std::string Token::getTokenType() {
  return mTokenType;
}