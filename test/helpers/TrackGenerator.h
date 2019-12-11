/*****************************************************************************/
/**
 * @file    TrackGenerator.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of a TrackGenerator
 */
/*****************************************************************************/

#ifndef _TRACK_GENERATOR_H_
#define _TRACK_GENERATOR_H_

#include <vector>

#include "Types/Queue.h"
#include "Types/Tracks.h"

class TrackGenerator {
 public:
  std::vector<BaseTrack> generateTracks(int nr) const;
  std::vector<QueuedTrack> generateQueuedTracks(int nr) const;
  PlaybackTrack generatePlaybackTrack() const;
  QueueStatus generateQueueStatus(int normalNr,
                                  int adminNr,
                                  bool playback) const;
};

#endif /* _TRACK_GENERATOR_H_ */
