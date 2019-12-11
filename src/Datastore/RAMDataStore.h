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
#include "Types/User.h"

/*
 * TODO:
 * (1) Overload DataStore.removeTrack function.
 *     DataStore.removeTrack(TTrackID const &ID, QueueType q);
 *        --> removes the given Track from the given queue
 *     DataStore.removeTrack(TTrackID const &ID);
 *        --> removes the given Track, from any queue (performs a search, like
 *            in Jukebox.removeTrack)
 * (2) Avoid having the same track in both queues
 *     --> a single track can only exist once
 */

class RAMDataStore : public DataStore {
 public:
  TResultOpt addUser(User const &user);
  TResult<User> getUser(TSessionID const &ID);
  TResult<User> removeUser(TSessionID const &ID);
  TResultOpt checkSessionExpirations();
  TResultOpt addTrack(BaseTrack const &track, QueueType q);
  TResult<BaseTrack> removeTrack(TTrackID const &ID, QueueType q);
  TResult<bool> hasTrack(TTrackID const &ID, QueueType q);
  TResultOpt voteTrack(TSessionID const &sID, TTrackID const &tID, TVote vote);
  TResult<Queue> getQueue(QueueType q);
  TResult<QueuedTrack> getPlayingTrack();
  TResult<bool> hasUser(TSessionID const &ID);
  TResultOpt nextTrack();

 private:
  Queue *SelectQueue(QueueType q);
  Queue mAdminQueue;
  Queue mNormalQueue;
  QueuedTrack mCurrentTrack;
  std::vector<User> mUsers;
  std::shared_mutex mUserMutex;
  std::shared_mutex mQueueMutex;
};

#endif /* _RAMDATASTORE_H_ */
