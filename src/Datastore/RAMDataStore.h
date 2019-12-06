/** ----------------------------------------------------------------------------
 * @file    RAMDataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class RAMDataStore definition
 * ---------------------------------------------------------------------------*/

#ifndef _RAMDATASTORE_H_
#define _RAMDATASTORE_H_

#include <mutex>
#include <shared_mutex>
#include <string>
#include <variant>
#include <vector>

#include "Datastore/DataStore.h"
#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"
#include "Types/Tracks.h"
#include "User.h"

class RAMDataStore : public DataStore {
 public:
  TResultOpt addUser(User user);
  TResult<User> removeUser(TSessionID ID);
  TResultOpt checkSessionExpirations();
  TResultOpt addTrack(BaseTrack track, QueueType q);
  TResult<BaseTrack> removeTrack(TTrackID ID, QueueType q);
  TResult<bool> hasTrack(TTrackID ID, QueueType q);
  TResultOpt voteTrack(TSessionID sID, TTrackID tID, TVote vote);
  TResult<Queue> getQueue(QueueType q);
  TResult<PlaybackTrack> getPlayingTrack();
  TResult<bool> hasUser(TSessionID ID);
  TResultOpt nextTrack();

 private:
  Queue* SelectQueue(QueueType q);
  Queue mAdminQueue;
  Queue mNormalQueue;
  PlaybackTrack mCurrentTrack;
  std::vector<User> mUsers;
  std::shared_mutex mUserMutex;
  std::shared_mutex mQueueMutex;
};

#endif /* _RAMDATASTORE_H_ */
