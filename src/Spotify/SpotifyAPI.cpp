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
  nlohmann::json tokenJson;
  try {
    tokenJson = nlohmann::json::parse(response.body);
  } catch (...) {
    return Error(ErrorCode::SpotifyParseError,
                 "[SpotifyAPI] in get AccessToken Json received Parse Error");
  }

  if (response.code == cHTTPOK) {
    Token token(tokenJson);
    return std::move(token);
  } else {
    // check for error object
    if (tokenJson.find("error") != tokenJson.end()) {
      SpotifyError spotifyError(tokenJson["error"]);
      return errorParser(spotifyError);
    }
  }

  // if we reach here spotify sent an unexpected message
  return Error(ErrorCode::SpotifyAPIError,
               "Spotify sent an unexpected message");
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
      // check if devices exist
      if (deviceListJson.find("devices") != deviceListJson.end()) {
        for (nlohmann::json &elem : deviceListJson["devices"]) {
          devices.emplace_back(Device(elem));
        }
        return std::move(devices);
      }

    } else {
      // check for error object
      if (deviceListJson.find("error") != deviceListJson.end()) {
        SpotifyError spotifyError(deviceListJson["error"]);
        return errorParser(spotifyError);
      }
    }
  } catch (...) {
    // parse exception
    return Error(ErrorCode::SpotifyParseError,
                 "[SpotifyAPI] received json couldn't be parsed");
  }
  // if we reach here spotify sent an unexpected message
  return Error(ErrorCode::SpotifyAPIError,
               "Spotify sent an unexpected message");
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
  if (response.code == cNoContent) {
    LOG(INFO) << "[SotifyAPI] in getCurrentPlayback, no content received"
              << std::endl;
    return Playback();
  }
  try {
    playbackJson = nlohmann::json::parse(response.body);

    if (response.code == cHTTPOK) {
      return Playback(playbackJson);

    } else {
      // check for error object
      if (playbackJson.find("error") != playbackJson.end()) {
        SpotifyError spotifyError(playbackJson["error"]);
        return errorParser(spotifyError);
      }
    }
    // if we reach here or the exception gets thrown spotify sent an unexpected
    // message
  } catch (...) {
    // parse exception
    return Error(ErrorCode::SpotifyParseError,
                 "[SpotifyAPI] received json couldn't be parsed");
  }
  // if we reach here spotify sent an unexpected message
  return Error(ErrorCode::SpotifyAPIError,
               "Spotify sent an unexpected message");
}

TResult<SpotifyPaging> SpotifyAPI::search(std::string const &accessToken,
                                          std::string const &queryKey,
                                          SpotifyApi::QueryType type,
                                          const int limit,
                                          int const offset,
                                          const std::string &market) {
  auto client = std::make_unique<RestClient::Connection>(cSpotifyAPIUrl);
  RestClient::HeaderFields headers;
  headers.insert(
      std::pair<std::string, std::string>("Accept", "application/json"));
  headers.insert(
      std::pair<std::string, std::string>("Content-Type", "application/json"));
  headers.insert(std::pair<std::string, std::string>("Authorization",
                                                     "Bearer " + accessToken));
  client->SetHeaders(headers);

  // build query
  std::stringstream queryStream;
  queryStream << "?q=" << stringUrlEncode(queryKey) << "&type="<<cQueryTypeMap.at(type)<<"&market="<<market<<"&limit="<<limit<<"&offset="<<offset;

  client->SetTimeout(cRequestTimeout);
  auto response = client->get("/v1/search"+queryStream.str());
  std::cout<<"/v1/search"+queryStream.str()<<std::endl;
//  std::cout<<response.body<<std::endl;
  nlohmann::json pagingJson;
  if (response.code == cNoContent) {
    LOG(INFO) << "[SotifyAPI] in search, no content received" << std::endl;
    return SpotifyPaging();
  }
  try {
    pagingJson = nlohmann::json::parse(response.body);

    if (response.code == cHTTPOK) {
      return SpotifyPaging(pagingJson);

    } else {
      // check for error object
      if (pagingJson.find("error") != pagingJson.end()) {
        SpotifyError spotifyError(pagingJson["error"]);
        return errorParser(spotifyError);
      }
    }
    // if we reach here or the exception gets thrown spotify sent an unexpected
    // message
  } catch (...) {
    // parse exception
    return Error(ErrorCode::SpotifyParseError,
                 "[SpotifyAPI] received json couldn't be parsed");
  }
  // if we reach here spotify sent an unexpected message
  return Error(ErrorCode::SpotifyAPIError,
               "Spotify sent an unexpected message");
}

Error SpotifyAPI::errorParser(SpotifyApi::SpotifyError const &error) {
  if (error.getStatus() == cHTTPUnouthorized) {
    if (error.getMessage().find("Invalid access token") != std::string::npos) {
      return Error(ErrorCode::AccessDenied, error.getMessage());
    } else if (error.getMessage().find("The access token expired") !=
               std::string::npos) {
      return Error(ErrorCode::SessionExpired, error.getMessage());
    }
  } else {
    // unhandled spotify error
    LOG(ERROR) << "[SpotifyAPI]: Error " << error.getMessage()
               << " Status: " << error.getStatus() << std::endl;
    return Error(ErrorCode::SpotifyAPIError, error.getMessage());
  }
}

std::string SpotifyAPI::stringUrlEncode(std::string const &str) {
  std::map<char, std::string> const replaceMap = {
      {' ', "%20"}, {'/', "%2F"}, {';', "%3B"}};

  std::string urlEncoded = "";
  for (auto &elem : str) {
    if (replaceMap.find(elem) != replaceMap.end()) {
      urlEncoded.append(replaceMap.at(elem));
    }
    urlEncoded.append(1, elem);
  }
  return std::move(urlEncoded);
}