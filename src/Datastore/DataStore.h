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
  virtual ~DataStore() {
  }
  /**
   * @brief    Add a User to the internal List of registered Users
   * @param    User The User class to add
   * @return   TResultOpt possibly containing Error Message
   */
  virtual TResultOpt addUser(User const &user) = 0;
  /**
   * @brief    Get a User from the internal List of registered Users
   * @param    TSessionID The ID of the User we want to get
   * @return   TResult<User> containing either the requested user  or an
   * Error Message
   */
  virtual TResult<User> getUser(TSessionID const &ID) = 0;
  /**
   * @brief    Remove a User from the internal List of registered Users. Does
   * not remove votes taken by this user from the corresponding tracks
   * @param    ID ID number of the User to remove
   * @return   TResult<User> containing either the user that was removed or an
   * Error Message
   */
  virtual TResult<User> removeUser(TSessionID const &ID) = 0;
  /**
   * @brief    Check if user session is expired.
   * @param    ID Session ID of the user to check
   * @retval   Returns false if session is not expired yet. Returns an Error
   * object on session expiration or error.
   */
  virtual TResult<bool> isSessionExpired(TSessionID const &ID) = 0;
  /**
   * @brief    Add Track to one of the internal Queues
   * @param    BaseTrack The Track to add
   * @param   QueueType Identifier for determining which Queue the Track should
   * be added to
   * @return   TResultOpt possibly containing Error Message
   */
  virtual TResultOpt addTrack(BaseTrack const &track, QueueType q) = 0;
  /**
   * @brief    Remove Track from one of the internal Queues
   * @param    TTrackID The ID of the Track to remove
   * @param   QueueType Identifier for determining which Queue the Track should
   * be removed from
   * @return   TResult<BaseTrack> Containing either the removed track or an
   * Error Message
   */
  virtual TResult<BaseTrack> removeTrack(TTrackID const &ID, QueueType q) = 0;
  /**
   * @brief    Check for Track in one of the internal Queues
   * @param    TTrackID The ID of the Track to check for
   * @param    QueueType Identifier for determining which Queue should be
   * checked for the Track
   * @return   TResult<bool> Containing either a boolean answer or an Error
   * Message
   */
  virtual TResult<bool> hasTrack(TTrackID const &ID, QueueType q) = 0;
  /**
   * @brief    Upvote/remove Upvote from a track
   * @param    TSessionID The ID of the User who wants to vote
   * @param    TTrackID The ID of the Track to vote for
   * @param    TVote The Vote which determines whether a new upvote should be
   * made or an old one removed
   * @return   TResultOpt possibly containing an Error Message
   */
  virtual TResultOpt voteTrack(TSessionID const &sID,
                               TTrackID const &tID,
                               TVote vote) = 0;
  /**
   * @brief    Get entire Queue
   * @param    QueueType Identifier for determining which Queue should be
   * returned
   * @return   TResult<Queue> Containing either the requested Queue or an Error
   * Message
   */
  virtual TResult<Queue> getQueue(QueueType q) = 0;
  /**
   * @brief    Get the currently playing track.
   * @param    none
   * @return   TResult<QueuedTrack> Containing either the requested Track or an
   * Error Message
   */
  virtual TResult<QueuedTrack> getPlayingTrack() = 0;
  /**
   * @brief    Query the internal user list whether a certain user exists
   * @param    TSessionID The ID of the user to check for
   * @return   bool Containing either a boolean answer to the request
   * or an Error Message
   */
  virtual bool hasUser(TSessionID const &ID) = 0;
  /**
   * @brief    Play next Track in Queue
   * @param    none
   * @return   TResultOpt possibly containing an Error Message
   */
  virtual TResultOpt nextTrack() = 0;

  unsigned const cSessionTimeoutAfterSeconds = 3600;
};

#endif /* _DATASTORE_H_ */
