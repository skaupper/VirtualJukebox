/** ----------------------------------------------------------------------------
 * @file    Playback.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class for handling Playback data
 * ---------------------------------------------------------------------------*/


#include "Playback.h"

Playback::Playback(Track const &track, int progress, bool playing) :
  mTrack(track), mProgress(progress), mPlaying(playing) {}

int Playback::getProgress() const {
   return mProgress;
}

Track Playback::getTrack() const {
    return mTrack;
}

bool Playback::isPlaying() const {
    return mPlaying;
}