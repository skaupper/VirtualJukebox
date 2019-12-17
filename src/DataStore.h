/*****************************************************************************/
/**
 * @file    DataStore.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class DataStore definition
 */
/*****************************************************************************/

#ifndef _DATASTORE_H_
#define _DATASTORE_H_

#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"
#include "Types/Tracks.h"
#include "Types/User.h"

/**
 * @class   DataStore
 * @brief   Interface for storing Data such as Tracks, Users, Votes, etc.
 */
class DataStore {
 public:
  virtual ~DataStore() {
  }

  /**
   * @brief    Add a User to the internal List of registered Users
   * @param    user The User class to add
   * @return   An Error message or nothing at all (at success).
   */
  virtual TResultOpt addUser(User const &user) = 0;

  /**
   * @brief    Get a User from the internal List of registered Users
   * @param    sID The ID of the User we want to get
   * @return   Either the requested user or an Error message.
   */
  virtual TResult<User> getUser(TSessionID const &sID) = 0;

  /**
   * @brief    Remove a User from the internal List of registered Users. Does
   * not remove votes taken by this user from the corresponding tracks
   * @param    sID ID number of the User to remove
   * @return   Either the user that was removed or an Error message.
   */
  virtual TResult<User> removeUser(TSessionID const &sID) = 0;

  /**
   * @brief    Check if user session is expired.
   * @param    sID Session ID of the user to check
   * @retval   Returns false if session is not expired yet. Returns an Error
   * object on session expiration or error.
   */
  virtual TResult<bool> isSessionExpired(TSessionID const &sID) = 0;

  /**
   * @brief    Add Track to one of the internal Queues
   * @param    track The Track to add
   * @param    q Identifier for determining which Queue the Track should
   * be added to
   * @return   An Error message or nothing at all (at success).
   */
  virtual TResultOpt addTrack(BaseTrack const &track, QueueType q) = 0;

  /**
   * @brief    Remove Track from one of the internal Queues
   * @param    tID The ID of the Track to remove
   * @param    q Identifier for determining which Queue the Track should
   * be removed from
   * @return   Either the removed track or an Error message.
   */
  virtual TResult<BaseTrack> removeTrack(TTrackID const &tID, QueueType q) = 0;

  /**
   * @brief    Check for Track in one of the internal Queues
   * @param    tID The ID of the Track to check for
   * @param    q Identifier for determining which Queue should be
   * checked for the Track
   * @return   Either a boolean answer or an Error message.
   */
  virtual TResult<bool> hasTrack(TTrackID const &tID, QueueType q) = 0;

  /**
   * @brief    Upvote/remove Upvote from a track
   * @param    sID The ID of the User who wants to vote
   * @param    tID The ID of the Track to vote for
   * @param    vote The Vote which determines whether a new upvote should be
   * made or an old one removed
   * @return   An Error message or nothing at all (at success).
   */
  virtual TResultOpt voteTrack(TSessionID const &sID,
                               TTrackID const &tID,
                               TVote vote) = 0;

  /**
   * @brief    Get entire Queue
   * @param    q Identifier for determining which Queue should be
   * returned
   * @return   Either the requested Queue or an Error message.
   */
  virtual TResult<Queue> getQueue(QueueType q) = 0;

  /**
   * @brief    Get the currently playing track.
   * @return   Returns the currently playing track (if any) or an Error message.
   */
  virtual TResult<std::optional<QueuedTrack>> getPlayingTrack() = 0;

  /**
   * @brief    Query the internal user list whether a certain user exists
   * @param    sID The ID of the user to check for
   * @return   `true` if an user with this ID exists, `false` otherwise
   */
  virtual bool hasUser(TSessionID const &sID) = 0;

  /**
   * @brief    Play next Track in Queue
   * @return   An Error message or nothing at all (at success).
   */
  virtual TResultOpt nextTrack() = 0;

  static unsigned const cSessionTimeoutAfterSeconds = 3600;
};

#endif /* _DATASTORE_H_ */
