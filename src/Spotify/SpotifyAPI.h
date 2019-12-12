/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAPI definition
 */

#ifndef SPOTIFYAPI_H_INCLUDED
#define SPOTIFYAPI_H_INCLUDED

#include "SpotifyAPITypes.h"
#include "Types/Result.h"
#include "restclient.h"

namespace SpotifyApi {

enum class QueryType { album, artist, playlist, track };

/**
 * @brief handles the calls with the spotify web api
 */
class SpotifyAPI {
 public:
  /**
   * @brief requests a Token (access token and refresh token) from the spotify
   * web api
   * @param grantType which authorization type to use
   * @param code authentification code (received from Spotify Authorization)
   * @param redirectUri redirection url (only for comparing)
   * @param clientID    clientID (from spotify developers dashboard)
   * @param clientSecret clientSecret (from spotify developers dashboard)
   * @details this function fetches a token from the spotify web api. until now,
   * only the GrantType AuthorizationCode is implemented.
   * @return if succeeded, the function returns a valid Token, otherwise a Error
   */
  TResult<Token> getAccessToken(GrantType grantType,
                                std::string const &code,
                                std::string const &redirectUri,
                                std::string clientID,
                                std::string clientSecret);

  /**
   * @brief requests with the refresh token a new access token
   * @param refreshToken the refresh token gathered from getAccessToken(..)
   * @param clientID    clientID (from spotify developers dashboard)
   * @param clientSecret clientSecret (from spotify developers dashboard)
   * @details this function fetches a new token from the spotify web api.
   * Caution it only fetches a access token, not a new refresh token (because it
   * doesn't need to be updated) so the refresh token value from the return will
   * be empty!!
   * @return if succeeded, the function returns a valid Token, otherwise a Error
   */
  TResult<Token> refreshAccessToken(std::string const &refreshToken,
                                    std::string const &clientID,
                                    std::string const &clientSecret);

  /**
   * @brief returns vector of available spotify devices
   * @param accessToken valid access token to use the spotify api
   * @return if succeeded vector of Devices, otherwise Error
   */
  TResult<std::vector<Device>> getAvailableDevices(
      std::string const &accessToken);

  /**
   * @brief gets the current playback status of the current active device from
   * spotify
   * @param accessToken valid access token
   * @param market specifies, that only tracks get returned which are valid in
   * the given market (not implemented)
   * @details a playback is the actual status of the player, if its playing,
   * wich device, which track and so on..
   * @return  if succeeded it returns eher a Playback object, else Error
   */
  TResult<std::optional<Playback>> getCurrentPlayback(
      std::string const &accessToken, std::string const &market = "AT");

  /**
   * @brief searches for tracks,artists,albums or playlists with the given query
   * @param accessToken valid access token
   * @param queryKey the search string
   * @param type specifies if it gets searched for tracks, artists, albums, or
   * playlists
   * @param limit sets the limit of maximum number returned tracks,..
   * @param offset number offset
   * @param market only return tracks,.. which are valid in the given market
   * @details the queryKey can also contain 2 wildcards. for further details on
   * the query string see
   * https://developer.spotify.com/documentation/web-api/reference/search/search/
   * @return if succeeded SpotifyPaging object else Error
   */
  TResult<SpotifyPaging> search(std::string const &accessToken,
                                std::string const &queryKey,
                                QueryType type,
                                int const limit = 20,
                                int const offset = 0,
                                std::string const &market = "AT");

  /**
   * @brief sets a volume
   * @param accessToken valid access token
   * @param volume volume in percent (0-100)
   * @param device select on which device to set the volume (if not given, the
   * current active device will be used)
   * @return on failure a Error object gets returned
   */
  TResultOpt setVolume(std::string const &accessToken,
                       int volume,
                       Device const &device = Device());

  /**
   * @brief pauses the acutal track
   * @param accessToken valid access token
   * @param device select on which device to pause (if not given, the current
   * active device will be used)
   * @return on failure a Error object gets returned
   */
  TResultOpt pause(std::string const &accessToken,
                   Device const &device = Device());

  /**
   * @brief plays new songs, or resume the actual song
   * @param accessToken valid access token
   * @param uris a vector of spotify uris (ids) which will be queued and played
   * @param device select on which device to play (if not given, the current
   * active device will be used)
   * @param positionMs set the start position in ms of the new track
   * @details if uris and devices are left empty, the actual song gets resumed,
   * and if the positionMs is greater than the duration of the song, the next
   * song will be played
   * @return on failure a Error object gets returned
   */
  TResultOpt play(
      std::string const &accessToken,
      std::vector<std::string> const &uris = std::vector<std::string>(),
      Device const &device = Device(),
      int positionMs = 0);

  /**
   * @brief searches for the given track id and returns a simplified track
   * object
   * @param accessToken valid access token
   * @param spotifyID spotify id of the song (without "spotify:track:" string)
   * @param market only return tracks, which are valid in the given market
   * @return if succeeded Track Object, otherwise Error
   */
  TResult<Track> getTrack(std::string const &accessToken,
                          std::string const &spotifyID,
                          std::string const &market = "AT");

  /**
   * @brief enables a playback on the given device
   * @details this function has to be called, when a device has no actual
   * playback. Note: Note that a value of false for the play parameter when also
   * transferring to another device_id will not pause playback. To ensure that
   * playback is paused on the new device you should send a pause command to the
   * currently active device before transferring to the new device_id
   * @param accessToken valid access token
   * @param devices its possible to set an array, but until now only a single
   * device is currently supported
   * @param play true ensures playback happends on new device otherwise keep the
   * current playback state
   * @return on failure a Error object
   */
  TResultOpt transferUsersPlayback(std::string const &accessToken,
                                   std::vector<Device> const &devices,
                                   bool play = false);

  /**
   * @brief encodes the given string with http special characters
   * @param str string to encode
   * @return encoded string
   */
  static std::string stringUrlEncode(std::string const &str);

  /**
   * @brief encodes the given string with base64
   * @param str string to encode
   * @return encoded string
   */
  static std::string stringBase64Encode(std::string const &str);

 private:
  /**
   * @brief parses the spotify error into an Error object
   * @param error spotify error
   * @return Error
   */
  Error errorParser(SpotifyError const &error);

  enum HttpMethod { HttpGet, HttpPost, HttpPut };
  TResult<RestClient::Response> spotifyCall(std::string const &accessToken,
                                            std::string const &endpoint,
                                            HttpMethod method,
                                            std::string const &query = "",
                                            std::string const &body = "");

  template <typename SpotifyAPIType>
  TResult<SpotifyAPIType> parseSpotifyCall(
      RestClient::Response const &response);

  std::string const cSpotifyAuthUrl = "https://accounts.spotify.com";
  std::string const cSpotifyAPIUrl = "https://api.spotify.com";
  size_t const cRequestTimeout = 5;
  std::map<QueryType, std::string> const cQueryTypeMap = {
      {QueryType::album, "album"},
      {QueryType::track, "track"},
      {QueryType::artist, "artist"},
      {QueryType::playlist, "playlist"}};

  int const cHTTPTimeout = 408;
  int const cHTTPBadRequest = 400;
  int const cHTTPUnouthorized = 401;
  int const cHTTPOK = 200;
  int const cHTTPNotFound = 404;
  int const cHTTPForbidden = 403;
  int const cNoContent = 204;
};

}  // namespace SpotifyApi

#endif  // SPOTIFYAPI_H_INCLUDED
