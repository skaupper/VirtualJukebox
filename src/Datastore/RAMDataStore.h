/*****************************************************************************/
/**
 * @file    RAMDataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class RAMDataStore definition
 */
/*****************************************************************************/

#ifndef _RAMDATASTORE_H_
#define _RAMDATASTORE_H_

#include <mutex>
#include <shared_mutex>
#include <string>
#include <variant>
#include <vector>

#include "DataStore.h"
#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"
#include "Types/Tracks.h"
#include "Types/User.h"

/**
 * @brief Implements a DataStore which stores its data purly in RAM (no
 * persistance).
 */
class RAMDataStore : public DataStore {
 public:
  TResultOpt addUser(User const &user) override;
  TResult<User> getUser(TSessionID const &ID) override;
  TResult<User> removeUser(TSessionID const &ID) override;
  TResult<bool> isSessionExpired(TSessionID const &ID) override;
  TResultOpt addTrack(BaseTrack const &track, QueueType q) override;
  TResult<BaseTrack> removeTrack(TTrackID const &ID, QueueType q) override;
  TResult<bool> hasTrack(TTrackID const &ID, QueueType q) override;
  TResultOpt voteTrack(TSessionID const &sID,
                       TTrackID const &tID,
                       TVote vote) override;
  TResult<Queue> getQueue(QueueType q) override;
  TResult<std::optional<QueuedTrack>> getPlayingTrack() override;
  bool hasUser(TSessionID const &ID) override;
  TResultOpt nextTrack() override;

 private:
  void removeVotesForTrack(TTrackID const &);
  Queue *SelectQueue(QueueType q);

  Queue mAdminQueue;
  Queue mNormalQueue;
  std::optional<QueuedTrack> mCurrentTrack = std::nullopt;
  std::vector<User> mUsers;
  std::recursive_mutex mUserMutex;
  std::shared_mutex mQueueMutex;
};

#endif /* _RAMDATASTORE_H_ */
