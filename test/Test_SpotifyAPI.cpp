/**
 * @file    SpotifyAPITest.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Test Class for the SpotifyAPI Class
 * @details for correct testing a valid access token must be set
 * the functions getAccessToken and refreshAccessToken are excluded from the
 * tests, because they can not be tested without a user interaction, or other
 * secret data which shouldn't be pushed in public repos
 */

#include <gtest/gtest.h>

#include "Spotify/SpotifyAPI.h"
#include "json/json.hpp"

using namespace std;
using namespace SpotifyApi;

class SpotifyAPITest : public ::testing::Test {
 public:
  virtual void SetUp() {
    // check if access token is valid
    auto testRet = mAPI.search(mAccessToken, "linkinPark", QueryType::track);

    if (auto value = get_if<Error>(&testRet)) {
      value = value;
      // if we get an error here, the access token is not valid or returned some
      // other error, so we skip the test to ensure that the tests are not
      // failing
      GTEST_SKIP_(" no valid access token given");
    }
  }
  virtual void TearDown() {
  }

 protected:
  std::string mAccessToken =
      "BQDJ9IG9YnSLze9srPny-SqdKVm-"
      "Msuzg3bjvUP5eEpPSOPc9eXrU4q3ZNIEzZp8GSP5LdrFeCToOZeXzTyQv6rO80CwsLiH7EbO"
      "k3UT6AI9t5u_nCHpZ_ognzoVHeZ-"
      "gc9381uEc0on8PTjFWG5nqJZTOcBYYZqYAXk77oBh6Pbep0-"
      "4PcrFovjPDioceSgkkheXAh00KsU";
  SpotifyAPI mAPI;
  static std::string mPageString;
  static std::string mInvalidDeviceString;
};

TEST_F(SpotifyAPITest, GetAvailableDevicesInvalid) {
  // test with invalid access token
  auto devicesRet = mAPI.getAvailableDevices("aiiiInvalid");
  auto value = get<Error>(devicesRet);

  EXPECT_EQ(ErrorCode::SpotifyAccessDenied, value.getErrorCode());
  EXPECT_EQ("Invalid access token", value.getErrorMessage());
}

TEST_F(SpotifyAPITest, GetAvailableDevicesValid) {
  // test with invalid access token
  auto devicesRet = mAPI.getAvailableDevices(mAccessToken);
  if (auto value = get_if<Error>(&devicesRet)) {
    value = value;
    EXPECT_EQ(0, 1);
  } else {
    // devices can't get tested better, because the amount is user dependant
    auto devices = get<vector<Device>>(devicesRet);
    cout << "Number of Devices: " << devices.size() << endl;
    EXPECT_EQ(1, 1);
  }
}

TEST_F(SpotifyAPITest, GetCurrentPlaybackInvalid) {
  // test with invalid access token
  auto playbackRet = mAPI.getCurrentPlayback("aiiiInvalid");
  auto value = get<Error>(playbackRet);

  EXPECT_EQ(ErrorCode::SpotifyAccessDenied, value.getErrorCode());
  EXPECT_EQ("Invalid access token", value.getErrorMessage());
}

TEST_F(SpotifyAPITest, GetCurrentPlaybackValid) {
  // test with invalid access token
  auto playbackRet = mAPI.getCurrentPlayback(mAccessToken);
  if (auto value = get_if<Error>(&playbackRet)) {
    value = value;
    EXPECT_EQ(0, 1);
  } else {
    // playback can't get tested better, because the amount is user dependant
    auto playback = get<Playback>(playbackRet);
    EXPECT_EQ(1, 1);
  }
}

TEST_F(SpotifyAPITest, searchInvalidToken) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes = mAPI.search("aiiiInvalid", query, QueryType::track);
  auto value = get<Error>(searchRes);

  EXPECT_EQ(ErrorCode::SpotifyAccessDenied, value.getErrorCode());
  EXPECT_EQ("Invalid access token", value.getErrorMessage());
}

TEST_F(SpotifyAPITest, searchValidTracks) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes = mAPI.search(mAccessToken, query, QueryType::track, 3);
  if (auto value = get_if<Error>(&searchRes)) {
    value = value;
    EXPECT_EQ(0, 1);
  } else {
    auto page = get<SpotifyPaging>(searchRes);

    // page string copied from the online request

    nlohmann::json spotifyJson;
    try {
      spotifyJson = nlohmann::json::parse(mPageString);
    } catch (...) {
      cout << "Error in Test search, parsing must succeed" << endl;
      EXPECT_EQ(1, 0);
    }
    SpotifyPaging spotifyPage(spotifyJson);
    EXPECT_EQ(page.getTotal(), spotifyPage.getTotal());
    EXPECT_EQ(page.getPrevious(), spotifyPage.getPrevious());
    EXPECT_EQ(page.getLimit(), spotifyPage.getLimit());
    EXPECT_EQ(page.getHref(), spotifyPage.getHref());
    for (size_t i = 0; i < page.getTracks().size(); ++i) {
      EXPECT_EQ(page.getTracks()[i].getName(),
                spotifyPage.getTracks()[i].getName());
      EXPECT_EQ(page.getTracks()[i].getUri(),
                spotifyPage.getTracks()[i].getUri());
      EXPECT_EQ(page.getTracks()[i].getHref(),
                spotifyPage.getTracks()[i].getHref());
      EXPECT_EQ(page.getTracks()[i].getDuration(),
                spotifyPage.getTracks()[i].getDuration());
      EXPECT_EQ(page.getTracks()[i].getArtists().size(),
                spotifyPage.getTracks()[i].getArtists().size());
    }
    EXPECT_EQ(page.getArtists().size(), spotifyPage.getArtists().size());
  }
}

TEST_F(SpotifyAPITest, searchInvalidMarket) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes =
      mAPI.search(mAccessToken, query, QueryType::track, 3, 0, "AIII");
  if (auto value = get_if<Error>(&searchRes)) {
    EXPECT_EQ(value->getErrorCode(), ErrorCode::SpotifyBadRequest);
    EXPECT_EQ(value->getErrorMessage(), "Invalid market code");
  } else {
    // cant reach here..
    EXPECT_EQ(0, 1);
  }
}

TEST_F(SpotifyAPITest, searchInvalidLimit1) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes = mAPI.search(mAccessToken, query, QueryType::track, 51);
  if (auto value = get_if<Error>(&searchRes)) {
    EXPECT_EQ(value->getErrorCode(), ErrorCode::SpotifyBadRequest);
    EXPECT_EQ(value->getErrorMessage(), "Invalid limit");
  } else {
    // cant reach here..
    EXPECT_EQ(0, 1);
  }
}

TEST_F(SpotifyAPITest, searchInvalidLimit2) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes = mAPI.search(mAccessToken, query, QueryType::track, 0);
  if (auto value = get_if<Error>(&searchRes)) {
    EXPECT_EQ(value->getErrorCode(), ErrorCode::SpotifyBadRequest);
    EXPECT_EQ(value->getErrorMessage(),
              "Bad limit, limit must be larger than 0");
  } else {
    // cant reach here..
    EXPECT_EQ(0, 1);
  }
}

TEST_F(SpotifyAPITest, searchInvalidLimit3) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes = mAPI.search(mAccessToken, query, QueryType::track, -100);
  if (auto value = get_if<Error>(&searchRes)) {
    EXPECT_EQ(value->getErrorCode(), ErrorCode::SpotifyBadRequest);
    EXPECT_EQ(value->getErrorMessage(),
              "Bad limit, limit must be larger than 0");
  } else {
    // cant reach here..
    EXPECT_EQ(0, 1);
  }
}

TEST_F(SpotifyAPITest, searchInvalidOffset1) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes =
      mAPI.search(mAccessToken, query, QueryType::track, 10, 1000000);
  if (auto value = get_if<Error>(&searchRes)) {
    EXPECT_EQ(value->getErrorCode(), ErrorCode::SpotifyNotFound);
    EXPECT_EQ(value->getErrorMessage(), "Not found.");
  } else {
    // cant reach here..
    EXPECT_EQ(0, 1);
  }
}

TEST_F(SpotifyAPITest, searchInvalidOffset2) {
  string query = "Frank Sinatra";
  // test with invalid access token
  auto searchRes = mAPI.search(mAccessToken, query, QueryType::track, 10, -1);
  if (auto value = get_if<Error>(&searchRes)) {
    EXPECT_EQ(value->getErrorCode(), ErrorCode::SpotifyBadRequest);
    EXPECT_EQ(value->getErrorMessage(),
              "Bad offset, offset must be greater than or equal to 0");
  } else {
    // cant reach here..
    EXPECT_EQ(0, 1);
  }
}

TEST_F(SpotifyAPITest, setVolumeInvalidAccessToken) {
  // test with invalid access token
  auto volRes = mAPI.setVolume("AIII", 20);
  if (!volRes.has_value()) {
    // cant reach here
    EXPECT_EQ(0, 1);
  } else {
    EXPECT_EQ(ErrorCode::SpotifyAccessDenied, volRes.value().getErrorCode());
    EXPECT_EQ("Invalid access token", volRes.value().getErrorMessage());
  }
}

TEST_F(SpotifyAPITest, setVolumeInvalidDevice) {
  Device invalidDevice(nlohmann::json::parse(mInvalidDeviceString));

  auto volRes = mAPI.setVolume(mAccessToken, 20, invalidDevice);
  if (!volRes.has_value()) {
    // cant reach here
    EXPECT_EQ(0, 1);
  } else {
    EXPECT_EQ(ErrorCode::SpotifyNotFound, volRes.value().getErrorCode());
    EXPECT_EQ("Device not found", volRes.value().getErrorMessage());
  }
}

TEST_F(SpotifyAPITest, setVolumeTryOnDevice) {
  // check if there are devices available
  auto devicesRet = mAPI.getAvailableDevices(mAccessToken);
  if (auto value = get_if<Error>(&devicesRet)) {
    value = value;
    cout << "Error happend in getting devices.. can't happen!!" << endl;
    EXPECT_EQ(0, 1);
  } else {
    auto devices = get<vector<Device>>(devicesRet);
    if (devices.empty()) {
      // if devices are empty test set on no device
      auto volRes = mAPI.setVolume(mAccessToken, 20);
      if (!volRes.has_value()) {
        // cant reach here
        EXPECT_EQ(0, 1);
      } else {
        EXPECT_EQ(ErrorCode::SpotifyNotFound, volRes.value().getErrorCode());
        EXPECT_EQ("Player command failed: No active device found",
                  volRes.value().getErrorMessage());
      }
    } else {
      // device found, set volume on it
      auto volRes = mAPI.setVolume(mAccessToken, 100);
      if (volRes.has_value()) {
        // if we reach here there the only valid error is a
        // VOLUME_CONTROL_DISALLOW
        EXPECT_EQ(volRes.value().getErrorCode(), ErrorCode::SpotifyForbidden);
        EXPECT_EQ(volRes.value().getErrorMessage(),
                  "Player command failed: Cannot control device volume");
      } else {
        EXPECT_EQ(volRes, nullopt);
      }
    }
  }
}

TEST_F(SpotifyAPITest, playInvalidAccessToken) {
  // test with invalid access token
  auto playRes = mAPI.play("AIII");
  if (!playRes.has_value()) {
    // cant reach here
    EXPECT_EQ(0, 1);
  } else {
    EXPECT_EQ(ErrorCode::SpotifyAccessDenied, playRes.value().getErrorCode());
    EXPECT_EQ("Invalid access token", playRes.value().getErrorMessage());
  }
}

TEST_F(SpotifyAPITest, playInvalidDevice) {
  Device invalidDevice(nlohmann::json::parse(mInvalidDeviceString));

  auto playRes =
      mAPI.play(mAccessToken,
                vector<string>{"spotify:track:7HbGgElHEx4fpyzHYtfiox"},
                invalidDevice);
  if (!playRes.has_value()) {
    // cant reach here
    EXPECT_EQ(0, 1);
  } else {
    EXPECT_EQ(ErrorCode::SpotifyNotFound, playRes.value().getErrorCode());
    EXPECT_EQ("Device not found", playRes.value().getErrorMessage());
  }
}

TEST_F(SpotifyAPITest, playInvalidSong) {
  // test with invalid access token
  auto playRes = mAPI.play(mAccessToken, vector<string>{"invalidsong"});
  if (!playRes.has_value()) {
    // cant reach here
    EXPECT_EQ(0, 1);
  } else {
    EXPECT_EQ(ErrorCode::SpotifyBadRequest, playRes.value().getErrorCode());
    EXPECT_EQ(string("Invalid track uri: ") + string("invalidsong"),
              playRes.value().getErrorMessage());
  }
}

TEST_F(SpotifyAPITest, playTryValidSong) {
  // check if there are devices available
  auto devicesRet = mAPI.getAvailableDevices(mAccessToken);
  if (auto value = get_if<Error>(&devicesRet)) {
    value = value;
    cout << "Error happend in getting devices.. can't happen!!" << endl;
    EXPECT_EQ(0, 1);
  } else {
    auto devices = get<vector<Device>>(devicesRet);
    if (devices.empty()) {
      // if devices are empty test set on no device
      auto playRes = mAPI.play(
          mAccessToken, vector<string>{"spotify:track:7HbGgElHEx4fpyzHYtfiox"});
      if (!playRes.has_value()) {
        // cant reach here
        EXPECT_EQ(0, 1);
      } else {
        EXPECT_EQ(ErrorCode::SpotifyNotFound, playRes.value().getErrorCode());
        EXPECT_EQ("Player command failed: No active device found",
                  playRes.value().getErrorMessage());
      }
    } else {
      // device found, set volume on it
      auto playRes = mAPI.play(
          mAccessToken, vector<string>{"spotify:track:7HbGgElHEx4fpyzHYtfiox"});
      if (playRes.has_value()) {
        // if we reach here not possible
        EXPECT_EQ(0, 1);
      } else {
        EXPECT_EQ(playRes, nullopt);
      }
    }
  }
}

TEST_F(SpotifyAPITest, getTrackTest) {
  auto trackRes = mAPI.getTrack(mAccessToken, "7HbGgElHEx4fpyzHYtfiox");
  if (auto value = get_if<Error>(&trackRes)) {
    value = value;
    cout << "error, should not reach here.." << endl;
    EXPECT_EQ(0, 1);
  } else {
    auto track = get<Track>(trackRes);
    EXPECT_EQ(track.getName(), "Get Down");
    EXPECT_EQ(track.getUri(), "spotify:track:7HbGgElHEx4fpyzHYtfiox");
    EXPECT_EQ(track.getDuration(), 165937);
  }
}

// some strings for testing
string SpotifyAPITest::mInvalidDeviceString =
    "{\n"
    "\"id\": \"aiiiiiiiiiiiiiiiiiii\", \n"
    "\"is_active\": true,\n"
    "\"is_private_session\": false,\n"
    "\"is_restricted\": false,\n"
    "\"name\": \"SM-G903F\",\n"
    "\"type\": \"Smartphone\",\n"
    "\"volume_percent\": 100\n"
    "}";

string SpotifyAPITest::mPageString =
    "{\n"
    "  \"tracks\": {\n"
    "    \"href\": "
    "\"https://api.spotify.com/v1/"
    "search?query=Frank+Sinatra&type=track&market=AT&offset=0&limit=3\",\n"
    "    \"items\": [\n"
    "      {\n"
    "        \"album\": {\n"
    "          \"album_type\": \"album\",\n"
    "          \"artists\": [\n"
    "            {\n"
    "              \"external_urls\": {\n"
    "                \"spotify\": "
    "\"https://open.spotify.com/artist/1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "              },\n"
    "              \"href\": "
    "\"https://api.spotify.com/v1/artists/1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "              \"id\": \"1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "              \"name\": \"Frank Sinatra\",\n"
    "              \"type\": \"artist\",\n"
    "              \"uri\": \"spotify:artist:1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "            }\n"
    "          ],\n"
    "          \"external_urls\": {\n"
    "            \"spotify\": "
    "\"https://open.spotify.com/album/4XbPmVHP7EYBMoE7ZVjKCU\"\n"
    "          },\n"
    "          \"href\": "
    "\"https://api.spotify.com/v1/albums/4XbPmVHP7EYBMoE7ZVjKCU\",\n"
    "          \"id\": \"4XbPmVHP7EYBMoE7ZVjKCU\",\n"
    "          \"images\": [\n"
    "            {\n"
    "              \"height\": 640,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/6bd881d7275f0b443dc3778a0f960ef60eb1eb6c\",\n"
    "              \"width\": 640\n"
    "            },\n"
    "            {\n"
    "              \"height\": 300,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/f34f37b96a1b98b6eeb052a4617dcb7c01e7648b\",\n"
    "              \"width\": 300\n"
    "            },\n"
    "            {\n"
    "              \"height\": 64,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/bce16007ce535f041fd08623a099874e2f6558c3\",\n"
    "              \"width\": 64\n"
    "            }\n"
    "          ],\n"
    "          \"name\": \"Christmas Songs by Sinatra\",\n"
    "          \"release_date\": \"1994-11-08\",\n"
    "          \"release_date_precision\": \"day\",\n"
    "          \"total_tracks\": 14,\n"
    "          \"type\": \"album\",\n"
    "          \"uri\": \"spotify:album:4XbPmVHP7EYBMoE7ZVjKCU\"\n"
    "        },\n"
    "        \"artists\": [\n"
    "          {\n"
    "            \"external_urls\": {\n"
    "              \"spotify\": "
    "\"https://open.spotify.com/artist/1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "            },\n"
    "            \"href\": "
    "\"https://api.spotify.com/v1/artists/1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "            \"id\": \"1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "            \"name\": \"Frank Sinatra\",\n"
    "            \"type\": \"artist\",\n"
    "            \"uri\": \"spotify:artist:1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "          },\n"
    "          {\n"
    "            \"external_urls\": {\n"
    "              \"spotify\": "
    "\"https://open.spotify.com/artist/0JXiS2FrAg3wQYJHcmZdrc\"\n"
    "            },\n"
    "            \"href\": "
    "\"https://api.spotify.com/v1/artists/0JXiS2FrAg3wQYJHcmZdrc\",\n"
    "            \"id\": \"0JXiS2FrAg3wQYJHcmZdrc\",\n"
    "            \"name\": \"B. Swanson Quartet\",\n"
    "            \"type\": \"artist\",\n"
    "            \"uri\": \"spotify:artist:0JXiS2FrAg3wQYJHcmZdrc\"\n"
    "          }\n"
    "        ],\n"
    "        \"disc_number\": 1,\n"
    "        \"duration_ms\": 155453,\n"
    "        \"explicit\": false,\n"
    "        \"external_ids\": {\n"
    "          \"isrc\": \"USSM10018232\"\n"
    "        },\n"
    "        \"external_urls\": {\n"
    "          \"spotify\": "
    "\"https://open.spotify.com/track/4kKdvXD0ez7jp1296JmAts\"\n"
    "        },\n"
    "        \"href\": "
    "\"https://api.spotify.com/v1/tracks/4kKdvXD0ez7jp1296JmAts\",\n"
    "        \"id\": \"4kKdvXD0ez7jp1296JmAts\",\n"
    "        \"is_local\": false,\n"
    "        \"is_playable\": true,\n"
    "        \"name\": \"Let It Snow! Let It Snow! Let It Snow! (with The B. "
    "Swanson Quartet)\",\n"
    "        \"popularity\": 87,\n"
    "        \"preview_url\": "
    "\"https://p.scdn.co/mp3-preview/"
    "650dc288044b9f2c07a0838198538f94362aa3c4?cid="
    "774b29d4f13844c495f206cafdad9c86\",\n"
    "        \"track_number\": 10,\n"
    "        \"type\": \"track\",\n"
    "        \"uri\": \"spotify:track:4kKdvXD0ez7jp1296JmAts\"\n"
    "      },\n"
    "      {\n"
    "        \"album\": {\n"
    "          \"album_type\": \"album\",\n"
    "          \"artists\": [\n"
    "            {\n"
    "              \"external_urls\": {\n"
    "                \"spotify\": "
    "\"https://open.spotify.com/artist/1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "              },\n"
    "              \"href\": "
    "\"https://api.spotify.com/v1/artists/1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "              \"id\": \"1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "              \"name\": \"Frank Sinatra\",\n"
    "              \"type\": \"artist\",\n"
    "              \"uri\": \"spotify:artist:1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "            }\n"
    "          ],\n"
    "          \"external_urls\": {\n"
    "            \"spotify\": "
    "\"https://open.spotify.com/album/4XbPmVHP7EYBMoE7ZVjKCU\"\n"
    "          },\n"
    "          \"href\": "
    "\"https://api.spotify.com/v1/albums/4XbPmVHP7EYBMoE7ZVjKCU\",\n"
    "          \"id\": \"4XbPmVHP7EYBMoE7ZVjKCU\",\n"
    "          \"images\": [\n"
    "            {\n"
    "              \"height\": 640,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/6bd881d7275f0b443dc3778a0f960ef60eb1eb6c\",\n"
    "              \"width\": 640\n"
    "            },\n"
    "            {\n"
    "              \"height\": 300,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/f34f37b96a1b98b6eeb052a4617dcb7c01e7648b\",\n"
    "              \"width\": 300\n"
    "            },\n"
    "            {\n"
    "              \"height\": 64,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/bce16007ce535f041fd08623a099874e2f6558c3\",\n"
    "              \"width\": 64\n"
    "            }\n"
    "          ],\n"
    "          \"name\": \"Christmas Songs by Sinatra\",\n"
    "          \"release_date\": \"1994-11-08\",\n"
    "          \"release_date_precision\": \"day\",\n"
    "          \"total_tracks\": 14,\n"
    "          \"type\": \"album\",\n"
    "          \"uri\": \"spotify:album:4XbPmVHP7EYBMoE7ZVjKCU\"\n"
    "        },\n"
    "        \"artists\": [\n"
    "          {\n"
    "            \"external_urls\": {\n"
    "              \"spotify\": "
    "\"https://open.spotify.com/artist/1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "            },\n"
    "            \"href\": "
    "\"https://api.spotify.com/v1/artists/1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "            \"id\": \"1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "            \"name\": \"Frank Sinatra\",\n"
    "            \"type\": \"artist\",\n"
    "            \"uri\": \"spotify:artist:1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "          }\n"
    "        ],\n"
    "        \"disc_number\": 1,\n"
    "        \"duration_ms\": 154640,\n"
    "        \"explicit\": false,\n"
    "        \"external_ids\": {\n"
    "          \"isrc\": \"USSM10018231\"\n"
    "        },\n"
    "        \"external_urls\": {\n"
    "          \"spotify\": "
    "\"https://open.spotify.com/track/6YeDjSHCDmJKgU8foiaruL\"\n"
    "        },\n"
    "        \"href\": "
    "\"https://api.spotify.com/v1/tracks/6YeDjSHCDmJKgU8foiaruL\",\n"
    "        \"id\": \"6YeDjSHCDmJKgU8foiaruL\",\n"
    "        \"is_local\": false,\n"
    "        \"is_playable\": true,\n"
    "        \"name\": \"Santa Claus Is Comin' to Town\",\n"
    "        \"popularity\": 73,\n"
    "        \"preview_url\": "
    "\"https://p.scdn.co/mp3-preview/"
    "24eab8e3b906684f2c57a7832c61bf952fbfa5e7?cid="
    "774b29d4f13844c495f206cafdad9c86\",\n"
    "        \"track_number\": 9,\n"
    "        \"type\": \"track\",\n"
    "        \"uri\": \"spotify:track:6YeDjSHCDmJKgU8foiaruL\"\n"
    "      },\n"
    "      {\n"
    "        \"album\": {\n"
    "          \"album_type\": \"album\",\n"
    "          \"artists\": [\n"
    "            {\n"
    "              \"external_urls\": {\n"
    "                \"spotify\": "
    "\"https://open.spotify.com/artist/1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "              },\n"
    "              \"href\": "
    "\"https://api.spotify.com/v1/artists/1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "              \"id\": \"1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "              \"name\": \"Frank Sinatra\",\n"
    "              \"type\": \"artist\",\n"
    "              \"uri\": \"spotify:artist:1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "            }\n"
    "          ],\n"
    "          \"external_urls\": {\n"
    "            \"spotify\": "
    "\"https://open.spotify.com/album/0ny6mZMBrYSO0s8HAKbcVq\"\n"
    "          },\n"
    "          \"href\": "
    "\"https://api.spotify.com/v1/albums/0ny6mZMBrYSO0s8HAKbcVq\",\n"
    "          \"id\": \"0ny6mZMBrYSO0s8HAKbcVq\",\n"
    "          \"images\": [\n"
    "            {\n"
    "              \"height\": 640,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/85fa41d125172f8d484b3178423bb80ca6258b97\",\n"
    "              \"width\": 640\n"
    "            },\n"
    "            {\n"
    "              \"height\": 300,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/47c1edef7d25964ae73d9fb82256f77b093eceaf\",\n"
    "              \"width\": 300\n"
    "            },\n"
    "            {\n"
    "              \"height\": 64,\n"
    "              \"url\": "
    "\"https://i.scdn.co/image/d76a8b5f5ea0b3d2c8a01fd0395876f7ccff6aa7\",\n"
    "              \"width\": 64\n"
    "            }\n"
    "          ],\n"
    "          \"name\": \"A Jolly Christmas From Frank Sinatra\",\n"
    "          \"release_date\": \"1957-09\",\n"
    "          \"release_date_precision\": \"month\",\n"
    "          \"total_tracks\": 15,\n"
    "          \"type\": \"album\",\n"
    "          \"uri\": \"spotify:album:0ny6mZMBrYSO0s8HAKbcVq\"\n"
    "        },\n"
    "        \"artists\": [\n"
    "          {\n"
    "            \"external_urls\": {\n"
    "              \"spotify\": "
    "\"https://open.spotify.com/artist/1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "            },\n"
    "            \"href\": "
    "\"https://api.spotify.com/v1/artists/1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "            \"id\": \"1Mxqyy3pSjf8kZZL4QVxS0\",\n"
    "            \"name\": \"Frank Sinatra\",\n"
    "            \"type\": \"artist\",\n"
    "            \"uri\": \"spotify:artist:1Mxqyy3pSjf8kZZL4QVxS0\"\n"
    "          }\n"
    "        ],\n"
    "        \"disc_number\": 1,\n"
    "        \"duration_ms\": 120693,\n"
    "        \"explicit\": false,\n"
    "        \"external_ids\": {\n"
    "          \"isrc\": \"USCA29900545\"\n"
    "        },\n"
    "        \"external_urls\": {\n"
    "          \"spotify\": "
    "\"https://open.spotify.com/track/4HEOgBHRCExyYVeTyrXsnL\"\n"
    "        },\n"
    "        \"href\": "
    "\"https://api.spotify.com/v1/tracks/4HEOgBHRCExyYVeTyrXsnL\",\n"
    "        \"id\": \"4HEOgBHRCExyYVeTyrXsnL\",\n"
    "        \"is_local\": false,\n"
    "        \"is_playable\": true,\n"
    "        \"name\": \"Jingle Bells - Remastered 1999\",\n"
    "        \"popularity\": 70,\n"
    "        \"preview_url\": null,\n"
    "        \"track_number\": 1,\n"
    "        \"type\": \"track\",\n"
    "        \"uri\": \"spotify:track:4HEOgBHRCExyYVeTyrXsnL\"\n"
    "      }\n"
    "    ],\n"
    "    \"limit\": 3,\n"
    "    \"next\": "
    "\"https://api.spotify.com/v1/"
    "search?query=Frank+Sinatra&type=track&market=AT&offset=3&limit=3\",\n"
    "    \"offset\": 0,\n"
    "    \"previous\": null,\n"
    "    \"total\": 13764\n"
    "  }\n"
    "}";
