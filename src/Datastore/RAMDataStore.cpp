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
  return nullopt;
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
    // copy user for return type
    user = *it;
    // delete User
    mUsers.erase(it);
    return user;
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
    qtr.votes = 0;
    qtr.insertedAt = time(nullptr);
    qtr.LastPlayedxSongsAgo =
        1;  // set to one so that votes of songs count immediately after
            // insertion. See nextTrack and VoteTrack and Tracks operator<
    pQueue->tracks.push_back(qtr);
    return nullopt;
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
    track = *it;
    // Track is there, delete it from vector
    pQueue->tracks.erase(it, it);
    return track;
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
  }

  // find track in Queues
  QueuedTrack track;
  track.trackId = tID;
  QueuedTrack *pAdminTrack = 0;
  QueuedTrack *pNormalTrack = 0;
  auto it_admin =
      find(mAdminQueue.tracks.begin(), mAdminQueue.tracks.end(), track);
  if (it_admin != mAdminQueue.tracks.end()) {
    pAdminTrack = &(*it_admin);
  }
  auto it_normal =
      find(mNormalQueue.tracks.begin(), mNormalQueue.tracks.end(), track);
  if (it_normal != mNormalQueue.tracks.end()) {
    pNormalTrack = &(*it_normal);
  }

  // User found, look for Track in vote vector
  auto it_track = find(it->votes.begin(), it->votes.end(), tID);
  if (it_track != it->votes.end()) {
    // Track already found in vote vector
    if (vote) {
      // track already in vote vector and we want to upvote it: this is a
      // duplicate, do nothing
    } else {
      // Track already in vote vector and we want to remove the upvote:
      // we want to remove it from upvoted tracks, so remove it from vector of
      // upvoted tracks and update vote counter in track
      it->votes.erase(it_track);
      // decrement its upvote counter
      if (pAdminTrack != nullptr) {
        pAdminTrack->votes--;
      }
      if (pNormalTrack != nullptr) {
        pNormalTrack->votes--;
      }
    }
  } else {
    // Track not in vote vector
    if (vote) {
      // Track not in vote vector and we want to upvote it: add to vector and
      // update counter
      it->votes.emplace_back(tID);
      // increment its upvote counter
      if (pAdminTrack != nullptr) {
        pAdminTrack->votes++;
      }
      if (pNormalTrack != nullptr) {
        pNormalTrack->votes++;
      }
    } else {
      // track not in vote vector and we want to remove upvote: cant remove
      // nonexistent upvote, so do nothing
    }
  }

  // sort Normal Queue
  sort(mNormalQueue.tracks.begin(), mNormalQueue.tracks.end());
  return nullopt;
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

TResult<PlaybackTrack> RAMDataStore::getPlayingTrack() {
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

  // Increment LastPlayed counter for all songs

  // doesnt work with auto for some reason
  //  for(auto elem : mAdminQueue.tracks){
  //      elem.LastPlayedxSongsAgo += 1;
  //  }
  //    for(auto elem : mNormalQueue.tracks){
  //        elem.LastPlayedxSongsAgo += 1;
  //    }
  for (int i = 0; i < mAdminQueue.tracks.size(); ++i) {
    mAdminQueue.tracks[i].LastPlayedxSongsAgo += 1;
  }
  for (int i = 0; i < mNormalQueue.tracks.size(); ++i) {
    mNormalQueue.tracks[i].LastPlayedxSongsAgo += 1;
  }

  QueuedTrack tr;

  // If there are songs in the Admin Queue, play the first of those and move it
  // to the user queue
  if (mAdminQueue.tracks.size()) {
    tr = mAdminQueue.tracks[0];

    // move track from Admin Queue to user Queue if it doesnt already exist
    // there and delete it from Admin Queue.
    auto it = find(mNormalQueue.tracks.begin(), mNormalQueue.tracks.end(), tr);
    if (it == mNormalQueue.tracks.end()) {
      tr.LastPlayedxSongsAgo = 0;
      mNormalQueue.tracks.push_back(tr);
    } else {
      it->LastPlayedxSongsAgo = 0;
    }
    mAdminQueue.tracks.erase(mAdminQueue.tracks.begin());
  } else {
    // no songs in the admin queue, use the first one from the user queue
    tr = mNormalQueue.tracks[0];
    mNormalQueue.tracks[0].LastPlayedxSongsAgo = 0;
  }

  // sort Normal Queue
  sort(mNormalQueue.tracks.begin(), mNormalQueue.tracks.end());

  // Set Current Track
  mCurrentTrack.trackId = tr.trackId;
  mCurrentTrack.title = tr.title;
  mCurrentTrack.album = tr.album;
  mCurrentTrack.artist = tr.artist;
  mCurrentTrack.duration = tr.duration;
  mCurrentTrack.iconUri = tr.iconUri;
  mCurrentTrack.addedBy = tr.addedBy;
  mCurrentTrack.progressMs = 0;
  mCurrentTrack.isPlaying = true;

  return nullopt;
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
