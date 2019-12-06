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
    return std::move(token);
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
    LOG(INFO) << "[SpotifyAPI] in getCurrentPlayback, no content received"
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
  queryStream << "?q=" << stringUrlEncode(queryKey)
              << "&type=" << cQueryTypeMap.at(type) << "&market=" << market
              << "&limit=" << limit << "&offset=" << offset;

  client->SetTimeout(cRequestTimeout);
  auto response = client->get("/v1/search" + queryStream.str());
  std::cout << "/v1/search" + queryStream.str() << std::endl;
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

TResultOpt SpotifyAPI::setVolume(std::string const &accessToken,
                                 int volume,
                                 const SpotifyApi::Device &device) {
  auto client = std::make_unique<RestClient::Connection>(cSpotifyAPIUrl);
  RestClient::HeaderFields headers;
  headers.insert(
      std::pair<std::string, std::string>("Accept", "application/json"));
  headers.insert(
      std::pair<std::string, std::string>("Content-Type", "application/json"));
  headers.insert(std::pair<std::string, std::string>("Authorization",
                                                     "Bearer " + accessToken));
  client->SetHeaders(headers);

  // set upper and lower bounds
  volume = volume > 100 ? 100 : volume;
  volume = volume < 0 ? 0 : volume;

  // build query
  std::stringstream queryStream;
  queryStream << "?volume_percent=" << (volume);
  if (device.getID() != "") {
    queryStream << "&device_id=" << device.getID();
  }

  client->SetTimeout(cRequestTimeout);
  auto response = client->put("/v1/me/player/volume" + queryStream.str(), "");

  if (response.code == cNoContent) {
    return std::nullopt;
  }

  nlohmann::json errJson;
  try {
    errJson = nlohmann::json::parse(response.body);

    // check for error object
    if (errJson.find("error") != errJson.end()) {
      SpotifyError spotifyError(errJson["error"]);
      return errorParser(spotifyError);
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

TResultOpt SpotifyAPI::pause(std::string const &accessToken,
                             const SpotifyApi::Device &device) {
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
  if (device.getID() != "") {
    queryStream << "?device_id=" << device.getID();
  }

  client->SetTimeout(cRequestTimeout);
  auto response = client->put("/v1/me/player/pause" + queryStream.str(), "");

  if (response.code == cNoContent) {
    return std::nullopt;
  }

  nlohmann::json errJson;
  try {
    errJson = nlohmann::json::parse(response.body);

    // check for error object
    if (errJson.find("error") != errJson.end()) {
      SpotifyError spotifyError(errJson["error"]);
      return errorParser(spotifyError);
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

TResultOpt SpotifyAPI::play(std::string const &accessToken,
                            std::vector<std::string> const &uris,
                            const SpotifyApi::Device &device,
                            int positionMs) {
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
  if (device.getID() != "") {
    queryStream << "?device_id=" << device.getID();
  }

  // build body
  nlohmann::json body = {{"position_ms", positionMs}};
  if (!uris.empty()) {
    body["uris"] = uris;
  }

  client->SetTimeout(cRequestTimeout);
  auto response =
      client->put("/v1/me/player/play" + queryStream.str(), body.dump());

  if (response.code == cNoContent) {
    return std::nullopt;
  }

  nlohmann::json errJson;
  try {
    errJson = nlohmann::json::parse(response.body);

    // check for error object
    if (errJson.find("error") != errJson.end()) {
      SpotifyError spotifyError(errJson["error"]);
      return errorParser(spotifyError);
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
    } else {
      return Error(ErrorCode::AccessDenied, error.getMessage());
    }
  } else if (error.getStatus() == cHTTPNotFound) {
    return Error(ErrorCode::SpotifyNotFound, error.getMessage());
  } else if (error.getStatus() == cHTTPForbidden) {
    return Error(ErrorCode::SpotifyForbidden, error.getMessage());
  } else {
    // unhandled spotify error
    LOG(ERROR) << "[SpotifyAPI]: Error " << error.getMessage()
               << " Status: " << error.getStatus() << std::endl;
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
  return std::move(urlEncoded);
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