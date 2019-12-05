/**
 * @file    SpotifyAPITypes.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Contains Structures for the Spotify Api
 * @details These Structures are implemented as described the Spotify Object
 * Model Link:
 * https://developer.spotify.com/documentation/web-api/reference/object-model/
 *          Some structures doesn't contain the whole information given in the
 * object model
 */

#ifndef SPOTIFYAPITYPES_H_INCLUDED
#define SPOTIFYAPITYPES_H_INCLUDED

#include <string>
#include <vector>

#include "json/json.hpp"

namespace SpotifyApi {

/**
 * @brief Possible Autorization Flows (until now only AuthorizationCode gets
 * implemented)
 */
enum GrantType { AuthorizationCode, ClientCredentials, ImplicitGrant };

/**
 * @brief return type on valid token request
 */
class Token {
 public:
  Token() = default;
  Token(nlohmann::json const &tokenJson);

  std::string getAccessToken();
  std::string getRefreshToken();
  std::string getTokenType();
  std::string getScope();
  size_t getExpiresIn();

 private:
  std::string
      mAccesToken; /**< Access token to interact with the SpotifyWebAPI */
  std::string
      mRefreshToken; /**< Can be used to refresh the accessToken when expired */
  std::string mTokenType; /**< Type of the token (always "Bearer" */
  std::string mScope;     /**< a list of space seperated scopes granted for this
                            acces token */
  size_t mExpiresIn; /**< time period (in seconds) for which the access token is
                       valid */
};

/**
 * @brief Device Object
 */
class Device {
 public:
  Device() = default;
  Device(nlohmann::json const &deviceJson);
  std::string const &getID() const;
  bool isActive();
  bool isPrivateSession();
  bool isRestricted();
  std::string const &getName() const;
  std::string const &getType() const;
  size_t getVolume();

 private:
  std::string mId;        /**< the device id. may be empty */
  bool mIsActive;         /**< if this device is the currently active device */
  bool mIsPrivateSession; /**< if this device is currently in a private session
                           */
  bool mIsRestricted; /**< if true, no web api commands will be accepted by this
                        device */
  std::string mName;  /**< name of the device */
  std::string
      mType; /**< type of the device (Computer, Smartphone, Speaker, ...) */
  size_t mVolume; /**< current volume in percent */
};

/**
 * @brief simplified artist object
 */
class Artist {
 public:
  Artist() = default;
  Artist(nlohmann::json const &artistJson);
  std::string const &getHref() const;
  std::string const &getID() const;
  std::string const &getName() const;
  std::string const &getType() const;
  std::string const &getUri() const;

 private:
  std::string mHref; /**< A link to the Web API endpoint providing full details
                       of the artist */
  std::string mId;   /**< the Spotify ID for the artist */
  std::string mName; /**< the name of the artist */
  std::string mType; /**< object type (always "artist") */
  std::string mUri;  /**< Spotify URI for the artist */
};

/**
 * @brief Spotify Image Class
 */
class Image {
 public:
  Image() = default;
  Image(nlohmann::json const &imageJson);
  int getHeight();
  int getWidth();
  std::string const &getUrl() const;

 private:
  int mHeight;      /**< the image height in pixels, if unknown 0 */
  int mWidth;       /**< the image width in pixels, if unknown 0 */
  std::string mUrl; /**< the source url of the image */
};

/**
 * @brief simplified Spotify Album class
 */
class Album {
 public:
  Album() = default;
  Album(nlohmann::json const &albumJson);
  std::vector<Artist> const &getArtists() const;
  std::vector<Image> const &getImages() const;
  std::string const &getAlbumType() const;
  std::string const &getHref() const;
  std::string const &getId() const;
  std::string const &getName() const;
  std::string const &getReleaseDate() const;
  std::string const &getType() const;
  std::string const &getUri() const;

 private:
  std::string
      mAlbumType; /**< type of album (can contain album, single, compilation) */
  std::vector<Artist> mArtists; /**< array of simplified artists */
  std::string mHref; /**< a link to the wep api endpoint providing full details
                        of the album */
  std::string mId;   /**< Spotify ID for the album */
  std::string mName; /**< name of the album (can be an empty string) */
  std::string
      mReleaseDate;  /**< release date ( can be 1981, 1981-12 or 1981-12-15) */
  std::string mType; /**< the object type, always "album" */
  std::string mUri;  /**< spotify uri for the album */
  std::vector<Image> mImages; /**< array of cover arts in various sizes */
};

/**
 * @brief simplified track object
 */
class Track {
 public:
  Track() = default;
  Track(nlohmann::json const &trackJson);
  std::vector<Artist> const &getArtists() const;
  Album const &getAlbum() const;
  size_t getDuration();
  std::string const &getHref() const;
  std::string const &getId() const;
  std::string const &getName() const;
  std::string const &getUri() const;

 private:
  std::vector<Artist> mArtists; /**< the artists who performed the track */
  Album mAlbum;
  size_t mDurationMs; /**< the track length in milliseconds */
  std::string mHref;  /**< a link to the wep api endpoint providing full details
                        of the track */
  std::string mId;    /**< Spotify ID for the track */
  std::string mName;  /**< name of the track */
  std::string mUri;   /**< spotify URI for the track */
};

/**
 * @brief contains infos about the actual playback
 */
class Playback {
 public:
  Playback() = default;
  Playback(nlohmann::json const &playbackJson);

  Device const &getDevice() const;
  std::string const &getRepeatState() const;
  bool getShuffleState();
  size_t getTimestamp();
  size_t getProgressMs();
  bool isPlaying();
  std::string const &getCurrentPlayingType() const;
  Track const &getCurrentPlayingTrack() const;

 private:
  Device mDevice;           /**< device that is currently active */
  std::string mRepeatState; /**< current repeat state status ("off", "track",
                              "context") */
  bool mShuffleState;       /**< if shuffle is on or off */
  size_t mTimestamp;  /**< unix millisecond timestamp when data was fetched */
  size_t mProgressMs; /**< progress into the currently playing track */
  bool mIsPlaying;    /**< if something is currently playing */
  std::string mCurrentPlayingType; /**< current playing type, can be "track",
                                     "episode", "ad", "unknown" */
  Track mTrack;                    /**< currently playing track */
};

class SpotifyPaging {
 public:
  SpotifyPaging() = default;
  SpotifyPaging(nlohmann::json const &pagingJson);

  std::vector<Track> const &getTracks();
  std::vector<Artist> const &getArtists();
  std::vector<Album> const &getAlbums();
  std::string const &getHref();
  int getLimit();
  std::string const &getNext();
  int getOffset();
  std::string const &getPrevious();
  int getTotal();

 private:
  std::vector<Track> mTracks;   /**< array of tracks */
  std::vector<Artist> mArtists; /**< array of artists */
  std::vector<Album> mAlbums;   /**< array of albums */
  std::string mHref; /**< a link to the web api endpoint returning the full
                        result of the request */
  int mLimit;        /**< the maximum number of items in the response */
  std::string mNext; /**< url to the next page of items (can be left empty) */
  int mOffset;       /**< offset of the items returned (as set in the query) */
  std::string mPrevious; /**< url to the previos page of items */
  int mTotal;            /**< total number of items available */
};

/**
 * @brief error type
 */
class SpotifyError {
 public:
  SpotifyError(nlohmann::json const &errorJson);
  int getStatus() const;
  std::string const &getMessage() const;

 private:
  int mStatus;          /**< http status code */
  std::string mMessage; /**< a short description of the cause of the error */
};

}  // namespace SpotifyApi

#endif  // VIRTUALJUKEBOX_SPOTIFYAPITYPES_H
