/*****************************************************************************/
/**
 * @file    SpotifyBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class handles Music Playback with a Spotify Backend
 */
/*****************************************************************************/

#ifndef _SPOTIFYBACKEND_H_
#define _SPOTIFYBACKEND_H_

#include "MusicBackend.h"
#include "SpotifyAPI.h"
#include "SpotifyAuthorization.h"
#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"

/**
 * @brief spotify music backend class which handles api calls and starts the
 * authentication process
 * @details all methods are capable of refreshing the token, if it gets expired
 */
class SpotifyBackend : public MusicBackend {
 public:
  /**
   * @brief initalizes the backend
   * @details this function must be called to start the authorization server
   * which is responsible for accessing the "access token"
   * @return none
   */
  virtual TResultOpt initBackend(void) override;

  /**
   * @brief starts a search request with the given pattern to the spotify api
   * @param pattern search patten (can also contain 2 wildcards *)
   * @param num maximum number of tracks get returned (max is 50)
   * @return on success vector of BaseTrack, else Error
   */
  virtual TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &pattern, size_t const num) override;

  /**
   * @brief set the given track as playback (means the track gets played)
   * @details the method checks first if there are any spotify players
   * (webplayer, smartphone,...), if not KeyNotFound Error gets returned. Next
   * step is to check if in the config.ini is a playingDevice is given. if yes,
   * and this playing device is found, this device gets selected for the
   * playback, else the actual selected (spotify select) playing device gets
   * used
   * @param track track to play
   * @return std::nullopt on succeed, otherwise Error
   */
  virtual TResultOpt setPlayback(BaseTrack const &track) override;

  /**
   * @brief returns the current playback of the active device as a playback
   * track
   * @return actual playback track on success (or nothing if none is currently
   * played), otherwise Error
   */
  virtual TResult<std::optional<PlaybackTrack>> getCurrentPlayback(
      void) override;

  /**
   * @brief pauses the actual playback
   * @return std::nullopt on success, else Error
   */
  virtual TResultOpt pause(void) override;

  /**
   * @brief resumes the actual playback
   * @return on failure (no active playback,..) returns Error
   */
  virtual TResultOpt play() override;

  /**
   * @brief returns the volume of the actual device in percent
   * @return volume in percent on success, otherwise Error
   */
  virtual TResult<size_t> getVolume(void) override;

  /**
   * @brief sets a new volume
   * @details if a device is given in the ini file, this device gets selected,
   * if not found, the actual active device
   * @param percent volume in percent
   * @return on failure Error
   */
  virtual TResultOpt setVolume(size_t const percent) override;

  /**
   * @brief creates a new BaseTrack item with the given trackID
   * @details the trackID is the same as the spotify uri string
   * @param trackID track id returned for example from queryTracks
   * @return new BaseTrack object on success, otherwise Error
   */
  virtual TResult<BaseTrack> createBaseTrack(TTrackID const &trackID) override;

 private:
  TResultOpt errorHandler(Error const &error);
  SpotifyApi::SpotifyAPI mSpotifyAPI;
  SpotifyApi::SpotifyAuthorization mSpotifyAuth;
};

#endif /* _SPOTIFYBACKEND_H_ */
