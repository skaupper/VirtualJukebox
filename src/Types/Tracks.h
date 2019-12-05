/*****************************************************************************/
/**
 * @file    Tracks.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Definitions of all track related structs
 */
/*****************************************************************************/

#ifndef _TRACKS_H_
#define _TRACKS_H_

#include "GlobalTypes.h"

class BaseTrack {
 public:
  TTrackID trackId;
  std::string title;
  std::string album;
  std::string artist;
  unsigned duration;
  std::string iconUri;
  std::string addedBy;
};

struct QueuedTrack : public BaseTrack {
  int votes;
  TVote currentVote;
  uint64_t insertedAt;
};

struct PlaybackTrack : public BaseTrack {
  int progressMs;
  bool isPlaying;
};

#endif /* _TRACKS_H_ */
