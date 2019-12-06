#include "RestAPIFixture.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "Utils/ConfigHandler.h"

using namespace std;
using namespace std::literals::chrono_literals;

void RestAPIFixture::SetUp() {
  static string const configFilePath = "../test/test_config.ini";
  auto res = ConfigHandler::getInstance()->setConfigFilePath(configFilePath);
  ASSERT_FALSE(res.has_value());

  api.setListener(&listener);
  serverThread = thread{[this]() { api.handleRequests(); }};

  // let the server start properly before firing requests
  this_thread::sleep_for(1ms);
}

void RestAPIFixture::TearDown() {
  if (serverThread.joinable()) {
    api.stopServer();
    serverThread.join();
  }
}

static optional<string> getRequestUrl(
    string const &endpoint,
    optional<map<string, string>> const &queryParameters = nullopt) {
  static const string PORT_SECTION = "RestAPI";
  static const string PORT_KEY = "port";
  static const string ENDPOINT_BASE_PATH = "/api/v1";

  auto portOpt =
      ConfigHandler::getInstance()->getValueInt(PORT_SECTION, PORT_KEY);
  if (!holds_alternative<int>(portOpt)) {
    return nullopt;
  }

  stringstream url;
  url << "http://localhost:" << get<int>(portOpt) << ENDPOINT_BASE_PATH
      << endpoint;

  if (queryParameters.has_value()) {
    url << "?";
    for (auto &&kv : queryParameters.value()) {
      url << kv.first << "=" << kv.second;
    }
  }

  return url.str();
}

optional<RestClient::Response> RestAPIFixture::post(string const &endpoint,
                                                    string const &body) {
  auto url = getRequestUrl(endpoint);
  if (!url.has_value()) {
    return nullopt;
  }
  return RestClient::post(url.value(), "application/json", body);
}
