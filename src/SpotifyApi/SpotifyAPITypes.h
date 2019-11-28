/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Contains Structures for the Spotify Api
 * @details These Structures are implemented as described the Spotify Object Model
 *          Link: https://developer.spotify.com/documentation/web-api/reference/object-model/
 *          Some structures doesn't contain the whole information given in the object model
 */

#ifndef SPOTIFYAPITYPES_H_INCLUDED
#define SPOTIFYAPITYPES_H_INCLUDED

#include <string>
#include <vector>

namespace SpotifyAPI{

/**
 * @brief Possible Autorization Flows (until now only AuthorizationCode gets implemented)
 */
enum GrantType{AuthorizationCode, ClientCredentials, ImplicitGrant};

/**
 * @brief return type on valid token request
 */
struct Token {
    std::string accesToken;     /**< Access token to interact with the SpotifyWebAPI */
    std::string refreshToken;   /**< Can be used to refresh the accessToken when expired */
    std::string tokenType;      /**< Type of the token (always "Bearer" */
    std::string scope;          /**< a list of space seperated scopes granted for this acces token */
    size_t      expiresIn;      /**< time period (in seconds) for which the access token is valid */
};

/**
 * @brief Device Object
 */
struct Device {
    std::string id;            /**< the device id. may be empty */
    bool isActive;             /**< if this device is the currently active device */
    bool isPrivateSession;     /**< if this device is currently in a private sesseion */
    bool isRestricted;         /**< if true, no web api commands will be accepted by this device */
    std::string name;          /**< name of the device */
    std::string type;          /**< type of the device (Computer, Smartphone, Speaker, ...) */
    size_t volume;             /**< current volume in percent */
};


/**
 * @brief simplified artist object
 */
struct Artist{
    std::string href;  /**< A link to the Web API endpoint providing full details of the artist */
    std::string id;    /**< the Spotify ID for the artist */
    std::string name;  /**< the name of the artist */
    std::string type;  /**< object type (always "artist") */
    std::string uri;   /**< Spotify URI for the artist */
};

/**
 * @brief simplified track object
 */
struct Track{
    std::vector<Artist> artists;                /**< the artists who performed the track */
    std::vector<std::string> availableMarkets;  /**< a list of countries in which the track can be played */
    size_t durationMs;    /**< the track length in milliseconds */
    std::string href;     /**< a link to the wep api endpoint providing full details of the track */
    std::string id;       /**< Spotify ID for the track */
    std::string name;     /**< name of the track */
    std::string uri;      /**< spotify URI for the track */
};

/**
 * @brief contains infos about the actual playback
 */
struct Playback{
    Device device;           /**< device that is currently active */
    std::string repeatState;  /**< current repeat state status ("off", "track", "context") */
    bool shuffleState;        /**< if shuffle is on or off */
    size_t timestamp;         /**< unix millisecond timestamp when data was fetched */
    size_t progressMs;        /**< progress into the currently playing track */
    bool isPlaying;           /**< if something is currently playing */
    std::string currentPlayingType;  /**< current playing type, can be "track", "episode", "ad", "unknown" */
    Track item;              /**< currently playing track */
};

/**
 * @brief error type
 */
struct Error{
    int status;          /**< http status code */
    std::string message; /**< a short description of the cause of the error */
};

}

#endif //VIRTUALJUKEBOX_SPOTIFYAPITYPES_H
