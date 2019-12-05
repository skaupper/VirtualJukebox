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

Playback::Playback(nlohmann::json playbackJson) {
  std::cout << playbackJson.dump(4) << std::endl;
  for (auto& [key, value] : playbackJson.items()) {
    if (key == "device") {
      mDevice = Device(value);
    } else if (key == "repeat_state") {
      mRepeatState = value;
    } else if (key == "shuffle_state") {
      mShuffleState = value;
    } else if (key == "timestamp") {
      mTimestamp = value;
    } else if (key == "progress_ms") {
      mProgressMs = value;
    } else if (key == "is_playing") {
      mIsPlaying = value;
    } else if (key == "currently_playing_type") {
      mCurrentPlayingType = value;
    } else if (key == "item") {
      mTrack = Track(value["item"]);
    }
  }
}

Device& Playback::getDevice() {
  return mDevice;
}
bool Playback::isPlaying() {
  return mIsPlaying;
}
std::string const& Playback::getRepeatState() {
  return mRepeatState;
}
size_t Playback::getProgressMs() {
  return mProgressMs;
}
Track const& Playback::getCurrentPlayingTrack() {
  return mTrack;
}
std::string const& Playback::getCurrentPlayingType() {
  return mCurrentPlayingType;
}
size_t Playback::getTimestamp() {
  return mTimestamp;
}
bool Playback::getShuffleState() {
  return mShuffleState;
}

Artist::Artist(nlohmann::json artistJson) {
  std::cout << artistJson.dump(4) << std::endl;
  for (auto& [key, value] : artistJson.items()) {
    if (key == "href") {
      mHref = value;
    } else if (key == "id") {
      mId = value;
    } else if (key == "name") {
      mName = value;
    } else if (key == "type") {
      mType = value;
    } else if (key == "uri") {
      mUri = value;
    }
  }
}
std::string Artist::getHref() {
  return mHref;
}
std::string Artist::getID() {
  return mId;
}
std::string Artist::getName() {
  return mName;
}
std::string Artist::getType() {
  return mType;
}
std::string Artist::getUri() {
  return mUri;
}

Album::Album(nlohmann::json albumJson) {
  std::cout << albumJson.dump(4) << std::endl;
  for (auto& [key, value] : albumJson.items()) {
    if (key == "artists") {
      for (nlohmann::json& elem : albumJson["artists"]) {
        mArtists.emplace_back(Artist(elem));
      }
    } else if (key == "album_type") {
      mAlbumType = value;
    } else if (key == "href") {
      mHref = value;
    } else if (key == "id") {
      mId = value;
    } else if (key == "name") {
      mName = value;
    } else if (key == "uri") {
      mUri = value;
    } else if (key == "release_date") {
      mReleaseDate = value;
    } else if (key == "type") {
      mType = value;
    }
  }
}

std::vector<Artist> const& Album::getArtists() {
  return mArtists;
}
std::string const& Album::getAlbumType() {
  return mAlbumType;
}
std::string const& Album::getHref() {
  return mHref;
}
std::string const& Album::getId() {
  return mId;
}
std::string const& Album::getName() {
  return mName;
}
std::string const& Album::getReleaseDate() {
  return mReleaseDate;
}
std::string const& Album::getType() {
  return mType;
}
std::string const& Album::getUri() {
  return mUri;
}

Track::Track(nlohmann::json trackJson) {
  std::cout << trackJson.dump(4) << std::endl;
  for (auto& [key, value] : trackJson.items()) {
    if (key == "artists") {
      for (nlohmann::json& elem : trackJson["artists"]) {
        mArtists.emplace_back(Artist(elem));
      }
    } else if (key == "album") {
      mAlbum = Album(trackJson["album"]);
    } else if (key == "href") {
      mHref = value;
    } else if (key == "id") {
      mId = value;
    } else if (key == "name") {
      mName = value;
    } else if (key == "uri") {
      mUri = value;
    } else if (key == "duration_ms") {
      mDurationMs = value;
    }
  }
}

std::vector<Artist> const& Track::getArtists() {
  return mArtists;
}
Album const& Track::getAlbum() {
  return mAlbum;
}
size_t Track::getDuration() {
  return mDurationMs;
}
std::string const& Track::getHref() {
  return mHref;
}
std::string const& Track::getId() {
  return mId;
}
std::string const& Track::getName() {
  return mName;
}
std::string const& Track::getUri() {
  return mUri;
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