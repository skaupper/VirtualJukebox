/*****************************************************************************/
/**
 * @file    RestEndpointHandlers.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Handler functions for all available REST endpoints
 */
/*****************************************************************************/

#include "RestEndpointHandlers.h"

#include "json/json.hpp"

using namespace std;
using namespace httpserver;
using json = nlohmann::json;

static TResult<json const> parseJsonString(string const &str) {
  try {
    return json::parse(str);
  } catch (json::parse_error const &) {
    return Error(ErrorCode::InvalidFormat, "Failed to parse body");
  }
}

static shared_ptr<http_response> const mapErrorToResponse(Error const &err) {
  static const map<ErrorCode, int> ERROR_TO_HTTP_STATUS = {
      {ErrorCode::AccessDenied, 403},  //
      {ErrorCode::InvalidFormat, 422}  //
  };

  int statusCode;
  string msg;

  // map internal error codes to HTTP status codes
  // unhandled ErrorCodes trigger an internal server error
  auto errorCode = err.getErrorCode();
  auto statusCodeIt = ERROR_TO_HTTP_STATUS.find(errorCode);
  if (statusCodeIt == ERROR_TO_HTTP_STATUS.cend()) {
    statusCode = 500;
    msg = "Unhandled error code detected! Original error message: " +
          err.getErrorMessage();
  } else {
    statusCode = statusCodeIt->second;
    msg = err.getErrorMessage();
  }

  // construct response
  json responseBody = {
      {"status", statusCode},  //
      {"error", msg}           //
  };
  return make_shared<string_response>(responseBody.dump(), statusCode);
}

//
// GENERATE SESSION
//

shared_ptr<http_response> const generateSessionHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);

  // parse body into JSON object
  auto parseResult = parseJsonString(infos.body);
  if (holds_alternative<Error>(parseResult)) {
    return mapErrorToResponse(get<Error>(parseResult));
  }
  json const bodyJson = get<json const>(parseResult);

  // parse request specific JSON fields
  optional<TPassword> password;
  if (bodyJson.find("password") != bodyJson.cend()) {
    auto passwordJson = bodyJson["password"];
    if (!passwordJson.is_string()) {
      return mapErrorToResponse(Error(ErrorCode::InvalidFormat,
                                      "Value of 'password' must be a string"));
    }
    password = passwordJson.get<string>();
  }

  optional<string> nickname;
  if (bodyJson.find("nickname") != bodyJson.cend()) {
    auto nicknameJson = bodyJson["nickname"];
    if (!nicknameJson.is_string()) {
      return mapErrorToResponse(Error(ErrorCode::InvalidFormat,
                                      "Value of 'nickname' must be a string"));
    }
    nickname = nicknameJson.get<string>();
  }

  // notify the listener about the request
  TResult<TSessionID> result = listener->generateSession(password, nickname);
  if (holds_alternative<Error>(result)) {
    return mapErrorToResponse(get<Error>(result));
  }

  // construct the response
  auto sessionId = get<TSessionID>(result);
  json responseBody = {
      {"session_id", static_cast<string>(sessionId)}  //
  };
  return make_shared<string_response>(responseBody.dump());
}

//
// QUERY TRACKS
//

shared_ptr<http_response> const queryTracksHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);

  if (infos.args.find("pattern") == infos.args.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Parameter 'pattern' not found"));
  }
  string pattern = infos.args.at("pattern");

  int maxEntries = 50;
  if (infos.args.find("max_entries") != infos.args.cend()) {
    auto maxEntriesStr = infos.args.at("max_entries");

    int tmpMaxEntries;
    size_t idx;
    try {
      tmpMaxEntries = stoi(maxEntriesStr, &idx);
    } catch (invalid_argument const &) {
      return mapErrorToResponse(
          Error(ErrorCode::InvalidFormat,
                "Parameter 'max_entries' is not an integer"));
    }

    if (idx != maxEntriesStr.size()) {
      return mapErrorToResponse(Error(
          ErrorCode::InvalidFormat,
          "Parameter 'max_entries' must not contain non-integer characters"));
    }

    maxEntries = tmpMaxEntries;
  }

  // notify the listener about the request
  TResult<vector<Track>> result = listener->queryTracks(pattern, maxEntries);
  if (holds_alternative<Error>(result)) {
    return mapErrorToResponse(get<Error>(result));
  }

  // construct the response
  json responseBody = {};
  json jsonTracks = {};

  for (auto &&track : get<vector<Track>>(result)) {
    // TODO: fill track and serialize it properly
    json jsonTrack{{"track_id", "dummyid"},
                   {"title", "Dummy Title"},
                   {"album", "Dummy Album"},
                   {"artist", "Du mmy Artist"},
                   {"duration", 123456},
                   {"icon_uri", "https://github.githubassets.com/favicon.ico"}};

    jsonTracks.push_back(jsonTracks);
  }
  responseBody["tracks"] = jsonTracks;

  return make_shared<string_response>(responseBody.dump());
}

//
// GET CURRENT QUEUES
//

shared_ptr<http_response> const getCurrentQueuesHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);

  if (infos.args.find("session_id") == infos.args.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Parameter 'session_id' not found"));
  }
  TSessionID sessionId = static_cast<TSessionID>(infos.args.at("session_id"));

  // notify the listener about the request
  // TODO: use the actual result type and serialize it
  //   auto result = listener->getCurrentQueues(sessionId);
  TResult<Queues> result =
      Error(ErrorCode::NotImplemented,
            "Endpoint 'getCurrentQueues' is not implemented yet");
  if (holds_alternative<Error>(result)) {
    return mapErrorToResponse(get<Error>(result));
  }

  // construct the response
  // TODO: use the actual result type and serialize it
  json responseBody = {};

  return make_shared<string_response>(responseBody.dump());
}

//
// ADD TRACK TO QUEUE
//

shared_ptr<http_response> const addTrackToQueueHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);  // parse body into JSON object

  auto parseResult = parseJsonString(infos.body);
  if (holds_alternative<Error>(parseResult)) {
    return mapErrorToResponse(get<Error>(parseResult));
  }
  json const bodyJson = get<json const>(parseResult);

  // parse request specific JSON fields
  if (bodyJson.find("session_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'session_id' not found"));
  }
  TSessionID sessionId = bodyJson["session_id"].get<TSessionID>();

  if (bodyJson.find("track_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'track_id' not found"));
  }
  TTrackID trackId = bodyJson["track_id"].get<TTrackID>();

  QueueType queueType = QueueType::Normal;
  if (bodyJson.find("queue_type") != bodyJson.cend()) {
    auto queueTypeJson = bodyJson["queue_type"];
    if (!queueTypeJson.is_string()) {
      return mapErrorToResponse(Error(
          ErrorCode::InvalidFormat, "Value of 'queue_type' must be a string"));
    }

    // TODO: do deserialization using the JSON framework
    auto queueTypeStr = queueTypeJson.get<string>();
    if (queueTypeStr == "admin") {
      queueType = QueueType::Admin;
    } else if (queueTypeStr == "normal") {
      queueType = QueueType::Normal;
    } else {
      return mapErrorToResponse(
          Error(ErrorCode::InvalidFormat,
                "Value of 'queue_type' must either be 'admin' or 'normal'"));
    }
  }

  // notify the listener about the request
  TResultOpt result = listener->addTrackToQueue(sessionId, trackId, queueType);
  if (result.has_value()) {
    return mapErrorToResponse(result.value());
  }

  // construct the response
  json responseBody = {};
  return make_shared<string_response>(responseBody.dump());
}

//
// VOTE TRACK
//

shared_ptr<http_response> const voteTrackHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);
  auto parseResult = parseJsonString(infos.body);
  if (holds_alternative<Error>(parseResult)) {
    return mapErrorToResponse(get<Error>(parseResult));
  }
  json const bodyJson = get<json const>(parseResult);

  // parse request specific JSON fields
  if (bodyJson.find("session_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'session_id' not found"));
  }
  TSessionID sessionId = bodyJson["session_id"].get<TSessionID>();

  if (bodyJson.find("track_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'track_id' not found"));
  }
  TTrackID trackId = bodyJson["track_id"].get<TTrackID>();

  TVote vote;
  if (bodyJson.find("vote") != bodyJson.cend()) {
    auto voteJson = bodyJson["vote"];
    if (!voteJson.is_number_integer()) {
      return mapErrorToResponse(Error(ErrorCode::InvalidFormat,
                                      "Value of 'vote' must be an integer"));
    }

    vote = (voteJson.get<int>() != 0);
  }

  // notify the listener about the request
  TResultOpt result = listener->voteTrack(sessionId, trackId, vote);
  if (result.has_value()) {
    return mapErrorToResponse(result.value());
  }

  // construct the response
  json responseBody = {};
  return make_shared<string_response>(responseBody.dump());
}

//
// CONTROL PLAYER
//

shared_ptr<http_response> const controlPlayerHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);
  auto parseResult = parseJsonString(infos.body);
  if (holds_alternative<Error>(parseResult)) {
    return mapErrorToResponse(get<Error>(parseResult));
  }
  json const bodyJson = get<json const>(parseResult);

  // parse request specific JSON fields
  if (bodyJson.find("session_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'session_id' not found"));
  }
  TSessionID sessionId = bodyJson["session_id"].get<TSessionID>();

  PlayerAction playerAction;
  if (bodyJson.find("player_action") != bodyJson.cend()) {
    auto playerActionJson = bodyJson["player_action"];
    if (!playerActionJson.is_string()) {
      return mapErrorToResponse(
          Error(ErrorCode::InvalidFormat,
                "Value of 'player_action' must be a string"));
    }

    // TODO: do deserialization using the JSON framework
    auto playerActionStr = playerActionJson.get<string>();
    if (playerActionStr == "play") {
      playerAction = PlayerAction::Play;
    } else if (playerActionStr == "pause") {
      playerAction = PlayerAction::Pause;
    } else if (playerActionStr == "stop") {
      playerAction = PlayerAction::Stop;
    } else if (playerActionStr == "skip") {
      playerAction = PlayerAction::Skip;
    } else if (playerActionStr == "volume_up") {
      playerAction = PlayerAction::VolumeUp;
    } else if (playerActionStr == "volume_down") {
      playerAction = PlayerAction::VolumeDown;
    } else {
      return mapErrorToResponse(
          Error(ErrorCode::InvalidFormat,
                "Value of 'player_action' must be a valid action."));
    }
  }

  // notify the listener about the request
  TResultOpt result = listener->controlPlayer(sessionId, playerAction);
  if (result.has_value()) {
    return mapErrorToResponse(result.value());
  }

  // construct the response
  json responseBody = {};
  return make_shared<string_response>(responseBody.dump());
}

//
// MOVE TRACK
//

shared_ptr<http_response> const moveTracksHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);
  auto parseResult = parseJsonString(infos.body);
  if (holds_alternative<Error>(parseResult)) {
    return mapErrorToResponse(get<Error>(parseResult));
  }
  json const bodyJson = get<json const>(parseResult);

  // parse request specific JSON fields
  if (bodyJson.find("session_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'session_id' not found"));
  }
  TSessionID sessionId = bodyJson["session_id"].get<TSessionID>();

  if (bodyJson.find("track_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'track_id' not found"));
  }
  TTrackID trackId = bodyJson["track_id"].get<TTrackID>();

  if (bodyJson.find("queue_type") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'queue_type' not found"));
  }
  auto queueTypeJson = bodyJson["queue_type"];
  if (!queueTypeJson.is_string()) {
    return mapErrorToResponse(Error(ErrorCode::InvalidFormat,
                                    "Value of 'queue_type' must be a string"));
  }

  // TODO: do deserialization using the JSON framework
  QueueType queueType;
  auto queueTypeStr = queueTypeJson.get<string>();
  if (queueTypeStr == "admin") {
    queueType = QueueType::Admin;
  } else if (queueTypeStr == "normal") {
    queueType = QueueType::Normal;
  } else {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat,
              "Value of 'queue_type' must either be 'admin' or 'normal'"));
  }

  // notify the listener about the request
  TResultOpt result = listener->moveTrack(sessionId, trackId, queueType);
  if (result.has_value()) {
    return mapErrorToResponse(result.value());
  }

  // construct the response
  json responseBody = {};
  return make_shared<string_response>(responseBody.dump());
}

//
// REMOVE TRACK
//

shared_ptr<http_response> const removeTrackHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);
  auto parseResult = parseJsonString(infos.body);
  if (holds_alternative<Error>(parseResult)) {
    return mapErrorToResponse(get<Error>(parseResult));
  }
  json const bodyJson = get<json const>(parseResult);

  // parse request specific JSON fields
  if (bodyJson.find("session_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'session_id' not found"));
  }
  TSessionID sessionId = bodyJson["session_id"].get<TSessionID>();

  if (bodyJson.find("track_id") == bodyJson.cend()) {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat, "Field 'track_id' not found"));
  }
  TTrackID trackId = bodyJson["track_id"].get<TTrackID>();

  // notify the listener about the request
  TResultOpt result = listener->removeTrack(sessionId, trackId);
  if (result.has_value()) {
    return mapErrorToResponse(result.value());
  }

  // construct the response
  json responseBody = {};
  return make_shared<string_response>(responseBody.dump());
}
