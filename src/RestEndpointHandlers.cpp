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
      {ErrorCode::AccessDenied, 403}  //
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

shared_ptr<http_response> const generateSessionHandler(
    NetworkListener *listener, string const &body) {
  assert(listener);

  // parse body into JSON object
  auto parseResult = parseJsonString(body);
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

  // notify the listener about the request
  TResult<TSessionID> result = listener->generateSession(password);
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

shared_ptr<http_response> const queryTracksHandler(NetworkListener *listener,
                                                   string const &body) {
  assert(listener);
  return make_shared<string_response>("queryTracks");
}
