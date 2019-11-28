/** ----------------------------------------------------------------------------
 * @file    RAMDataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class RAMDataStore definition
 * ---------------------------------------------------------------------------*/

#ifndef _RAMDATASTORE_H_
#define _RAMDATASTORE_H_

#include <string>
#include <variant>
#include <vector>
#include <mutex>
#include <shared_mutex>

#include "GlobalTypes.h"
#include "Queue.h"
#include "Result.h"
#include "Track.h"
#include "User.h"
#include "DataStore.h"

class RAMDataStore : public DataStore{
public:
    TResultOpt addUser(User user);
    TResult<User> removeUser(TSessionID ID);
    TResult<std::vector<User>> checkSessionExpirations();
    TResultOpt addTrack(Track track, QueueType q);
    TResult<Track> removeTrack(TTrackID ID, QueueType q);
    TResult<bool> hasTrack(TTrackID ID, QueueType q);
    TResultOpt voteTrack(TSessionID sID, TTrackID tID, TVote vote);
    TResult<Queue> getQueue(QueueType q);
    TResult<Track> getPlayingTrack();
    TResult<bool> hasUser(TSessionID ID);
    TResultOpt nextTrack();
private:
    Queue * SelectQueue(QueueType q);
    Queue mAdminQueue;
    Queue mNormalQueue;
    Track mCurrentTrack;
    std::vector<User> mUsers;
    std::shared_mutex mUserMutex;
    std::shared_mutex mQueueMutex;
};

#endif /* _RAMDATASTORE_H_ */
