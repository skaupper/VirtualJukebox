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
  testGenerateSession(this, sid, expPw, expNickname, 1);

  // Password set
  sid = "1234";
  expPw = "123 password 123";
  expNickname = nullopt;
  testGenerateSession(this, sid, expPw, expNickname, 2);

  // Nickname set
  sid = "1+987";
  expPw = nullopt;
  expNickname = "nicky1";
  testGenerateSession(this, sid, expPw, expNickname, 3);

  // Password+Nickname set (special characters)
  sid = "\"ß@§$%&/()=?`´";
  expPw = ".-!§@\"'";
  expNickname = "@€¶ŧ←↓→øþ";
  testGenerateSession(this, sid, expPw, expNickname, 4);
}

TEST_F(RestAPIFixture, generateSession_badCases) {
  ASSERT_FALSE(listener.hasParametersGenerateSession());
  ASSERT_EQ(listener.getCountGenerateSession(), 0);

  json requestBody;
  optional<TPassword> pw;
  optional<string> nickname;
  RestClient::Response resp;

  // Wrong method
  resp = this->put("/generateSession", "empty").value();
  ASSERT_EQ(resp.code, 404);
  ASSERT_EQ(listener.getCountGenerateSession(), 0);
  ASSERT_FALSE(listener.hasParametersGenerateSession());

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
TEST_F(RestAPIFixture, queryTracks_goodCases) {
  ASSERT_FALSE(listener.hasParametersQueryTracks());
  ASSERT_EQ(listener.getCountQueryTracks(), 0);

  string pattern;
  int maxEntries;

  // No entries
  pattern = "123";
  maxEntries = 0;
  testQueryTracks(this, pattern, maxEntries, 1);

  // A single entry
  pattern = "311";
  maxEntries = 1;
  testQueryTracks(this, pattern, maxEntries, 2);

  // Empty pattern
  pattern = "";
  maxEntries = 10;
  testQueryTracks(this, pattern, maxEntries, 3);

  // Normal case
  pattern = "test";
  maxEntries = 10;
  testQueryTracks(this, pattern, maxEntries, 4);

  // Many entries
  pattern = "pattern!\"@€¶ŧ←§%$§%";
  maxEntries = 100;
  testQueryTracks(this, pattern, maxEntries, 5);
}

//
// getCurrentQueues
//
TEST_F(RestAPIFixture, getCurrentQueues_goodCases) {
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
  testGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 1);

  // Single entry in normal queue
  sid = "1234";
  normalNr = 1;
  adminNr = 0;
  playbackTrack = false;
  testGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 2);

  // Single entry in admin queue
  sid = "aöskdlfaöslkjdföaslkjdf!\"@€¶ŧ←§%$§%";
  normalNr = 0;
  adminNr = 1;
  playbackTrack = false;
  testGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 3);

  // Playback track only
  sid = "asdf";
  normalNr = 0;
  adminNr = 0;
  playbackTrack = true;
  testGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 4);

  // Small queues
  sid = "test 133564 =(=)$%§'*Ü`´";
  normalNr = 5;
  adminNr = 10;
  playbackTrack = true;
  testGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 5);

  // Big queues
  sid = "test33564 =(=)$%§'*Ü`´";
  normalNr = 100;
  adminNr = 8;
  playbackTrack = false;
  testGetCurrentQueues(this, sid, normalNr, adminNr, playbackTrack, 6);
}

//
// addTrackToQueue
//
TEST_F(RestAPIFixture, addTrackToQueue_goodCases) {
  ASSERT_FALSE(listener.hasParametersAddTrackToQueue());
  ASSERT_EQ(listener.getCountAddTrackToQueue(), 0);

  TSessionID sid;
  TTrackID trkid;
  QueueType queueType;

  // Empty IDs, normal queue
  sid = "";
  trkid = "";
  queueType = QueueType::Normal;
  testAddTrackToQueue(this, sid, trkid, queueType, 1);

  // Empty IDs, admin queue
  sid = "";
  trkid = "";
  queueType = QueueType::Admin;
  testAddTrackToQueue(this, sid, trkid, queueType, 2);

  // Empty Track ID
  sid = "asdf";
  trkid = "";
  queueType = QueueType::Normal;
  testAddTrackToQueue(this, sid, trkid, queueType, 3);

  // Empty session ID
  sid = "";
  trkid = "testtrack1";
  queueType = QueueType::Admin;
  testAddTrackToQueue(this, sid, trkid, queueType, 4);

  // Random IDs, normal queue
  sid = "TESTSESSION!§$%/&%&()=??\\";
  trkid = "1234567@ł€¶ŧ¶ł¶þø↓←";
  queueType = QueueType::Normal;
  testAddTrackToQueue(this, sid, trkid, queueType, 5);
}

//
// voteTrack
//
TEST_F(RestAPIFixture, voteTrack_goodCases) {
  ASSERT_FALSE(listener.hasParametersVoteTrack());
  ASSERT_EQ(listener.getCountVoteTrack(), 0);

  TSessionID sid;
  TTrackID trkid;
  TVote vote;

  // Empty IDs
  sid = "";
  trkid = "";
  vote = false;
  testVoteTrack(this, sid, trkid, vote, 1);

  // Empty Track ID
  sid = "asdf";
  trkid = "";
  vote = true;
  testVoteTrack(this, sid, trkid, vote, 2);

  // Empty session ID
  sid = "";
  trkid = "testtrack1";
  vote = false;
  testVoteTrack(this, sid, trkid, vote, 3);

  // Random IDs
  sid = "TESTSESSION!§$%/&%&()=??\\";
  trkid = "1234567@ł€¶ŧ¶ł¶þø↓←";
  vote = true;
  testVoteTrack(this, sid, trkid, vote, 4);
}

//
// controlPlayer
//
TEST_F(RestAPIFixture, controlPlayer_goodCases) {
  ASSERT_FALSE(listener.hasParametersControlPlayer());
  ASSERT_EQ(listener.getCountControlPlayer(), 0);

  TSessionID sid;
  PlayerAction playerAction;

  // Empty ID, Pause
  sid = "";
  playerAction = PlayerAction::Pause;
  testControlPlayer(this, sid, playerAction, 1);

  // Play
  sid = "1234";
  playerAction = PlayerAction::Pause;
  testControlPlayer(this, sid, playerAction, 2);

  // Stop
  sid = "4321";
  playerAction = PlayerAction::Pause;
  testControlPlayer(this, sid, playerAction, 3);

  // Skip
  sid = "1";
  playerAction = PlayerAction::Pause;
  testControlPlayer(this, sid, playerAction, 4);

  // VolumeDown
  sid = "//()/==??€@€¶";
  playerAction = PlayerAction::VolumeDown;
  testControlPlayer(this, sid, playerAction, 5);

  // VolumeUp
  sid = ".-.-.--..-.-.-";
  playerAction = PlayerAction::VolumeUp;
  testControlPlayer(this, sid, playerAction, 6);
}

//
// moveTrack
//
TEST_F(RestAPIFixture, moveTrack_goodCases) {
  ASSERT_FALSE(listener.hasParametersMoveTrack());
  ASSERT_EQ(listener.getCountMoveTrack(), 0);

  TSessionID sid;
  TTrackID trkid;
  QueueType queueType;

  // Empty IDs, Pause
  sid = "";
  trkid = "";
  queueType = QueueType::Normal;
  testMoveTrack(this, sid, trkid, queueType, 1);

  // Empty session ID
  sid = "";
  trkid = "asdf";
  queueType = QueueType::Admin;
  testMoveTrack(this, sid, trkid, queueType, 2);

  // Empty track ID
  sid = "445";
  trkid = "";
  queueType = QueueType::Normal;
  testMoveTrack(this, sid, trkid, queueType, 3);

  // Random IDs
  sid = "uuuiipoiuUOUUOJÖLKŧøĸđ„·«”„“”";
  trkid = "¢„æðđŋħĸłĸ";
  queueType = QueueType::Admin;
  testMoveTrack(this, sid, trkid, queueType, 4);
}
