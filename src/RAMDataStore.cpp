/** ----------------------------------------------------------------------------
 * @file    RAMDataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class RAMDataStore implementation
 * ---------------------------------------------------------------------------*/

#include "RAMDataStore.h"

#include "GlobalTypes.h"
#include "Result.h"

#include <algorithm>
#include <ctime>

using namespace std;

TResultOpt RAMDataStore::addUser(User user){
    // Exclusive Access to User List
    unique_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
    MyLock.lock();

    // check for existing user
    auto it = find(mUsers.begin(), mUsers.end(), user);
    if(it == mUsers.end()){
        // User is unique, insert it into vector
        mUsers.emplace_back(user);
    }
    else{
        return Error(ErrorCode::AlreadyExists, "User already exists");
    }
}

// doesnt remove votes taken by this user
TResult<User> RAMDataStore::removeUser(TSessionID ID){
    // Exclusive Access to User List
    unique_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
    MyLock.lock();

    // find user
    User user;
    user.SessionID = ID;
    auto it = find(mUsers.begin(), mUsers.end(), user);
    if(it == mUsers.end()){
        return Error(ErrorCode::DoesntExist, "User doesnt exist");
    }
    else{
        // delete User
        mUsers.erase(it);
    }
}

// remove expired sessions
TResult<std::vector<User>> RAMDataStore::checkSessionExpirations(){
    // Exclusive Access to User List
    unique_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
    MyLock.lock();

    time_t now = time(nullptr);
    // custom predicate
    auto checkExpired = [now](const User &user) { return (user.ExpirationDate < now); };
    // actually remove all expired sessions
    mUsers.erase(remove_if(mUsers.begin(), mUsers.end(), checkExpired), mUsers.end());
}


TResultOpt RAMDataStore::addTrack(Track track, QueueType q){
    // Exclusive Access to Song Queue
    unique_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
    MyLock.lock();

    // select Queue
    Queue *pQueue = SelectQueue(q);
    if(pQueue == nullptr){return Error(ErrorCode::InvalidParam, "Invalid Parameter in Queue");}

   // check for existing Track
   auto it = find(pQueue->Tracks.begin(), pQueue->Tracks.end(), track);
   if(it == pQueue->Tracks.end()){
       // Track is unique, insert it into vector
       pQueue->Tracks.emplace_back(track);
   }
   else{
       return Error(ErrorCode::AlreadyExists, "Track already exists");
   }
}


TResult<Track> RAMDataStore::removeTrack(TTrackID ID, QueueType q){
    // Exclusive Access to Song Queue
    unique_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
    MyLock.lock();

    // select Queue
    Queue *pQueue = SelectQueue(q);
    if(pQueue == nullptr){return Error(ErrorCode::InvalidParam, "Invalid Parameter in Queue");}

    // delete Track
    Track track;
    track.TrackID = ID;
    auto it = find(pQueue->Tracks.begin(), pQueue->Tracks.end(), track);
    if(it == pQueue->Tracks.end()){
        return Error(ErrorCode::DoesntExist, "Track doesnt exist in this Queue");
    }
    else{
        // Track is there, delete it from vector
        pQueue->Tracks.emplace_back(track);
    }
}


TResult<bool> RAMDataStore::hasTrack(TTrackID ID, QueueType q){
    // Shared Access to Song Queue
    shared_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
    MyLock.lock();

    // select Queue
    Queue *pQueue = SelectQueue(q);
    if(pQueue == nullptr){return Error(ErrorCode::InvalidParam, "Invalid Parameter in Queue");}

    // find Track
    Track track;
    track.TrackID = ID;
    auto it = find(pQueue->Tracks.begin(), pQueue->Tracks.end(), track);
    if(it == pQueue->Tracks.end()){
        return false;
    }
    else{
        return true;
    }
}


TResultOpt RAMDataStore::voteTrack(TSessionID sID, TTrackID tID, TVote vote){
    // Exclusive Access to Song Queue and User
    unique_lock<shared_mutex> MyLockQueue(mQueueMutex, defer_lock);
    unique_lock<shared_mutex> MyLockUser(mUserMutex, defer_lock);
    scoped_lock(MyLockQueue, MyLockUser);

    // find user
    User user;
    user.SessionID = sID;
    auto it = find(mUsers.begin(), mUsers.end(), user);
    if(it == mUsers.end()){
        return Error(ErrorCode::DoesntExist, "User doesnt exist");
    }
    else{
        auto it_track = find(it->votes.begin(), it->votes.end(), tID);
        if(it_track == it->votes.end()){
            // Track not found
            return Error(ErrorCode::DoesntExist, "Track doesnt exist");
        } else{
            // Track found, upvote by adding it to the vector of upvoted tracks
            it->votes.emplace_back(tID);
        }
    }
}


TResult<Queue> RAMDataStore::getQueue(QueueType q){
    // Shared Access to Song Queue
    shared_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
    MyLock.lock();

    // select Queue
    Queue *pQueue = SelectQueue(q);
    if(pQueue == nullptr){return Error(ErrorCode::InvalidParam, "Invalid Parameter in Queue");}

    // return read only access to Queue
    return (const Queue)(*pQueue);
}


TResult<Track> RAMDataStore::getPlayingTrack(){

}


TResult<bool> RAMDataStore::hasUser(TSessionID ID){
    // Shared Access to User List
    shared_lock<shared_mutex> MyLock(mUserMutex, defer_lock);
    MyLock.lock();

    // find user
    User user;
    user.SessionID = ID;
    auto it = find(mUsers.begin(), mUsers.end(), user);
    if(it == mUsers.end()){
        return false;
    }
    else{
        return true;
    }
}


TResultOpt RAMDataStore::nextTrack(){
    // Exclusive Access to Song Queue
    unique_lock<shared_mutex> MyLock(mQueueMutex, defer_lock);
    MyLock.lock();




}

Queue *RAMDataStore::SelectQueue(QueueType q) {
    if(q == QueueType::Admin){
        return &mAdminQueue;
    }
    else if(q == QueueType::Normal){
        return &mNormalQueue;
    }
    else{
        return nullptr;
    }
}

