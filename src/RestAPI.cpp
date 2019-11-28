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

  // check if the configuration value for "port" is an integer and a valid port
  // number
  try {
    port = std::stoi(portStr);
    if (port < 0 || port > 65535) {
      throw invalid_argument("Port value is out of range");
    }
  } catch (invalid_argument const &) {
    return Error(ErrorCode::InvalidFormat,
                 "RestAPI.handleRequests: Configuration value '" +
                     CONFIG_SECTION + ".port' cannot is not an integer");
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
