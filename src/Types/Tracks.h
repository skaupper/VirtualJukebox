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

class BaseTrack {
 public:
  TTrackID trackId;
  std::string title;
  std::string album;
  std::string artist;
  unsigned duration;
  std::string iconUri;
  std::string addedBy;
    bool operator==(const BaseTrack tr){
        return trackId == tr.trackId;
    }
};

struct QueuedTrack : public BaseTrack {
  int votes;
  TVote currentVote;
  uint64_t insertedAt;
  int LastPlayedxSongsAgo;
    bool operator<(const QueuedTrack tr){
        int lvotes = votes < 0 ? 1 : votes;
        int rvotes = tr.votes < 0 ? 1 : tr.votes;
        return (lvotes*LastPlayedxSongsAgo) > (rvotes*tr.LastPlayedxSongsAgo);
    }
};

struct PlaybackTrack : public BaseTrack {
  int progressMs;
  bool isPlaying;
};

#endif /* _TRACKS_H_ */
