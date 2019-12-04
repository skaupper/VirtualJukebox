/**
 * @file    SpotifyAPI.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAPI definition
 */

#include "SpotifyAPI.h"

#include <connection.h>

#include <cassert>
#include <memory>

#include "restclient.h"

using namespace SpotifyApi;

TResult<Token> SpotifyAPI::getAccessToken(GrantType grantType,
                                          std::string const &code,
                                          std::string const &redirectUri,
                                          std::string clientID,
                                          std::string clientSecret) {
  // only authorization code supported until now ..
  assert(grantType == AuthorizationCode);
  auto client = std::make_unique<RestClient::Connection>(cSpotifyBaseUrl);

  // build body
  std::string body;
  body += std::string("grant_type=authorization_code") + std::string("&code=") +
          code + std::string("&client_id=" + clientID) +
          std::string("&client_secret=" + clientSecret) +
          std::string("&redirect_uri=") + redirectUri;

  // build header
  RestClient::HeaderFields headers;
  headers.insert(
      std::pair<std::string, std::string>("Accept", "application/json"));
  headers.insert(std::pair<std::string, std::string>(
      "Content-Type", "application/x-www-form-urlencoded"));
  client->SetHeaders(headers);

  client->SetTimeout(cRequestTimeout);
  auto response = client->post("/api/token", body);

  if (response.code == cHTTPOK) {
    Token token(nlohmann::json::parse(response.body));
    return std::move(token);
  } else {
    return Error(ErrorCode::AccessDenied, "Access denied");
  }
}

TResult<std::vector<Device>> getAvailableDevices(
    std::string const &accessToken) {
  std::vector<Device> devices;
  devices.emplace_back(Device{});

  return std::move(devices);
}

TResult<Playback> getCurrentPlayback(std::string const &accessToken,
                                     std::string const &market) {
  Playback playback;

  return playback;
}