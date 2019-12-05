/** ----------------------------------------------------------------------------
 * @file    RAMDataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class RAMDataStore implementation
 * ---------------------------------------------------------------------------*/

#include "Datastore/RAMDataStore.h"

#include <algorithm>
#include <ctime>

#include "Types/GlobalTypes.h"
#include "Types/Result.h"

using namespace std;

TResultOpt RAMDataStore::addUser(User user) {
  // Exclusive Access to User List
  unique_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
  MyLock.lock();

  // check for existing user
  auto it = find(mUsers.begin(), mUsers.end(), user);
  if (it == mUsers.end()) {
    // User is unique, insert it into vector
    mUsers.emplace_back(user);
  } else {
    return Error(ErrorCode::AlreadyExists, "User already exists");
  }
}

// doesnt remove votes taken by this user
TResult<User> RAMDataStore::removeUser(TSessionID ID) {
  // Exclusive Access to User List
  unique_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
  MyLock.lock();

  // find user
  User user;
  user.SessionID = ID;
  auto it = find(mUsers.begin(), mUsers.end(), user);
  if (it == mUsers.end()) {
    return Error(ErrorCode::DoesntExist, "User doesnt exist");
  } else {
    // delete User
    mUsers.erase(it);
  }
}

// remove expired sessions
TResultOpt RAMDataStore::checkSessionExpirations() {
  // Exclusive Access to User List
  unique_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
  MyLock.lock();

  time_t now = time(nullptr);
  // custom predicate
  auto checkExpired = [now](const User &user) {
    return (user.ExpirationDate < now);
  };
  // actually remove all expired sessions
  mUsers.erase(remove_if(mUsers.begin(), mUsers.end(), checkExpired),
               mUsers.end());

  return nullopt;
}

TResultOpt RAMDataStore::addTrack(BaseTrack track, QueueType q) {
  // Exclusive Access to Song Queue
  unique_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
  MyLock.lock();

  // select Queue
  Queue *pQueue = SelectQueue(q);
  if (pQueue == nullptr) {
    return Error(ErrorCode::InvalidValue, "Invalid Parameter in Queue");
  }

  // check for existing Track
  QueuedTrack qtr;
  qtr.trackId = track.trackId;
  auto it = find(pQueue->tracks.begin(), pQueue->tracks.end(), qtr);
  if (it == pQueue->tracks.end()) {
    // Track is unique, insert it into vector
    qtr.title = track.title;
    qtr.album = track.album;
    qtr.artist = track.artist;
    qtr.duration = track.duration;
    qtr.iconUri = track.iconUri;
    qtr.addedBy = track.addedBy;
    pQueue->tracks.push_back(qtr);
  } else {
    return Error(ErrorCode::AlreadyExists, "Track already exists");
  }
}

TResult<BaseTrack> RAMDataStore::removeTrack(TTrackID ID, QueueType q) {
  // Exclusive Access to Song Queue
  unique_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
  MyLock.lock();

  // select Queue
  Queue *pQueue = SelectQueue(q);
  if (pQueue == nullptr) {
    return Error(ErrorCode::InvalidValue, "Invalid Parameter in Queue");
  }

  // delete Track
  QueuedTrack track;
  track.trackId = ID;
  auto it = find(pQueue->tracks.begin(), pQueue->tracks.end(), track);
  if (it == pQueue->tracks.end()) {
    return Error(ErrorCode::DoesntExist, "Track doesnt exist in this Queue");
  } else {
    // Track is there, delete it from vector
    pQueue->tracks.erase(it,it);
  }
}

TResult<bool> RAMDataStore::hasTrack(TTrackID ID, QueueType q) {
  // Shared Access to Song Queue
  shared_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
  MyLock.lock();

  // select Queue
  Queue *pQueue = SelectQueue(q);
  if (pQueue == nullptr) {
    return Error(ErrorCode::InvalidValue, "Invalid Parameter in Queue");
  }

  // find Track
  QueuedTrack track;
  track.trackId = ID;
  auto it = find(pQueue->tracks.begin(), pQueue->tracks.end(), track);
  if (it == pQueue->tracks.end()) {
    return false;
  } else {
    return true;
  }
}

TResultOpt RAMDataStore::voteTrack(TSessionID sID, TTrackID tID, TVote vote) {
  // Exclusive Access to Song Queue and User
  unique_lock<shared_mutex> MyLockQueue(mQueueMutex, defer_lock);
  unique_lock<shared_mutex> MyLockUser(mUserMutex, defer_lock);
  scoped_lock(MyLockQueue, MyLockUser);

  // find user
  User user;
  user.SessionID = sID;
  auto it = find(mUsers.begin(), mUsers.end(), user);
  if (it == mUsers.end()) {
      // User not found
    return Error(ErrorCode::DoesntExist, "User doesnt exist");
  } else {
      // User found, look for Track
    auto it_track = find(it->votes.begin(), it->votes.end(), tID);
    if (it_track == it->votes.end()) {
      // Track not found
      return Error(ErrorCode::DoesntExist, "Track doesnt exist");
    } else {
      // Track found
      if(vote){
          // we want to upvote it, so upvote by adding it to the vector of upvoted tracks
          it->votes.emplace_back(tID);
      }
      else{
          // we want to remove it from upvoted tracks, so remove it from vector of upvoted tracks
          it->votes.erase(it_track, it_track);
      }
    }
  }
}

TResult<Queue> RAMDataStore::getQueue(QueueType q) {
  // Shared Access to Song Queue
  shared_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
  MyLock.lock();

  // select Queue
  Queue *pQueue = SelectQueue(q);
  if (pQueue == nullptr) {
    return Error(ErrorCode::InvalidValue, "Invalid Parameter in Queue");
  }

  // return read only access to Queue
  return (const Queue)(*pQueue);
}

TResult<BaseTrack> RAMDataStore::getPlayingTrack() {
    // Shared Access to Song Queue
    shared_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
    MyLock.lock();

    return mCurrentTrack;
}

TResult<bool> RAMDataStore::hasUser(TSessionID ID) {
  // Shared Access to User List
  shared_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
  MyLock.lock();

  // find user
  User user;
  user.SessionID = ID;
  auto it = find(mUsers.begin(), mUsers.end(), user);
  if (it == mUsers.end()) {
    return false;
  } else {
    return true;
  }
}

TResultOpt RAMDataStore::nextTrack() {
  // Exclusive Access to Song Queue
  unique_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
  MyLock.lock();




  
}

Queue *RAMDataStore::SelectQueue(QueueType q) {
  if (q == QueueType::Admin) {
    return &mAdminQueue;
  } else if (q == QueueType::Normal) {
    return &mNormalQueue;
  } else {
    return nullptr;
  }
}
