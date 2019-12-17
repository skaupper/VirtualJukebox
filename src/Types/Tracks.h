/*****************************************************************************/
/**
 * @file    Tracks.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Definitions of all track related structs
 */
/*****************************************************************************/

#ifndef _TRACKS_H_
#define _TRACKS_H_

#include "Types/GlobalTypes.h"

/**
 * @brief The most basic representation of a track.
 */
class BaseTrack {
 public:
  TTrackID trackId;
  std::string title;
  std::string album;
  std::string artist;
  unsigned durationMs;
  std::string iconUri;
  std::string addedBy;
  bool operator==(const BaseTrack &tr) {
    return trackId == tr.trackId;
  }
};

/**
 * @brief Specialized track which is found inside queues.
 */
struct QueuedTrack : public BaseTrack {
  int votes;
  TVote userHasVoted;
  uint64_t insertedAt;
  bool operator<(const QueuedTrack tr) {
    if (votes == tr.votes) {
      return insertedAt < tr.insertedAt;
    } else {
      return votes > tr.votes;
    }
  }
};

/**
 * @brief Specialized track representing the currently playing track.
 */
struct PlaybackTrack : public BaseTrack {
  int progressMs;
  bool isPlaying;
};

#endif /* _TRACKS_H_ */
