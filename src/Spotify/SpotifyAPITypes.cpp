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
std::string const& Device::getName() const {
  return mName;
}
std::string const& Device::getType() const {
  return mType;
}
size_t Device::getVolume() {
  return mVolume;
}
std::string const& Device::getID() const {
  return mId;
}
bool Device::isActive() {
  return mIsActive;
}
bool Device::isRestricted() {
  return mIsRestricted;
}

Playback::Playback(nlohmann::json const& playbackJson) {
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
      mTrack = Track(value);
    }
  }
}

Device const& Playback::getDevice() const {
  return mDevice;
}
bool Playback::isPlaying() {
  return mIsPlaying;
}
std::string const& Playback::getRepeatState() const {
  return mRepeatState;
}
size_t Playback::getProgressMs() {
  return mProgressMs;
}
Track const& Playback::getCurrentPlayingTrack() const {
  return mTrack;
}
std::string const& Playback::getCurrentPlayingType() const {
  return mCurrentPlayingType;
}
size_t Playback::getTimestamp() {
  return mTimestamp;
}
bool Playback::getShuffleState() {
  return mShuffleState;
}

Artist::Artist(nlohmann::json const& artistJson) {
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
std::string const& Artist::getHref() const {
  return mHref;
}
std::string const& Artist::getID() const {
  return mId;
}
std::string const& Artist::getName() const {
  return mName;
}
std::string const& Artist::getType() const {
  return mType;
}
std::string const& Artist::getUri() const {
  return mUri;
}

Image::Image(nlohmann::json const& imageJson) {
  for (auto& [key, value] : imageJson.items()) {
    if (key == "height") {
      mHeight = value;
    } else if (key == "width") {
      mWidth = value;
    } else if (key == "url") {
      mUrl = value;
    }
  }
}
int Image::getHeight() {
  return mHeight;
}
int Image::getWidth() {
  return mWidth;
}
std::string const& Image::getUrl() const {
  return mUrl;
}

Album::Album(nlohmann::json const& albumJson) {
  for (auto& [key, value] : albumJson.items()) {
    if (key == "artists") {
      for (auto& elem : value) {
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
    } else if (key == "images") {
      for (auto& elem : value) {
        mImages.emplace_back(Image(elem));
      }
    }
  }
}

std::vector<Artist> const& Album::getArtists() const {
  return mArtists;
}
std::string const& Album::getAlbumType() const {
  return mAlbumType;
}
std::string const& Album::getHref() const {
  return mHref;
}
std::string const& Album::getId() const {
  return mId;
}
std::string const& Album::getName() const {
  return mName;
}
std::string const& Album::getReleaseDate() const {
  return mReleaseDate;
}
std::string const& Album::getType() const {
  return mType;
}
std::string const& Album::getUri() const {
  return mUri;
}
std::vector<Image> const& Album::getImages() const {
  return mImages;
}

Track::Track(nlohmann::json const& trackJson) {
  for (auto& [key, value] : trackJson.items()) {
    if (key == "artists") {
      for (auto& elem : value) {
        mArtists.emplace_back(Artist(elem));
      }
    } else if (key == "album") {
      mAlbum = Album(value);
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

std::vector<Artist> const& Track::getArtists() const {
  return mArtists;
}
Album const& Track::getAlbum() const {
  return mAlbum;
}
size_t Track::getDuration() {
  return mDurationMs;
}
std::string const& Track::getHref() const {
  return mHref;
}
std::string const& Track::getId() const {
  return mId;
}
std::string const& Track::getName() const {
  return mName;
}
std::string const& Track::getUri() const {
  return mUri;
}

SpotifyPaging::SpotifyPaging(nlohmann::json const& pagingJson) {
  for (auto& [key, value] : pagingJson.items()) {
    if (key == "artists") {
      for (auto& elem : value) {
        mArtists.emplace_back(Artist(elem));
      }
    } else if (key == "albums") {
      for (auto& elem : value) {
        mAlbums.emplace_back(Album(elem));
      }
    } else if (key == "tracks") {
      for (auto& elem : value) {
        mTracks.emplace_back(Track(elem));
      }
    } else if (key == "href") {
      mHref = value;
    } else if (key == "limit") {
      mLimit = value;
    } else if (key == "next") {
      mNext = value;
    } else if (key == "offset") {
      mOffset = value;
    } else if (key == "previous") {
      mPrevious = value;
    } else if (key == "total") {
      mTotal = value;
    }
  }
}

std::vector<Track> const& SpotifyPaging::getTracks() {
  return mTracks;
}
std::vector<Artist> const& SpotifyPaging::getArtists() {
  return mArtists;
}
std::vector<Album> const& SpotifyPaging::getAlbums() {
  return mAlbums;
}
std::string const& SpotifyPaging::getHref() {
  return mHref;
}
int SpotifyPaging::getLimit() {
  return mLimit;
}
std::string const& SpotifyPaging::getNext() {
  return mNext;
}
int SpotifyPaging::getOffset() {
  return mOffset;
}
std::string const& SpotifyPaging::getPrevious() {
  return mPrevious;
}
int SpotifyPaging::getTotal() {
  return mTotal;
}

SpotifyError::SpotifyError(nlohmann::json const& errorJson) {
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