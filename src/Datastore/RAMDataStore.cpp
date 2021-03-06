
/*****************************************************************************/
/**
 * @file    RAMDataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class RAMDataStore implementation
 */
/*****************************************************************************/

#include "Datastore/RAMDataStore.h"

#include <algorithm>
#include <ctime>

#include "Types/GlobalTypes.h"
#include "Types/Result.h"
#include "Utils/LoggingHandler.h"

using namespace std;

void RAMDataStore::removeVotesForTrack(TTrackID const &id) {
  unique_lock<recursive_mutex> MyUserLock(mUserMutex);
  for (auto &&user : mUsers) {
    auto it = find(user.votes.begin(), user.votes.end(), id);
    if (it != user.votes.end()) {
      user.votes.erase(it);
    }
  }
}

TResultOpt RAMDataStore::addUser(User const &user) {
  // Exclusive Access to User List
  unique_lock<recursive_mutex> MyLock(mUserMutex);

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

TResult<User> RAMDataStore::getUser(TSessionID const &ID) {
  // Exclusive Access to User List
  unique_lock<recursive_mutex> MyLock(mUserMutex);

  // find user
  User user;
  user.SessionID = ID;
  auto it = find(mUsers.begin(), mUsers.end(), user);
  if (it == mUsers.end()) {
    return Error(ErrorCode::DoesntExist, "User doesn't exist");
  } else {
    // copy user for return type
    user = *it;
    return user;
  }
}

// doesn't remove votes taken by this user
TResult<User> RAMDataStore::removeUser(TSessionID const &ID) {
  // Exclusive Access to User List
  unique_lock<recursive_mutex> MyLock(mUserMutex);

  // find user
  User user;
  user.SessionID = ID;
  auto it = find(mUsers.begin(), mUsers.end(), user);
  if (it == mUsers.end()) {
    return Error(ErrorCode::DoesntExist, "User doesn't exist");
  } else {
    // copy user for return type
    user = *it;
    // delete User
    mUsers.erase(it);
    return user;
  }
}

// check expired sessions
TResult<bool> RAMDataStore::isSessionExpired(TSessionID const &ID) {
  // Exclusive Access to User List
  unique_lock<recursive_mutex> MyLock(mUserMutex);

  auto retUser = getUser(ID);
  if (holds_alternative<Error>(retUser))
    return get<Error>(retUser);

  time_t now = time(nullptr);
  if (now < get<User>(retUser).ExpirationDate) {
    /* Session is not timed out. Advance expiration time, since user was active
     * right now. */
    get<User>(retUser).ExpirationDate = now + cSessionTimeoutAfterSeconds;
    return false;
  }

  string msg = "Session expired for user ID '" + ID + "'.";
  LOG(WARNING) << msg;
  return Error(ErrorCode::SessionExpired, msg);
}

TResultOpt RAMDataStore::addTrack(BaseTrack const &track, QueueType q) {
  // Exclusive Access to Song Queue
  unique_lock<shared_mutex> MyLock(mQueueMutex);

  // Get pointers for this Queue and for other Queue
  Queue *pThisQueue = SelectQueue(q);
  if (pThisQueue == nullptr) {
    return Error(ErrorCode::InvalidValue, "Invalid Parameter in Queue");
  }
  Queue *pOtherQueue = q == QueueType::Admin ? &mNormalQueue : &mAdminQueue;

  // check for existing Track in not selected Queue
  QueuedTrack qtr;
  qtr.trackId = track.trackId;
  auto it = find(pOtherQueue->tracks.begin(), pOtherQueue->tracks.end(), qtr);
  if (it != pOtherQueue->tracks.end()) {
    // This Track already exists in the other Queue, dont add it here
    return Error(ErrorCode::AlreadyExists,
                 "Track already exists in other Queue");
  }

  // check for existing Track in selected Queue
  qtr.trackId = track.trackId;
  it = find(pThisQueue->tracks.begin(), pThisQueue->tracks.end(), qtr);
  if (it == pThisQueue->tracks.end()) {
    // Track is unique, insert it into vector
    qtr.title = track.title;
    qtr.album = track.album;
    qtr.artist = track.artist;
    qtr.durationMs = track.durationMs;
    qtr.iconUri = track.iconUri;
    qtr.addedBy = track.addedBy;
    qtr.votes = 0;
    qtr.insertedAt = time(nullptr);
    pThisQueue->tracks.push_back(qtr);
    return nullopt;
  } else {
    return Error(ErrorCode::AlreadyExists, "Track already exists");
  }
}

TResult<BaseTrack> RAMDataStore::removeTrack(TTrackID const &ID, QueueType q) {
  QueuedTrack track;

  // remove track from queue
  {
    // Exclusive Access to Song Queue
    unique_lock<shared_mutex> MyLock(mQueueMutex);

    Queue *pQueue = SelectQueue(q);
    if (pQueue == nullptr) {
      return Error(ErrorCode::InvalidValue, "Invalid Parameter in SelectQueue");
    }

    // Deep copy track, then remove it
    track.trackId = ID;
    auto it = find(pQueue->tracks.begin(), pQueue->tracks.end(), track);
    if (it == pQueue->tracks.end()) {
      return Error(ErrorCode::DoesntExist, "Track doesn't exist in this Queue");
    } else {
      track = *it;
      // Found track, remove it from vector
      pQueue->tracks.erase(it);
    }
  }

  removeVotesForTrack(track.trackId);

  return track;
}

TResult<bool> RAMDataStore::hasTrack(TTrackID const &ID, QueueType q) {
  // Shared Access to Song Queue
  shared_lock<shared_mutex> MyLock(mQueueMutex);

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

TResultOpt RAMDataStore::voteTrack(TSessionID const &sID,
                                   TTrackID const &tID,
                                   TVote vote) {
  // Exclusive Access to Song Queue and User
  unique_lock<shared_mutex> MyLockQueue(mQueueMutex, defer_lock);
  unique_lock<recursive_mutex> MyLockUser(mUserMutex, defer_lock);
  lock(MyLockQueue, MyLockUser);

  // find user
  User user;
  user.SessionID = sID;
  auto it = find(mUsers.begin(), mUsers.end(), user);
  if (it == mUsers.end()) {
    // User not found
    return Error(ErrorCode::DoesntExist, "User doesn't exist");
  }

  // find track in Queues
  QueuedTrack track;
  track.trackId = tID;
  QueuedTrack *pNormalTrack = 0;
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
  shared_lock<shared_mutex> MyLock(mQueueMutex);

  // select Queue
  Queue *pQueue = SelectQueue(q);
  if (pQueue == nullptr) {
    return Error(ErrorCode::InvalidValue, "Invalid Parameter in Queue");
  }

  // return read only access to Queue
  return (const Queue)(*pQueue);
}

TResult<optional<QueuedTrack>> RAMDataStore::getPlayingTrack() {
  // Shared Access to Song Queue
  shared_lock<shared_mutex> MyLock(mQueueMutex);

  return mCurrentTrack;
}

bool RAMDataStore::hasUser(TSessionID const &ID) {
  // Shared Access to User List
  unique_lock<recursive_mutex> MyLock(mUserMutex);

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
  QueuedTrack track;

  {
    // Exclusive Access to Song Queue
    unique_lock<shared_mutex> MyLock(mQueueMutex);

    // If there are songs in the Admin Queue, play the first of those
    if (mAdminQueue.tracks.size()) {
      track = mAdminQueue.tracks[0];
      mAdminQueue.tracks.erase(mAdminQueue.tracks.begin());
    } else if (mNormalQueue.tracks.size() != 0) {
      // no songs in the admin queue, use the first one from the user queue
      track = mNormalQueue.tracks[0];
      mNormalQueue.tracks.erase(mNormalQueue.tracks.begin());
    } else {
      // no next track available
      return Error(ErrorCode::DoesntExist,
                   "No more Tracks available in either Queue");
    }

    // sort Normal Queue
    sort(mNormalQueue.tracks.begin(), mNormalQueue.tracks.end());

    // Set Current Track
    mCurrentTrack = track;
  }

  removeVotesForTrack(track.trackId);

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
