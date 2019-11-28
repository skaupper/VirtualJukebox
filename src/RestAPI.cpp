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

TResultOpt RestAPI::handleRequests() {
  //   auto configHandler = ConfigHandler::GetInstance();
  //   auto port = configHandler.getValue("RestAPI");
  auto port = 8080;
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
