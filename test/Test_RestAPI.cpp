#include <gtest/gtest.h>

#include <iostream>
#include <memory>

#include "Network/RestAPI.h"
#include "NetworkListenerHelper.h"
#include "RestAPIFixture.h"
#include "json/json.hpp"
#include "restclient-cpp/restclient.h"

using namespace std;
using json = nlohmann::json;

/**
 * Test cases to implement:
 * - Call each endpoint and check if the parameters are parsed correctly
 * - Generate dummy data as responses and check if they are serialized properly
 *
 * For the test cases above a mock NetworkListener is needed who needs at least
 * following functionality:
 * - Provide the request body (+ querystring) of the last call.
 * - Provide all arguments of the last call.
 * - A method to set the response data for the next calls.
 */

TEST_F(RestAPIFixture, generateSession_goodCases) {
  ASSERT_FALSE(listener.hasParametersGenerateSession());
  ASSERT_EQ(listener.getCountGenerateSession(), 0);

  TSessionID sid;
  optional<TPassword> expPw;
  optional<string> expNickname;

  // Empty body
  sid = "test1";
  expPw = nullopt;
  expNickname = nullopt;
  testGoodGenerateSession(this, listener, sid, expPw, expNickname, 1);

  // Password set
  sid = "1234";
  expPw = "123 password 123";
  expNickname = nullopt;
  testGoodGenerateSession(this, listener, sid, expPw, expNickname, 2);

  // Nickname set
  sid = "1+987";
  expPw = nullopt;
  expNickname = "nicky1";
  testGoodGenerateSession(this, listener, sid, expPw, expNickname, 3);

  // Password+Nickname set (special characters)
  sid = "\"ß@§$%&/()=?`´";
  expPw = ".-!§@\"'";
  expNickname = "@€¶ŧ←↓→øþ";
  testGoodGenerateSession(this, listener, sid, expPw, expNickname, 4);
}

TEST_F(RestAPIFixture, generateSession_badCases) {
  ASSERT_FALSE(listener.hasParametersGenerateSession());
  ASSERT_EQ(listener.getCountGenerateSession(), 0);

  json requestBody;
  optional<TPassword> pw;
  optional<string> nickname;
  RestClient::Response resp;

  // No JSON body
  resp = this->post("/generateSession", "password=1234").value();
  ASSERT_EQ(resp.code, 422);
  ASSERT_EQ(listener.getCountGenerateSession(), 0);
  ASSERT_FALSE(listener.hasParametersGenerateSession());

  // Integer password
  resp = this->post("/generateSession", "{\"password\":1234}").value();
  ASSERT_EQ(resp.code, 422);
  ASSERT_EQ(listener.getCountGenerateSession(), 0);
  ASSERT_FALSE(listener.hasParametersGenerateSession());

  // Integer nickname
  resp = this->post("/generateSession", "{\"nickname\":1234}").value();
  ASSERT_EQ(resp.code, 422);
  ASSERT_EQ(listener.getCountGenerateSession(), 0);
  ASSERT_FALSE(listener.hasParametersGenerateSession());

  // Password typo
  // request is successful but the field "password" is not found
  resp =
      this->post("/generateSession", "{\"pasword\":\"typo in key\"}").value();
  ASSERT_EQ(resp.code, 200);
  ASSERT_EQ(listener.getCountGenerateSession(), 1);
  ASSERT_TRUE(listener.hasParametersGenerateSession());
  listener.getLastParametersGenerateSession(pw, nickname);
  ASSERT_FALSE(pw.has_value());
  ASSERT_FALSE(nickname.has_value());

  // Nickname typo
  // request is successful but the field "nickname" is not found
  resp =
      this->post("/generateSession", "{\"nicknam\":\"typo in key\"}").value();
  ASSERT_EQ(resp.code, 200);
  ASSERT_EQ(listener.getCountGenerateSession(), 2);
  ASSERT_TRUE(listener.hasParametersGenerateSession());
  listener.getLastParametersGenerateSession(pw, nickname);
  ASSERT_FALSE(pw.has_value());
  ASSERT_FALSE(nickname.has_value());
}
