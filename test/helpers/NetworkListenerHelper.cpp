#include "NetworkListenerHelper.h"

#include <gtest/gtest.h>

#include "json/json.hpp"

using namespace std;
using json = nlohmann::json;

void testGoodGenerateSession(RestAPIFixture *fixture,
                             MockNetworkListener &listener,
                             TSessionID const &sid,
                             optional<TPassword> const &expPw,
                             optional<string> const &expNickname,
                             size_t count) {
  json requestBody;
  json expResponseBody;
  RestClient::Response resp;

  optional<TPassword> pw;
  optional<string> nickname;

  // prepare json bodies
  requestBody = {};
  if (expPw.has_value()) {
    requestBody["password"] = expPw.value();
  }
  if (expNickname.has_value()) {
    requestBody["nickname"] = expNickname.value();
  }
  expResponseBody = {{"session_id", sid}};

  // do request
  listener.setResponseGenerateSession(sid);
  resp = fixture->post("/generateSession", requestBody.dump()).value();

  // check response
  ASSERT_EQ(resp.code, 200);
  ASSERT_EQ(json::parse(resp.body), expResponseBody);
  ASSERT_EQ(listener.getCountGenerateSession(), count);

  ASSERT_TRUE(listener.hasParametersGenerateSession());
  listener.getLastParametersGenerateSession(pw, nickname);
  ASSERT_FALSE(listener.hasParametersGenerateSession());

  ASSERT_EQ(pw, expPw);
  ASSERT_EQ(nickname, expNickname);
}
