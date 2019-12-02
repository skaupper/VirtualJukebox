/*****************************************************************************/
/**
 * @file    Playback.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class for handling Playback data
 */
/*****************************************************************************/

#include "PlaybackTrack.h"

PlaybackTrack::PlaybackTrack(Track const &track, int progress, bool playing)
    : mTrack(track), mProgress(progress), mPlaying(playing) {
}

int PlaybackTrack::getProgress() const {
  return mProgress;
}

Track PlaybackTrack::getTrack() const {
  return mTrack;
}

bool PlaybackTrack::isPlaying() const {
  return mPlaying;
}
