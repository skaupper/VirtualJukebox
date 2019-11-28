/*****************************************************************************/
/**
 * @file    RestAPI.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of class RestAPI
 */
/*****************************************************************************/

#include "RestAPI.h"

#include <sstream>

#include "ConfigHandler.h"
#include "RestEndpointHandler.h"

using namespace std;
using namespace httpserver;

static string const CONFIG_SECTION = "RestAPI";

static TResult<int> parsePort(string const &portStr) {
  int port = -1;
  size_t idx;

  // check if the configuration value for "port" is an integer
  try {
    port = std::stoi(portStr, &idx);
  } catch (invalid_argument const &) {
    return Error(ErrorCode::InvalidFormat,
                 "RestAPI.handleRequests: Configuration value '" +
                     CONFIG_SECTION + ".port' is not an integer");
  }

  // check if the whole string is an integer (idx is set to one past the end)
  if (idx != portStr.size()) {
    return Error(
        ErrorCode::InvalidFormat,
        "RestAPI.handleRequests: Configuration value for '" + CONFIG_SECTION + ".port' must only consist of digits");
  }

  if (port < 0 || port > 65535) {
    return Error(ErrorCode::InvalidValue, "Port value is out of range");
  }

  return port;
}

TResultOpt RestAPI::handleRequests() {
  auto configHandler = ConfigHandler::getInstance();

  TResult<string> configPort = configHandler->getValue(CONFIG_SECTION, "port");
  if (holds_alternative<Error>(configPort)) {
    return std::get<Error>(configPort);
  }

  auto parsedPort = parsePort(std::get<string>(configPort));
  if (holds_alternative<Error>(parsedPort)) {
    return std::get<Error>(parsedPort);
  }

  int port = std::get<int>(parsedPort);

  auto webserverParams =
      create_webserver(port)                                         //
          .not_found_resource(RestEndpointHandler::NotFoundHandler)  //
          .internal_error_resource(
              RestEndpointHandler::InternalErrorHandler)  //
          .no_regex_checking()                            //
          .single_resource()                              //
          .no_basic_auth()                                //
          .no_digest_auth()                               //
          .start_method(http::http_utils::THREAD_PER_CONNECTION);

  // create the webserver
  webserver ws{webserverParams};

  // use a single handler sensitive on all paths
  RestEndpointHandler handler(listener);
  ws.register_resource("/", &handler, true);

  // run the webserver in blocking mode
  ws.start(true);

  return nullopt;
}
