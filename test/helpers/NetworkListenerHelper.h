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

void testGoodGenerateSession(RestAPIFixture *fixture,
                             TSessionID const &sid,
                             std::optional<TPassword> const &expPw,
                             std::optional<std::string> const &expNickname,
                             size_t count);

void testGoodQueryTracks(RestAPIFixture *fixture,
                         std::string const &pattern,
                         int maxEntries,
                         size_t count);

void testGoodGetCurrentQueues(RestAPIFixture *fixture,
                              TSessionID const &sid,
                              int normalNr,
                              int adminNr,
                              bool playbackTrack,
                              size_t count);

#endif /* _NETWORK_LISTENER_HELPER_H_ */
