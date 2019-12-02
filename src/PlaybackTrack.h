/*****************************************************************************/
/**
 * @file    Playback.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class for handling Playback data
 */
/*****************************************************************************/

#ifndef _PLAYBACK_TRACK_H_
#define _PLAYBACK_TRACK_H_

#include "Result.h"
#include "Track.h"

class PlaybackTrack {
 public:
  PlaybackTrack(Track const &track, int progress = 0, bool playing = false);
  Track getTrack(void) const;
  int getProgress(void) const;
  bool isPlaying(void) const;

 private:
  int mProgress = 0;
  bool mPlaying = false;
  Track mTrack;
};

#endif /* _PLAYBACK_TRACK_H_ */
