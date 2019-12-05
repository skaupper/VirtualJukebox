/*****************************************************************************/
/**
 * @file    NetworkListener.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Interface Class NetworkListener definition
 */
/*****************************************************************************/

#ifndef _NETWORKLISTENER_H_
#define _NETWORKLISTENER_H_

#include <string>
#include <variant>
#include <vector>

#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"

/**
 * @brief Provides interface methods for all supported requests.
 * @details The receiving part of the observer pattern.
 * @sa NetworkAPI
 */
class NetworkListener {
 public:
  /**
   * @brief Generate a session for an user.
   * @details A password may be provided to request admin privileges.
   * @param pw (optional) Password to authenticate the user as an admin.
   * @param nickname (optional) A human readable nickname for the user.
   * @return The created Session ID string on success, `Error` otherwise.
   */
  virtual TResult<TSessionID> generateSession(
      std::optional<TPassword> const &pw,
      std::optional<std::string> const &nickname) = 0;

  /**
   * @brief Query available tracks using different music backends.
   * @param searchPattern The search pattern used to query tracks from the
   * backend(s).
   * @param nrOfEntries Limits the number of returned track.
   * @return On success the a maximum of `nrOfEntries` tracks are returned, an
   * `Error` otherwise.
   */
  virtual TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &searchPattern, size_t const nrOfEntries) = 0;

  /**
   * @brief Query the content of the current queues.
   * @details Additionally to the current normal and admin queue, the currently
   * playing track is returned as well (if any).
   *
   * @return On success the current queues and the current track are returned,
   * an `Error` otherwise.
   */
  virtual TResult<QueueStatus> getCurrentQueues(TSessionID const &) = 0;

  /**
   * @brief Add a track to a given queue (normal or admin).
   * @details Depending on the value of `type` a track is added to either the
   * normal or the admin queue. If the track is not found, or the user has no
   * permission to add the track to the specified queue, an error is returned.
   *
   * @param sid The session ID of the user. Used to authenticate as admin if
   * needed.
   * @param trkid The ID of the track to add to a queue. Has to be queried using
   * `queryMusic` beforehand.
   * @param type Determines to which queue the track should be added.
   * @return Returns an `Error` if something went wrong.
   *
   * @note To add something to the admin queue, the user must have been
   * authenticated as admin when generating the session ID!
   */
  virtual TResultOpt addTrackToQueue(TSessionID const &sid,
                                     TTrackID const &trkid,
                                     QueueType type) = 0;

  /**
   * @brief Vote for a track or revoke a vote.
   * @details Depending on the value of `type` a track is added to either the
   * normal or the admin queue. If the track is not found, or the user has no
   * permission to add the track to the specified queue, an error is returned.
   *
   * @param sid The session ID of the user. Used to authenticate as admin if
   * needed.
   * @param trkid The ID of the track to add to a queue. Has to be queried using
   * `queryMusic` beforehand.
   * @param type Determines to which queue the track should be added.
   * @param vote Determines if a vote is set or revoked. There cannot be more
   * than 1 or less than 0 votes from the same user on the same track!
   *
   * @return Returns an `Error` if something went wrong.
   */
  virtual TResultOpt voteTrack(TSessionID const &sid,
                               TTrackID const &trkid,
                               TVote vote) = 0;

  /**
   * @brief Controls the behaviour of the music player.
   * @details Allows the admin to control some behaviour of the player. This
   * includes pausing/resuming/stopping the playback, skipping a track and
   * adjusting the volume by a fixed percentage at a time.
   *
   * @param sid The session ID of the user. Used to authenticate as admin.
   * @param action Specifies which action to take from the available ones listed
   * above.
   * @return Returns an `Error` if something went wrong.
   *
   * @note To control the player, the user must have been authenticated as admin
   * when generating the session ID!
   */
  virtual TResultOpt controlPlayer(TSessionID const &sid,
                                   PlayerAction action) = 0;

  /**
   * @brief Remove a track from a queue.
   * @details Can be used to remove a track from a queue. Since one track can
   * only be in one queue at a time, a queue type is not needed. This action is
   * only allowed for admins.
   *
   * @param sid The session ID of the user. Used to authenticate as admin.
   * @param trkid The track to be removed. Has to be queried using `queryMusic`
   * beforehand.
   * @return Returns an `Error` if something went wrong.
   *
   * @note To remove a track from any queue, the user must have been
   * authenticated as admin when generating the session ID!
   */
  virtual TResultOpt removeTrack(TSessionID const &sid,
                                 TTrackID const &trkid) = 0;

  /**
   * @brief Move a track between queues.
   * @details Can be used to move a track from one queue to another. If the
   * track is already in the specified queue, nothing happens. Otherwise the
   * track will be moved to the end of the queue and loses all queue specific
   * properties (i.e. votes). This action is only allowed for admins.
   *
   * @param sid The session ID of the user. Used to authenticate as admin.
   * @param trkid The track to be moved. Has to be queried using `queryMusic`
   * beforehand.
   * @param type Determines to which queue the track should be moved.
   * @return Returns an `Error` if something went wrong.
   *
   * @note To move a track from and to any queue, the user must have been
   * authenticated as admin when generating the session ID!
   */
  virtual TResultOpt moveTrack(TSessionID const &sid,
                               TTrackID const &trkid,
                               QueueType type) = 0;
};

#endif /* _NETWORKLISTENER_H_ */
