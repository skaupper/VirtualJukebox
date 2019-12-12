/**
 * @file    SpotifyAPI.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SpotifyAPI definition
 */

#include "SpotifyAPI.h"

#include <connection.h>

#include <cassert>
#include <memory>

using namespace SpotifyApi;

TResult<Token> SpotifyAPI::getAccessToken(GrantType grantType,
                                          std::string const &code,
                                          std::string const &redirectUri,
                                          std::string clientID,
                                          std::string clientSecret) {
  (void)grantType;

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
    return token;
  } else {
    // check for error object
    if (tokenJson.find("error") != tokenJson.end()) {
      SpotifyError spotifyError(tokenJson);
      return errorParser(spotifyError);
    }
  }

  // if we reach here spotify sent an unexpected message
  return Error(ErrorCode::SpotifyAPIError,
               "Spotify sent an unexpected message");
}

TResult<Token> SpotifyAPI::refreshAccessToken(std::string const &refreshToken,
                                              std::string const &clientID,
                                              std::string const &clientSecret) {
  auto client = std::make_unique<RestClient::Connection>(cSpotifyAuthUrl);

  // build body

  std::string body;
  body.append("grant_type=")
      .append("refresh_token")
      .append("&refresh_token=")
      .append(refreshToken);

  // build header
  RestClient::HeaderFields headers;
  headers.insert(
      std::pair<std::string, std::string>("Accept", "application/json"));
  headers.insert(std::pair<std::string, std::string>(
      "Content-Type", "application/x-www-form-urlencoded"));
  headers.insert(std::pair<std::string, std::string>(
      "Authorization",
      "Basic " + (stringBase64Encode(clientID + ":" + clientSecret))));
  client->SetHeaders(headers);

  client->SetTimeout(cRequestTimeout);
  auto response = client->post("/api/token", body);
  nlohmann::json tokenJson;
  try {
    tokenJson = nlohmann::json::parse(response.body);
  } catch (...) {
    return Error(
        ErrorCode::SpotifyParseError,
        "[SpotifyAPI] in refreshAccessToken Json received Parse Error");
  }

  if (response.code == cHTTPOK) {
    Token token(tokenJson);
    return token;
  } else {
    // check for error object
    if (tokenJson.find("error") != tokenJson.end()) {
      SpotifyError spotifyError(tokenJson);
      return errorParser(spotifyError);
    }
  }

  // if we reach here spotify sent an unexpected message
  return Error(ErrorCode::SpotifyAPIError,
               "Spotify sent an unexpected message");
}

TResult<std::vector<Device>> SpotifyAPI::getAvailableDevices(
    std::string const &accessToken) {
  auto responseRet = spotifyCall(accessToken, "/v1/me/player/devices", HttpGet);
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  auto devicesRet = parseSpotifyCall<Devices>(response);
  if (auto error = std::get_if<Error>(&devicesRet)) {
    return *error;
  }
  return std::get<Devices>(devicesRet).mDevices;
}

TResult<std::optional<Playback>> SpotifyAPI::getCurrentPlayback(
    std::string const &accessToken, std::string const &market) {
  (void)market;

  auto responseRet = spotifyCall(accessToken, "/v1/me/player", HttpGet);
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  if (response.code == cNoContent) {
    LOG(INFO) << "SpotifyAPI.getCurrentPlayback: No content received";
    return std::nullopt;
  }

  auto playbackRet = parseSpotifyCall<Playback>(response);
  if (auto error = std::get_if<Error>(&playbackRet)) {
    LOG(ERROR) << "SpotifyAPI.getCurrentPlayback: " << error->getErrorMessage()
               << std::endl;
    return *error;
  }
  return std::get<Playback>(playbackRet);
}

TResult<SpotifyPaging> SpotifyAPI::search(std::string const &accessToken,
                                          std::string const &queryKey,
                                          SpotifyApi::QueryType type,
                                          const int limit,
                                          int const offset,
                                          const std::string &market) {
  auto client = std::make_unique<RestClient::Connection>(cSpotifyAPIUrl);

  // build query
  std::stringstream queryStream;
  queryStream << "?q=" << stringUrlEncode(queryKey)
              << "&type=" << cQueryTypeMap.at(type) << "&market=" << market
              << "&limit=" << limit << "&offset=" << offset;

  auto responseRet =
      spotifyCall(accessToken, "/v1/search", HttpGet, queryStream.str());
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  nlohmann::json pagingJson;
  if (response.code == cNoContent) {
    LOG(INFO) << "SpotifyAPI.search: No content received";
    return SpotifyPaging();
  }

  auto pagingRet = parseSpotifyCall<SpotifyPaging>(response);
  return pagingRet;
}

TResultOpt SpotifyAPI::setVolume(std::string const &accessToken,
                                 int volume,
                                 const SpotifyApi::Device &device) {
  // set upper and lower bounds
  volume = volume > 100 ? 100 : volume;
  volume = volume < 0 ? 0 : volume;

  // build query
  std::stringstream queryStream;
  queryStream << "?volume_percent=" << (volume);
  if (!device.getID().empty()) {
    queryStream << "&device_id=" << device.getID();
  }

  auto responseRet = spotifyCall(
      accessToken, "/v1/me/player/volume", HttpPut, queryStream.str());
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  if (response.code == cNoContent) {
    return std::nullopt;
  }

  auto volumeRet = parseSpotifyCall<SpotifyDummy>(response);
  if (auto error = std::get_if<Error>(&volumeRet)) {
    return *error;
  }

  // we should never reach here because this endpoint only sends Error or
  // noContent
  LOG(ERROR) << "SpotifyAPI.setVolume: Fatal Error" << std::endl;
  return Error(ErrorCode::SpotifyAPIError, "SpotifyAPI.setVolume: Fatal Error");
}

TResultOpt SpotifyAPI::pause(std::string const &accessToken,
                             const SpotifyApi::Device &device) {
  // build query
  std::stringstream queryStream;
  if (!device.getID().empty()) {
    queryStream << "?device_id=" << device.getID();
  }

  auto responseRet = spotifyCall(
      accessToken, "/v1/me/player/pause", HttpPut, queryStream.str());
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  if (response.code == cNoContent) {
    return std::nullopt;
  }

  auto ret = parseSpotifyCall<SpotifyDummy>(response);
  if (auto error = std::get_if<Error>(&ret)) {
    return *error;
  }

  // we should never reach here because this endpoint only sends Error or
  // noContent
  LOG(ERROR) << "SpotifyAPI.pause: Fatal Error" << std::endl;
  return Error(ErrorCode::SpotifyAPIError, "SpotifyAPI.pause: Fatal Error");
}

TResultOpt SpotifyAPI::play(std::string const &accessToken,
                            std::vector<std::string> const &uris,
                            const SpotifyApi::Device &device,
                            int positionMs) {
  // build query
  std::stringstream queryStream;
  if (!device.getID().empty()) {
    queryStream << "?device_id=" << device.getID();
  }

  // build body
  nlohmann::json body = {{"position_ms", positionMs}};
  if (!uris.empty()) {
    body["uris"] = uris;
  }

  auto responseRet = spotifyCall(accessToken,
                                 "/v1/me/player/play",
                                 HttpPut,
                                 queryStream.str(),
                                 body.dump());
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  if (response.code == cNoContent) {
    return std::nullopt;
  }

  auto ret = parseSpotifyCall<SpotifyDummy>(response);
  if (auto error = std::get_if<Error>(&ret)) {
    return *error;
  }

  // we should never reach here because this endpoint only sends Error or
  // noContent
  LOG(ERROR) << "SpotifyAPI.setVolume: Fatal Error" << std::endl;
  return Error(ErrorCode::SpotifyAPIError, "SpotifyAPI.play: Fatal Error");
}

TResult<Track> SpotifyAPI::getTrack(std::string const &accessToken,
                                    std::string const &spotifyID,
                                    const std::string &market) {
  (void)market;

  auto responseRet =
      spotifyCall(accessToken, "/v1/tracks/" + spotifyID, HttpGet);
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  if (response.code == cNoContent) {
    LOG(ERROR) << "SpotifyAPI.getTrack Spotify WebAPI Error, we never should "
                  "reach here"
               << std::endl;
    return Error(
        ErrorCode::SpotifyAPIError,
        "SpotifyAPI.getTrack Spotify WebAPI Error, we never should reach here");
    return Track();
  }

  auto trackRet = parseSpotifyCall<Track>(response);
  return trackRet;
}

TResultOpt SpotifyAPI::transferUsersPlayback(std::string const &accessToken,
                                             std::vector<Device> const &devices,
                                             bool play) {
  // build body
  nlohmann::json body;
  body["device_ids"] = nlohmann::json::array();

  for (auto const &device : devices) {
    body["device_ids"].emplace_back(device.getID());
  }
  body["play"] = play;

  auto responseRet =
      spotifyCall(accessToken, "/v1/me/player", HttpPut, "", body.dump());
  if (auto value = std::get_if<Error>(&responseRet)) {
    return *value;
  }
  auto response = std::get<RestClient::Response>(responseRet);

  if (response.code == cNoContent) {
    return std::nullopt;
  }

  auto ret = parseSpotifyCall<SpotifyDummy>(response);
  if (auto error = std::get_if<Error>(&ret)) {
    return *error;
  }

  // we should never reach here because this endpoint only sends Error or
  // noContent
  LOG(ERROR) << "SpotifyAPI.transferUsersPlayback: Fatal Error" << std::endl;
  return Error(ErrorCode::SpotifyAPIError,
               "SpotifyAPI.transferUsersPlayback: Fatal Error");
}

TResult<RestClient::Response> SpotifyAPI::spotifyCall(
    std::string const &accessToken,
    std::string const &endpoint,
    SpotifyApi::SpotifyAPI::HttpMethod method,
    const std::string &query,
    const std::string &body) {
  // check if access token is not empty
  if (accessToken.empty()) {
    return Error(ErrorCode::SpotifyAccessDenied, "Invalid access token");
  }

  // create standard headers for spotify api communication
  RestClient::HeaderFields headers;
  headers.insert({"Accept", "application/json"});
  headers.insert({"Content-Type", "application/json"});
  headers.insert({"Authorization", "Bearer " + accessToken});

  auto client = std::make_unique<RestClient::Connection>(cSpotifyAPIUrl);
  client->SetHeaders(headers);
  client->SetTimeout(cRequestTimeout);

  RestClient::Response response;

  switch (method) {
    case HttpGet: {
      response = client->get(endpoint + query);
    } break;

    case HttpPost: {
      response = client->post(endpoint + query, body);
    } break;

    case HttpPut: {
      response = client->put(endpoint + query, body);
    } break;
    default:
      return Error(ErrorCode::SpotifyAPIError, "Invalid Http method");
  }

  if (response.code == cHTTPTimeout) {
    return Error(ErrorCode::SpotifyHttpTimeout, "Timout on Spotify request");
  }

  return response;
}

template <typename SpotifyAPIType>
TResult<SpotifyAPIType> SpotifyAPI::parseSpotifyCall(
    RestClient::Response const &response) {
  nlohmann::json jsonData;
  try {
    jsonData = nlohmann::json::parse(response.body);

    if (response.code == cHTTPOK) {
      return SpotifyAPIType(jsonData);

    } else {
      // check for error object
      if (jsonData.find("error") != jsonData.end()) {
        SpotifyError spotifyError(jsonData["error"]);
        return errorParser(spotifyError);
      }
    }
    // if we reach here or the exception gets thrown spotify sent an unexpected
    // message
  } catch (...) {
    // parse exception
    return Error(ErrorCode::SpotifyParseError,
                 "Received json couldn't be parsed");
  }
  // if we reach here spotify sent an unexpected message
  return Error(ErrorCode::SpotifyAPIError,
               "Spotify sent an unexpected message");
}

Error SpotifyAPI::errorParser(SpotifyApi::SpotifyError const &error) {
  if (error.getStatus() == cHTTPUnouthorized) {
    if (error.getMessage().find("Invalid access token") != std::string::npos) {
      return Error(ErrorCode::SpotifyAccessDenied, error.getMessage());
    } else if (error.getMessage().find("The access token expired") !=
               std::string::npos) {
      return Error(ErrorCode::SpotifyAccessExpired, error.getMessage());
    } else {
      return Error(ErrorCode::SpotifyAccessDenied, error.getMessage());
    }
  } else if (error.getStatus() == cHTTPNotFound) {
    return Error(ErrorCode::SpotifyNotFound, error.getMessage());
  } else if (error.getStatus() == cHTTPForbidden) {
    return Error(ErrorCode::SpotifyForbidden, error.getMessage());
  } else if (error.getStatus() == cHTTPBadRequest) {
    return Error(ErrorCode::SpotifyBadRequest, error.getMessage());
  } else {
    // unhandled spotify error
    LOG(ERROR) << "SpotifyAPI.errorParser: Unhandled Spotify Error "
               << error.getMessage() << " Status: " << error.getStatus();
    return Error(ErrorCode::SpotifyAPIError, error.getMessage());
  }
}

std::string SpotifyAPI::stringUrlEncode(std::string const &str) {
  std::map<char, std::string> const replaceMap = {
      {' ', "%20"}, {'/', "%2F"}, {';', "%3B"}, {':', "%3A"}};

  std::string urlEncoded = "";
  for (auto &elem : str) {
    if (replaceMap.find(elem) != replaceMap.end()) {
      urlEncoded.append(replaceMap.at(elem));
    } else {
      urlEncoded.append(1, elem);
    }
  }
  return urlEncoded;
}

std::string SpotifyAPI::stringBase64Encode(std::string const &str) {
  static const char sEncodingTable[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

  size_t in_len = str.size();
  size_t out_len = 4 * ((in_len + 2) / 3);
  std::string ret(out_len, '\0');
  size_t i;
  char *p = const_cast<char *>(ret.c_str());

  for (i = 0; i < in_len - 2; i += 3) {
    *p++ = sEncodingTable[(str[i] >> 2) & 0x3F];
    *p++ =
        sEncodingTable[((str[i] & 0x3) << 4) | ((int)(str[i + 1] & 0xF0) >> 4)];
    *p++ = sEncodingTable[((str[i + 1] & 0xF) << 2) |
                          ((int)(str[i + 2] & 0xC0) >> 6)];
    *p++ = sEncodingTable[str[i + 2] & 0x3F];
  }
  if (i < in_len) {
    *p++ = sEncodingTable[(str[i] >> 2) & 0x3F];
    if (i == (in_len - 1)) {
      *p++ = sEncodingTable[((str[i] & 0x3) << 4)];
      *p++ = '=';
    } else {
      *p++ = sEncodingTable[((str[i] & 0x3) << 4) |
                            ((int)(str[i + 1] & 0xF0) >> 4)];
      *p++ = sEncodingTable[((str[i + 1] & 0xF) << 2)];
    }
    *p++ = '=';
  }

  return ret;
}
