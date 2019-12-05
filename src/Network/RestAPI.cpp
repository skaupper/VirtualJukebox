/*****************************************************************************/
/**
 * @file    RestAPI.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of class RestAPI
 */
/*****************************************************************************/

#include "RestAPI.h"

#include <sstream>

#include "RestRequestHandler.h"
#include "Utils/ConfigHandler.h"

using namespace std;
using namespace httpserver;

static string const CONFIG_SECTION = "RestAPI";

TResultOpt RestAPI::handleRequests() {
  auto configHandler = ConfigHandler::getInstance();

  TResult<int> configPort = configHandler->getValueInt(CONFIG_SECTION, "port");
  if (holds_alternative<Error>(configPort)) {
    return get<Error>(configPort);
  }

  int port = get<int>(configPort);

  if (port < 0 || port > 65535) {
    return Error(ErrorCode::InvalidValue,
                 "RestAPI.handleRequests: Port value is out of range");
  }

  auto webserverParams =
      create_webserver(port)
          .not_found_resource(RestRequestHandler::NotFoundHandler)
          .internal_error_resource(RestRequestHandler::InternalErrorHandler)
          .no_regex_checking()
          .single_resource()
          .no_basic_auth()
          .no_digest_auth()
          .start_method(http::http_utils::THREAD_PER_CONNECTION);

  // create the webserver
  ws = make_unique<webserver>(webserverParams);

  // use a single handler sensitive on all paths
  RestRequestHandler handler(listener);
  ws->register_resource("/", &handler, true);

  // run the webserver in blocking mode
  ws->start(true);

  return nullopt;
}

void RestAPI::stopServer() {
  if (ws && ws->is_running()) {
    ws->stop();
    ws = nullptr;
  }
}
