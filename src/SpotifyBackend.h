/*****************************************************************************/
/**
 * @file    SpotifyBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class handles Music Playback with a Spotify Backend
 */
/*****************************************************************************/

#ifndef SPOTIFYBACKEND_H_INCLUDED
#define SPOTIFYBACKEND_H_INCLUDED

#include "GlobalTypes.h"
#include "MusicBackend.h"
#include "PlaybackTrack.h"
#include "Queue.h"
#include "Result.h"
#include "Track.h"

class SpotifyBackend : public MusicBackend {
 public:
  virtual TResultOpt initBackend(void) override;
  virtual TResult<std::vector<Track>> queryTracks(std::string const &pattern,
                                                  size_t const num) override;
  virtual TResultOpt setPlayback(Track const &track) override;
  virtual TResult<PlaybackTrack> getCurrentPlayback(void) override;
  virtual TResultOpt pause(void) override;
  virtual TResultOpt play() override;
  virtual TResult<size_t> getVolume(void) override;
  virtual TResultOpt setVolume(size_t const percent) override;
};

#endif  // SPOTIFYBACKEND_H_INCLUDED
