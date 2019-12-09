/*****************************************************************************/
/**
 * @file    NetworkListenerHelper.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Defines a set of helper functions for the NetworkListener methods
 *          used in `Test_RestAPI.cpp`.
 */
/*****************************************************************************/

#ifndef _NETWORK_LISTENER_HELPER_H_
#define _NETWORK_LISTENER_HELPER_H_

#include "RestAPIFixture.h"

void testGenerateSession(RestAPIFixture *fixture,
                         TSessionID const &sid,
                         std::optional<TPassword> const &expPw,
                         std::optional<std::string> const &expNickname,
                         size_t count);

void testQueryTracks(RestAPIFixture *fixture,
                     std::string const &pattern,
                     int maxEntries,
                     size_t count);

void testGetCurrentQueues(RestAPIFixture *fixture,
                          TSessionID const &sid,
                          int normalNr,
                          int adminNr,
                          bool playbackTrack,
                          size_t count);

void testAddTrackToQueue(RestAPIFixture *fixture,
                         TSessionID const &expSid,
                         TTrackID const &expTrkid,
                         QueueType expQueueType,
                         int count);

void testVoteTrack(RestAPIFixture *fixture,
                   TSessionID const &expSid,
                   TTrackID const &expTrkid,
                   TVote vote,
                   int count);

void testControlPlayer(RestAPIFixture *fixture,
                       TSessionID const &expSid,
                       PlayerAction action,
                       int count);

void testMoveTrack(RestAPIFixture *fixture,
                   TSessionID const &expSid,
                   TTrackID const &expTrkid,
                   QueueType expQueueType,
                   int count);

void testRemoveTrack(RestAPIFixture *fixture,
                     TSessionID const &expSid,
                     TTrackID const &expTrkid,
                     int count);

#endif /* _NETWORK_LISTENER_HELPER_H_ */
