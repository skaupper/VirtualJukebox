/**
 * @file    SpotifyAPI.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAuthorization implementation
 */

#include "SpotifyAuthorization.h"

#include <chrono>
#include <memory>

#include "Result.h"
#include "httpserver.hpp"

using namespace SpotifyApi;
using namespace httpserver;

bool SpotifyAuthorization::startServer(void) {
  // TODO read data from config file and setup server here

  webserver ws = create_webserver(8080);
  ws.register_resource("/", this, true);
  ws.start(true);

  return ws.is_running();
}

std::string const &SpotifyAuthorization::getRefreshToken(void) {
  return mToken.getRefreshToken();
}
std::string const &SpotifyAuthorization::getAccessToken(void) {
  return mToken.getAccessToken();
}
TResultOpt SpotifyAuthorization::refreshAccessToken(void) {
  return Error(ErrorCode::AccessDenied, "Test");
}

__int64_t SpotifyAuthorization::getExpiresAt(void) {
  return mTokenReceiveTime + mToken.getExpiresIn();
}

void SpotifyAuthorization::setScopes(std::string const &scopes) {
  mScopes = scopes;
}

std::string SpotifyAuthorization::getScopes(void) {
  return mScopes;
}

const std::shared_ptr<httpserver::http_response> SpotifyAuthorization::render(
    httpserver::http_request const &request) {
  // dispatch path
  std::cout << request.get_path() << std::endl;
  if (request.get_path().find("/spotifyLogin") != std::string::npos) {
    return loginHandler(request);
  } else if (request.get_path().find("/spotifyCallback") != std::string::npos) {
    return callbackHandler(request);
  } else {
    auto response = std::make_shared<http_response>(
        string_response("Invalid Endpoint", http::http_utils::http_not_found));
    return response;
  }
}

const std::shared_ptr<httpserver::http_response>
SpotifyAuthorization::loginHandler(httpserver::http_request const &request) {
  auto response = std::make_shared<http_response>(
      string_response("", http::http_utils::http_found));

  auto state = generateRandomString(16);

  // build redirection string with query
  std::string redirectString("https://accounts.spotify.com/authorize");
  redirectString.append("?client_id=").append(mClientID);
  redirectString.append("&response_type=").append("code");
  redirectString.append("&scope=").append(mScopes);
  redirectString.append("&redirect_uri=").append(mRedirectUri);
  redirectString.append("&state=").append(state);

  std::cout << redirectString << std::endl;

  response->with_cookie("spotify_auth_state", state);
  response->with_header("Location", redirectString);
  return response;
}

const std::shared_ptr<httpserver::http_response>
SpotifyAuthorization::callbackHandler(httpserver::http_request const &request) {
  auto response = std::make_shared<http_response>(
      string_response("OK", http::http_utils::http_ok));
  auto queryString = request.get_querystring();
  std::cout << queryString << std::endl;

  // check if the state is the same as the sent one
  std::cout << getFromQueryString(queryString, "code") << std::endl;
  std::cout << getFromQueryString(queryString, "state") << std::endl;

  if (getFromQueryString(queryString, "state") !=
      request.get_cookie("spotify_auth_state")) {
    std::cout << "SpotifyAuthorization: received state != sent state"
              << std::endl;
  }

  if (getFromQueryString(queryString, "error") != "") {
    std::cout << "SpotifyAuthorization: callback error, reason: "
              << getFromQueryString(queryString, "error") << std::endl;
  } else if (getFromQueryString(queryString, "code") != "") {
    // successfull
    SpotifyAPI spotify;
    auto ret = spotify.getAccessToken(AuthorizationCode,
                                      getFromQueryString(queryString, "code"),
                                      mRedirectUri,
                                      mClientID,
                                      mClientSecret);
    if (auto value = std::get_if<Token>(&ret)) {
      std::cout << "access token: " << value->getAccessToken() << std::endl;
      std::cout << "refresh token: " << value->getRefreshToken() << std::endl;
      std::cout << "token type token: " << value->getTokenType() << std::endl;
      std::cout << "scope: " << value->getScope() << std::endl;
      std::cout << "expires in: " << value->getExpiresIn() << std::endl;
      mToken = *value;
      mTokenReceiveTime =
          std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
    }
  } else {
    // requested query not received
    std::cout
        << "SpotifyAuthorization: callback error, reason: invalid query string"
        << std::endl;
  }

  return response;
}

std::string SpotifyAuthorization::generateRandomString(size_t length) {
  std::string text = "";
  std::string possible =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

  for (size_t i = 0; i < length; i++) {
    text += possible[rand() % possible.size()];
  }
  return text;
};

std::string SpotifyAuthorization::getFromQueryString(std::string const &query,
                                                     std::string const &key) {
  std::string value;
  auto pos = query.find(key);
  if (pos != std::string::npos) {
    pos = query.find("=", pos);
    if ((pos != std::string::npos) && (pos + 1) < query.length()) {
      value = query.substr(pos + 1, query.find("&", pos));
    }
  }
  return value;
}