/*****************************************************************************/
/**
 * @file    RestAPIFixture.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of a test fixture for the RestAPI tests
 */
/*****************************************************************************/

#ifndef _REST_API_FIXTURE_H_
#define _REST_API_FIXTURE_H_

#include <gtest/gtest.h>

#include <thread>

#include "MockNetworkListener.h"
#include "Network/RestAPI.h"
#include "restclient-cpp/restclient.h"

class RestAPIFixture : public ::testing::Test {
 public:
  std::optional<RestClient::Response> post(std::string const &endpoint,
                                           std::string const &body);
  MockNetworkListener listener;

 protected:
  void SetUp() override;
  void TearDown() override;

  RestAPI api;
  std::thread serverThread;
};

#endif /* _REST_API_FIXTURE_H_ */
