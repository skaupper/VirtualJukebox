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
                             MockNetworkListener &listener,
                             TSessionID const &sid,
                             std::optional<TPassword> const &expPw,
                             std::optional<std::string> const &expNickname,
                             size_t count);

#endif /* _NETWORK_LISTENER_HELPER_H_ */
