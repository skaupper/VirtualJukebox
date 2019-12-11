/*****************************************************************************/
/**
 * @file    MusicBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Interface for MusicBackend
 */
/*****************************************************************************/

#ifndef _MUSICBACKEND_H_
#define _MUSICBACKEND_H_

#include <memory>
#include <vector>

#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"

class MusicBackend {
 public:
  virtual ~MusicBackend() {
  }
  virtual TResultOpt initBackend(void) = 0;
  virtual TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &pattern, size_t const num) = 0;
  virtual TResultOpt setPlayback(BaseTrack const &track) = 0;
  virtual TResult<PlaybackTrack> getCurrentPlayback(void) = 0;
  virtual TResultOpt pause(void) = 0;
  virtual TResultOpt play() = 0;
  virtual TResult<size_t> getVolume(void) = 0;
  virtual TResultOpt setVolume(size_t const percent) = 0;
  virtual TResult<BaseTrack> createBaseTrack(TTrackID const &trackID) = 0;
};

#endif /* _MUSICBACKEND_H_ */
