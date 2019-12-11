/** ----------------------------------------------------------------------------
 * @file    DataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class DataStore definition
 * ---------------------------------------------------------------------------*/

#ifndef _DATASTORE_H_
#define _DATASTORE_H_

#include <string>
#include <variant>
#include <vector>

#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"
#include "Types/Tracks.h"
#include "Types/User.h"

/**
 * @class   DataStore
 * @brief   Interface for storing Data such as Tracks, Users votes, etc
 */
class DataStore {
 public:
  virtual TResultOpt addUser(User const &user) = 0;
  virtual TResult<User> getUser(TSessionID const &ID) = 0;
  // does not remove votes taken by this user
  virtual TResult<User> removeUser(TSessionID const &ID) = 0;
  virtual TResultOpt checkSessionExpirations() = 0;
  virtual TResultOpt addTrack(BaseTrack const &track, QueueType q) = 0;
  virtual TResult<BaseTrack> removeTrack(TTrackID const &ID) = 0;
  virtual TResult<bool> hasTrack(TTrackID const &ID, QueueType q) = 0;
  virtual TResultOpt voteTrack(TSessionID const &sID,
                               TTrackID const &tID,
                               TVote vote) = 0;
  virtual TResult<Queue> getQueue(QueueType q) = 0;
  // undefined behaviour before first call to nextTrack
  virtual TResult<QueuedTrack> getPlayingTrack() = 0;
  virtual TResult<bool> hasUser(TSessionID const &ID) = 0;
  virtual TResultOpt nextTrack() = 0;
};

#endif /* _DATASTORE_H_ */
