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
  auto client = std::make_unique<RestClient::Connection>(cSpotifyAuthUrl);

  // build body

  std::string body;
  body.append("grant_type=")
      .append("authorization_code")
      .append("&code=")
      .append(code)
      .append("&client_id=")
      .append(clientID)
      .append("&client_secret=")
      .append(clientSecret)
      .append("&redirect_uri=")
      .append(redirectUri);

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

TResult<std::vector<Device>> SpotifyAPI::getAvailableDevices(
    std::string const &accessToken) {
  auto client = std::make_unique<RestClient::Connection>(cSpotifyAPIUrl);
  RestClient::HeaderFields headers;
  headers.insert(
      std::pair<std::string, std::string>("Accept", "application/json"));
  headers.insert(
      std::pair<std::string, std::string>("Content-Type", "application/json"));
  headers.insert(std::pair<std::string, std::string>("Authorization",
                                                     "Bearer " + accessToken));
  client->SetHeaders(headers);
  client->SetTimeout(cRequestTimeout);
  auto response = client->get("/v1/me/player/devices");

  nlohmann::json deviceListJson;
  std::vector<Device> devices;

  try {
    deviceListJson = nlohmann::json::parse(response.body);

    if (response.code == cHTTPOK) {
      std::cout << deviceListJson.dump(2) << std::endl;

      // check if devices exist
      if (deviceListJson.find("devices") != deviceListJson.end()) {
        for (nlohmann::json &elem : deviceListJson["devices"]) {
          devices.emplace_back(Device(elem));
        }
        return std::move(devices);
      }

    } else {
      // check for error string
      if (deviceListJson.find("error") != deviceListJson.end()) {
        SpotifyError spotifyError(deviceListJson);
        return errorParser(spotifyError);
      }
    }
    // if we reach here spotify sent an unexpected message
    // return spotify Unexpected
  } catch (...) {
    // parse exception
    // return spotify parse error
  }
}

TResult<Playback> SpotifyAPI::getCurrentPlayback(std::string const &accessToken,
                                                 std::string const &market) {
  auto client = std::make_unique<RestClient::Connection>(cSpotifyAPIUrl);
  RestClient::HeaderFields headers;
  headers.insert(
      std::pair<std::string, std::string>("Accept", "application/json"));
  headers.insert(
      std::pair<std::string, std::string>("Content-Type", "application/json"));
  headers.insert(std::pair<std::string, std::string>("Authorization",
                                                     "Bearer " + accessToken));
  client->SetHeaders(headers);
  client->SetTimeout(cRequestTimeout);
  auto response = client->get("/v1/me/player");

  nlohmann::json playbackJson;

  try {
    playbackJson = nlohmann::json::parse(response.body);
    std::cout << playbackJson.dump(4) << std::endl;
  } catch (...) {
  }

  return Error(ErrorCode::NotImplemented, "test");
}

Error SpotifyAPI::errorParser(SpotifyApi::SpotifyError const &error) {
  if (error.getStatus() == cHTTPUnouthorized) {
    if (error.getMessage() == "Invalid access token") {
      return Error(ErrorCode::AccessDenied, error.getMessage());
    } else if (error.getMessage() == "The access token expired") {
      return Error(ErrorCode::AccessDenied, error.getMessage());
    }
  } else {
    // unhandled spotify error
    return Error(ErrorCode::NotImplemented, error.getMessage());
  }
}