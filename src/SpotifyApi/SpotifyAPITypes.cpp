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

Token::Token(nlohmann::json const& tokenJson) {
  std::cout << "Token: " << tokenJson.dump(4) << std::endl;
  for (auto& [key, value] : tokenJson.items()) {
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

Device::Device(nlohmann::json const& deviceJson) {
  std::cout << "Device: " << deviceJson.dump(4) << std::endl;
  for (auto& [key, value] : deviceJson.items()) {
    if (key == "id") {
      mId = value;
    } else if (key == "is_active") {
      mIsActive = value;
    } else if (key == "is_private_session") {
      mIsPrivateSession = value;
    } else if (key == "is_restricted") {
      mIsRestricted = value;
    } else if (key == "name") {
      mName = value;
    } else if (key == "type") {
      mType = value;
    } else if (key == "volume_percent") {
      mVolume = value;
    }
  }
}

bool Device::isPrivateSession() {
  return mIsPrivateSession;
}
std::string const& Device::getName() {
  return mName;
}
std::string const& Device::getType() {
  return mType;
}
size_t Device::getVolume() {
  return mVolume;
}
std::string const& Device::getID() {
  return mId;
}
bool Device::isActive() {
  return mIsActive;
}
bool Device::isRestricted() {
  return mIsRestricted;
}


SpotifyError::SpotifyError(nlohmann::json errorJson) {
  std::cout << "Error: " << errorJson.dump(4) << std::endl;
  for (auto& [key, value] : errorJson.items()) {
    if (key == "status") {
      mStatus = value;
    } else if (key == "message") {
      mMessage = value;
    }
  }
}

std::string const& SpotifyError::getMessage() const {
  return mMessage;
}
int SpotifyError::getStatus() const {
  return mStatus;
}