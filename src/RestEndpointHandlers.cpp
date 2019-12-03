/*****************************************************************************/
/**
 * @file    RestEndpointHandlers.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Handler functions for all available REST endpoints
 */
/*****************************************************************************/

#include "RestEndpointHandlers.h"

#include <iostream>

#include "Serializer.h"
#include "json/json.hpp"

using namespace std;
using namespace httpserver;
using json = nlohmann::json;

//
// Helper functions
//

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
// Helper macros
//

#define PARSE_REQUIRED_STRING_FIELD(name, body)                                \
  do {                                                                         \
    if (body.find(#name) == body.cend()) {                                     \
      return mapErrorToResponse(                                               \
          Error(ErrorCode::InvalidFormat, "Field '" #name "' not found"));     \
    }                                                                          \
    if (!body[#name].is_string()) {                                            \
      return mapErrorToResponse(Error(                                         \
          ErrorCode::InvalidFormat, "Value of '" #name "' must be a string")); \
    }                                                                          \
    name = body[#name].get<string>();                                          \
  } while (0)

#define PARSE_REQUIRED_INT_FIELD(name, body)                               \
  do {                                                                     \
    if (body.find(#name) == body.cend()) {                                 \
      return mapErrorToResponse(                                           \
          Error(ErrorCode::InvalidFormat, "Field '" #name "' not found")); \
    }                                                                      \
    if (!body[#name].is_number_integer()) {                                \
      return mapErrorToResponse(Error(ErrorCode::InvalidFormat,            \
                                      "Value of '" #name                   \
                                      "' must be an integer"));            \
    }                                                                      \
    name = body[#name].get<int>();                                         \
  } while (0)

#define PARSE_OPTIONAL_STRING_FIELD(name, body)                   \
  do {                                                            \
    if (body.find(#name) != body.cend()) {                        \
      auto nameJson = body[#name];                                \
      if (!nameJson.is_string()) {                                \
        return mapErrorToResponse(Error(ErrorCode::InvalidFormat, \
                                        "Value of '" #name        \
                                        "' must be a string"));   \
      }                                                           \
      name = nameJson.get<string>();                              \
    }                                                             \
  } while (0)

#define PARSE_OPTIONAL_INT_PARAMETER(name, args)                               \
  do {                                                                         \
    if (args.find(#name) != args.cend()) {                                     \
      auto paramStr = args.at(#name);                                          \
      int tmpValue;                                                            \
      size_t idx;                                                              \
      try {                                                                    \
        tmpValue = stoi(paramStr, &idx);                                       \
      } catch (invalid_argument const &) {                                     \
        return mapErrorToResponse(Error(ErrorCode::InvalidFormat,              \
                                        "Parameter '" #name                    \
                                        "' is not an integer"));               \
      }                                                                        \
      if (idx != paramStr.size()) {                                            \
        return mapErrorToResponse(Error(                                       \
            ErrorCode::InvalidFormat,                                          \
            "Parameter '" #name "' must not contain non-integer characters")); \
      }                                                                        \
      name = tmpValue;                                                         \
    }                                                                          \
  } while (0)

#define PARSE_REQUIRED_STRING_PARAMETER(name, args)                            \
  do {                                                                         \
    if (args.find(#name) == args.cend()) {                                     \
      return mapErrorToResponse(                                               \
          Error(ErrorCode::InvalidFormat, "Parameter '" #name "' not found")); \
    }                                                                          \
    name = args.at(#name);                                                     \
  } while (0)

//
// GENERATE SESSION
//

shared_ptr<http_response> const generateSessionHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);

  auto parseResult = parseJsonString(infos.body);
  if (holds_alternative<Error>(parseResult)) {
    return mapErrorToResponse(get<Error>(parseResult));
  }
  json const bodyJson = get<json const>(parseResult);

  // parse request parameters
  optional<TPassword> password;
  optional<string> nickname;

  PARSE_OPTIONAL_STRING_FIELD(password, bodyJson);
  PARSE_OPTIONAL_STRING_FIELD(nickname, bodyJson);

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

  // parse request parameters
  std::string pattern;
  int max_entries = 50;

  PARSE_REQUIRED_STRING_PARAMETER(pattern, infos.args);
  PARSE_OPTIONAL_INT_PARAMETER(max_entries, infos.args);

  // notify the listener about the request
  auto result = listener->queryTracks(pattern, max_entries);
  if (holds_alternative<Error>(result)) {
    return mapErrorToResponse(get<Error>(result));
  }

  // construct the response
  auto queriedTracks = get<0>(result);

  json jsonTracks = {};
  for (auto &&track : queriedTracks) {
    jsonTracks.push_back(Serializer::serialize(track));
  }

  json responseBody = {{"tracks", jsonTracks}};

  return make_shared<string_response>(responseBody.dump());
}

//
// GET CURRENT QUEUES
//

shared_ptr<http_response> const getCurrentQueuesHandler(
    NetworkListener *listener, RequestInformation const &infos) {
  assert(listener);

  // parse request parameters
  TSessionID session_id;
  PARSE_REQUIRED_STRING_PARAMETER(session_id, infos.args);

  // notify the listener about the request
  auto result = listener->getCurrentQueues(session_id);
  if (holds_alternative<Error>(result)) {
    return mapErrorToResponse(get<Error>(result));
  }

  // construct the response

  auto queueStatus = get<0>(result);

  json playbackTrack = Serializer::serialize(queueStatus.currentTrack);
  json normalQueue;
  json adminQueue;
  for (auto &&track : queueStatus.normalQueue.tracks) {
    normalQueue.push_back(Serializer::serialize(track));
  }
  for (auto &&track : queueStatus.adminQueue.tracks) {
    adminQueue.push_back(Serializer::serialize(track));
  }

  json responseBody = {{"currently_playing", playbackTrack},
                       {"normal_queue", normalQueue},
                       {"admin_queue", adminQueue}};

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
  TSessionID session_id;
  TTrackID track_id;
  optional<string> queue_type;

  PARSE_REQUIRED_STRING_FIELD(session_id, bodyJson);
  PARSE_REQUIRED_STRING_FIELD(track_id, bodyJson);
  PARSE_OPTIONAL_STRING_FIELD(queue_type, bodyJson);

  QueueType queueType = QueueType::Normal;
  if (queue_type.has_value()) {
    if (queue_type.value() == "admin") {
      queueType = QueueType::Admin;
    } else if (queue_type.value() == "normal") {
      queueType = QueueType::Normal;
    } else {
      return mapErrorToResponse(
          Error(ErrorCode::InvalidFormat,
                "Value of 'queue_type' must either be 'admin' or 'normal'"));
    }
  }

  // notify the listener about the request
  TResultOpt result =
      listener->addTrackToQueue(session_id, track_id, queueType);
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
  TSessionID session_id;
  TTrackID track_id;
  int vote;

  PARSE_REQUIRED_STRING_FIELD(session_id, bodyJson);
  PARSE_REQUIRED_STRING_FIELD(track_id, bodyJson);
  PARSE_REQUIRED_INT_FIELD(vote, bodyJson);

  // notify the listener about the request
  TResultOpt result = listener->voteTrack(session_id, track_id, (vote != 0));
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
  TSessionID session_id;
  string player_action;

  PARSE_REQUIRED_STRING_FIELD(session_id, bodyJson);
  PARSE_REQUIRED_STRING_FIELD(player_action, bodyJson);

  PlayerAction playerAction;
  // TODO: do deserialization using the JSON framework
  if (player_action == "play") {
    playerAction = PlayerAction::Play;
  } else if (player_action == "pause") {
    playerAction = PlayerAction::Pause;
  } else if (player_action == "stop") {
    playerAction = PlayerAction::Stop;
  } else if (player_action == "skip") {
    playerAction = PlayerAction::Skip;
  } else if (player_action == "volume_up") {
    playerAction = PlayerAction::VolumeUp;
  } else if (player_action == "volume_down") {
    playerAction = PlayerAction::VolumeDown;
  } else {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat,
              "Value of 'player_action' must be a valid action."));
  }

  // notify the listener about the request
  TResultOpt result = listener->controlPlayer(session_id, playerAction);
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
  TSessionID session_id;
  TTrackID track_id;
  optional<string> queue_type;

  PARSE_REQUIRED_STRING_FIELD(session_id, bodyJson);
  PARSE_REQUIRED_STRING_FIELD(track_id, bodyJson);
  PARSE_OPTIONAL_STRING_FIELD(queue_type, bodyJson);

  // TODO: do deserialization using the JSON framework
  QueueType queueType;
  if (queue_type.value() == "admin") {
    queueType = QueueType::Admin;
  } else if (queue_type.value() == "normal") {
    queueType = QueueType::Normal;
  } else {
    return mapErrorToResponse(
        Error(ErrorCode::InvalidFormat,
              "Value of 'queue_type' must either be 'admin' or 'normal'"));
  }

  // notify the listener about the request
  TResultOpt result = listener->moveTrack(session_id, track_id, queueType);
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
  TSessionID session_id;
  TTrackID track_id;

  PARSE_REQUIRED_STRING_FIELD(session_id, bodyJson);
  PARSE_REQUIRED_STRING_FIELD(track_id, bodyJson);

  // notify the listener about the request
  TResultOpt result = listener->removeTrack(session_id, track_id);
  if (result.has_value()) {
    return mapErrorToResponse(result.value());
  }

  // construct the response
  json responseBody = {};
  return make_shared<string_response>(responseBody.dump());
}
