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
  std::string const &getID();
  bool isActive();
  bool isPrivateSession();
  bool isRestricted();
  std::string const &getName();
  std::string const &getType();
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
struct Artist {
  std::string href; /**< A link to the Web API endpoint providing full details
                       of the artist */
  std::string id;   /**< the Spotify ID for the artist */
  std::string name; /**< the name of the artist */
  std::string type; /**< object type (always "artist") */
  std::string uri;  /**< Spotify URI for the artist */
};

/**
 * @brief simplified track object
 */
struct Track {
  std::vector<Artist> artists; /**< the artists who performed the track */
  std::vector<std::string> availableMarkets; /**< a list of countries in which
                                                the track can be played */
  size_t durationMs; /**< the track length in milliseconds */
  std::string href;  /**< a link to the wep api endpoint providing full details
                        of the track */
  std::string id;    /**< Spotify ID for the track */
  std::string name;  /**< name of the track */
  std::string uri;   /**< spotify URI for the track */
};

/**
 * @brief contains infos about the actual playback
 */
class Playback {
 public:
 private:
  Device device;           /**< device that is currently active */
  std::string repeatState; /**< current repeat state status ("off", "track",
                              "context") */
  bool shuffleState;       /**< if shuffle is on or off */
  size_t timestamp;  /**< unix millisecond timestamp when data was fetched */
  size_t progressMs; /**< progress into the currently playing track */
  bool isPlaying;    /**< if something is currently playing */
  std::string currentPlayingType; /**< current playing type, can be "track",
                                     "episode", "ad", "unknown" */
  Track item;                     /**< currently playing track */
};

/**
 * @brief error type
 */
class SpotifyError {
 public:
  SpotifyError(nlohmann::json errorJson);
  int getStatus() const;
  std::string const &getMessage() const;

 private:
  int mStatus;          /**< http status code */
  std::string mMessage; /**< a short description of the cause of the error */
};

}  // namespace SpotifyApi

#endif  // VIRTUALJUKEBOX_SPOTIFYAPITYPES_H
