#include <gtest/gtest.h>

#include <iostream>
#include <memory>

#include "Network/RestAPI.h"
#include "NetworkListenerHelper.h"
#include "RestAPIFixture.h"
#include "Utils/Serializer.h"
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

//
// generateSession
//
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
  testGoodGenerateSession(this, sid, expPw, expNickname, 1);

  // Password set
  sid = "1234";
  expPw = "123 password 123";
  expNickname = nullopt;
  testGoodGenerateSession(this, sid, expPw, expNickname, 2);

  // Nickname set
  sid = "1+987";
  expPw = nullopt;
  expNickname = "nicky1";
  testGoodGenerateSession(this, sid, expPw, expNickname, 3);

  // Password+Nickname set (special characters)
  sid = "\"ß@§$%&/()=?`´";
  expPw = ".-!§@\"'";
  expNickname = "@€¶ŧ←↓→øþ";
  testGoodGenerateSession(this, sid, expPw, expNickname, 4);
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

//
// queryTracks
//
TEST_F(RestAPIFixture, queryTracks) {
  ASSERT_FALSE(listener.hasParametersQueryTracks());
  ASSERT_EQ(listener.getCountQueryTracks(), 0);

  string pattern;
  int maxEntries;

  // No entries
  pattern = "123";
  maxEntries = 0;
  testGoodQueryTracks(this, pattern, maxEntries, 1);

  // A single entry
  pattern = "311";
  maxEntries = 1;
  testGoodQueryTracks(this, pattern, maxEntries, 2);

  // Empty pattern
  pattern = "";
  maxEntries = 10;
  testGoodQueryTracks(this, pattern, maxEntries, 3);

  // Normal case
  pattern = "test";
  maxEntries = 10;
  testGoodQueryTracks(this, pattern, maxEntries, 4);

  // Many entries
  pattern = "pattern!\"@€¶ŧ←§%$§%";
  maxEntries = 100;
  testGoodQueryTracks(this, pattern, maxEntries, 5);
}

TEST_F(RestAPIFixture, getCurrentQueues) {
  ASSERT_FALSE(listener.hasParametersGetCurrentQueues());
  ASSERT_EQ(listener.getCountGetCurrentQueues(), 0);

  string sid;
  int normalNr;
  int adminNr;
  bool playbackTrack;

  // Empty queues
  sid = "";
  normalNr = 0;
  adminNr = 0;
  playbackTrack = false;
  testGoodGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 1);

  // Single entry in normal queue
  sid = "1234";
  normalNr = 1;
  adminNr = 0;
  playbackTrack = false;
  testGoodGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 2);

  // Single entry in admin queue
  sid = "aöskdlfaöslkjdföaslkjdf!\"@€¶ŧ←§%$§%";
  normalNr = 0;
  adminNr = 1;
  playbackTrack = false;
  testGoodGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 3);

  // Playback track only
  sid = "asdf";
  normalNr = 0;
  adminNr = 0;
  playbackTrack = true;
  testGoodGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 4);

  // Small queues
  sid = "test 133564 =(=)$%§'*Ü`´";
  normalNr = 5;
  adminNr = 10;
  playbackTrack = true;
  testGoodGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 5);

  // Big queues
  sid = "test33564 =(=)$%§'*Ü`´";
  normalNr = 100;
  adminNr = 8;
  playbackTrack = false;
  testGoodGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 6);
}
