/*****************************************************************************/
/**
 * @file    MusicBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Interface for MusicBackends
 */
/*****************************************************************************/

#ifndef MUSICBACKEND_H_INCLUDED
#define MUSICBACKEND_H_INCLUDED

#include <memory>
#include <vector>

#include "GlobalTypes.h"
#include "PlaybackTrack.h"
#include "Queue.h"
#include "Result.h"
#include "Track.h"

class MusicBackend {
 public:
  virtual TResultOpt initBackend(void) = 0;
  virtual TResult<std::vector<Track>> queryTracks(std::string const &pattern,
                                                  size_t const num) = 0;
  virtual TResultOpt setPlayback(Track const &track) = 0;
  virtual TResult<PlaybackTrack> getCurrentPlayback(void) = 0;
  virtual TResultOpt pause(void) = 0;
  virtual TResultOpt play() = 0;
  virtual TResult<size_t> getVolume(void) = 0;
  virtual TResultOpt setVolume(size_t const percent) = 0;
};

#endif  // MUSICBACKEND_H_INCLUDED
