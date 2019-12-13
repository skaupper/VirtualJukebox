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
 * (1) Avoid having the same track in both queues
 *     --> a single track can only exist once
 */

class RAMDataStore : public DataStore {
 public:
  /**
   * @brief    Add a User to the internal List of registered Users
   * @param    User The User class to add
   * @retval   TResultOpt possibly containing Error Message
   */
  TResultOpt addUser(User const &user);
  /**
   * @brief    Get a User from the internal List of registered Users
   * @param    TSessionID The ID of the User we want to get
   * @retval   TResult<User> containing either the requested user  or an
   * Error Message
   */
  TResult<User> getUser(TSessionID const &ID);
  /**
   * @brief    Remove a User from the internal List of registered Users. Does
   * not remove votes taken by this user from the corresponding tracks
   * @param    ID ID number of the User to remove
   * @retval   TResult<User> containing either the user that was removed or an
   * Error Message
   */
  TResult<User> removeUser(TSessionID const &ID);
  /**
   * @brief    Check for users whose session has expired and remove those from
   * the internal list
   * @param    none
   * @retval   TResultOpt possibly containing Error Message
   */
  TResultOpt checkSessionExpirations();
  /**
   * @brief    Add Track to one of the internal Queues
   * @param    BaseTrack The Track to add
   * @param   QueueType Identifier for determining which Queue the Track should
   * be added to
   * @retval   TResultOpt possibly containing Error Message
   */
  TResultOpt addTrack(BaseTrack const &track, QueueType q);
  /**
   * @brief    Remove Track from one of the internal Queues
   * @param    TTrackID The ID of the Track to remove
   * @param   QueueType Identifier for determining which Queue the Track should
   * be removed from
   * @retval   TResult<BaseTrack> Containing either the removed track or an
   * Error Message
   */
  TResult<BaseTrack> removeTrack(TTrackID const &ID, QueueType q);
  /**
   * @brief    Check for Track in one of the internal Queues
   * @param    TTrackID The ID of the Track to check for
   * @param    QueueType Identifier for determining which Queue should be
   * checked for the Track
   * @retval   TResult<bool> Containing either a boolean answer or an Error
   * Message
   */
  TResult<bool> hasTrack(TTrackID const &ID, QueueType q);
  /**
   * @brief    Upvote/remove Upvote from a track
   * @param    TSessionID The ID of the User who wants to vote
   * @param    TTrackID The ID of the Track to vote for
   * @param    TVote The Vote which determines whether a new upvote should be
   * made or an old one removed
   * @retval   TResultOpt possibly containing an Error Message
   */
  TResultOpt voteTrack(TSessionID const &sID, TTrackID const &tID, TVote vote);
  /**
   * @brief    Get entire Queue
   * @param    QueueType Identifier for determining which Queue should be
   * returned
   * @retval   TResult<Queue> Containing either the requested Queue or an Error
   * Message
   */
  TResult<Queue> getQueue(QueueType q);
  /**
   * @brief    Get the currently playing track.
   * @param    none
   * @retval   TResult<QueuedTrack> Containing either the requested Track or an
   * Error Message
   */
  TResult<QueuedTrack> getPlayingTrack();
  /**
   * @brief    Query the internal user list whether a certain user exists
   * @param    TSessionID The ID of the user to check for
   * @retval   TResult<bool> Containing either a boolean answer to the request
   * or an Error Message
   */
  TResult<bool> hasUser(TSessionID const &ID);
  /**
   * @brief    Play next Track in Queue
   * @param    none
   * @retval   TResultOpt possibly containing an Error Message
   */
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
