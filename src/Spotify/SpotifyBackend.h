/*****************************************************************************/
/**
 * @file    SpotifyBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class handles Music Playback with a Spotify Backend
 */
/*****************************************************************************/

#ifndef _SPOTIFYBACKEND_H_
#define _SPOTIFYBACKEND_H_

#include <mutex>

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
   * @details This function must be called to start the authorization server
   * which is needed to acquire an *access token*.
   * @copydoc MusicBackend::initBackend
   */
  virtual TResultOpt initBackend() override;

  virtual TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &pattern, size_t const num) override;

  /**
   * @details This method checks if there are any active Spotify devices
   * (webplayer, smartphone,...), if not \link #ErrorCode KeyNotFound\endlink is
   * returned. \n
   * The next step is to check if `playingDevice` is set in the configuration
   * file. If yes, and this device is currently active, it gets
   * selected for the playback, otherwise the currently selected (Spotify
   * Connect) device is used.
   * @copydoc MusicBackend::setPlayback
   */
  virtual TResultOpt setPlayback(BaseTrack const &track) override;

  virtual TResult<std::optional<PlaybackTrack>> getCurrentPlayback() override;

  virtual TResultOpt pause() override;

  virtual TResultOpt play() override;

  virtual TResult<size_t> getVolume() override;

  virtual TResultOpt setVolume(size_t const percent) override;

  /**
   * @details the trackID is the same as the spotify uri string
   * @copydoc MusicBackend::createBaseTrack
   */
  virtual TResult<BaseTrack> createBaseTrack(TTrackID const &trackID) override;

 private:
  TResultOpt errorHandler(Error const &error);
  SpotifyApi::SpotifyAPI mSpotifyAPI;
  SpotifyApi::SpotifyAuthorization mSpotifyAuth;

  std::mutex mPlayPauseMtx;
  std::mutex mVolumeMtx;
};

#endif /* _SPOTIFYBACKEND_H_ */
