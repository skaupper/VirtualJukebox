/** ----------------------------------------------------------------------------
 * @file    Playback.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class for handling Playback data
 * ---------------------------------------------------------------------------*/

#ifndef PLAYBACK_TRACK_H_INCLUDED
#define PLAYBACK_TRACK_H_INCLUDED

#include "Result.h"
#include "Track.h"

class PlaybackTrack{
public:
    PlaybackTrack(Track const &track, int progress = 0, bool playing=false);
    Track getTrack(void) const;
    int   getProgress(void) const;
    bool  isPlaying(void) const;

private:
    int mProgress = 0;
    bool mPlaying = false;
    Track mTrack;
};

#endif //PLAYBACK_H_INCLUDED
