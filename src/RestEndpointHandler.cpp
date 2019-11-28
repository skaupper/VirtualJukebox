/*****************************************************************************/
/**
 * @file    RestEndpointHandler.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of class RestEndpointHandler
 */
/*****************************************************************************/

#include "RestEndpointHandler.h"

#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;
using namespace httpserver;

static string const CONFIG_SECTION = "RestAPI";
static string const API_BASE_PATH = "/api/v1";

//
// Default request handlers
//

shared_ptr<http_response> const RestEndpointHandler::NotFoundHandler(
    http_request const &req) {
  stringstream msg;
  msg << "Endpoint '" << req.get_path() << "' was not found!";
  return make_shared<string_response>(msg.str(), 404);
}
shared_ptr<http_response> const RestEndpointHandler::NotAllowedHandler(
    http_request const &req) {
  stringstream msg;
  msg << "Method '" << req.get_method() << "' is not allowed at endpoint '"
      << req.get_path() << "'!";
  return make_shared<string_response>(msg.str(), 405);
}

shared_ptr<http_response> const RestEndpointHandler::InternalErrorHandler(
    http_request const &req) {
  stringstream msg;
  msg << "The request to endpoint '" << req.get_path() << "' ";
  msg << "with method '" << req.get_method() << "' ";
  msg << "lead to an internal server error." << std::endl;
  msg << "Please contact the server team!" << std::endl << std::endl;
  msg << "Request content:" << std::endl;
  msg << req.get_content();
  return make_shared<string_response>(msg.str(), 500);
}

//
// RestEndpointHandler implementation
//

RestEndpointHandler::RestEndpointHandler(NetworkListener *listener)
    : listener(listener) {
  assert(listener);
}

bool RestEndpointHandler::isValidBasePath(string const &path) const {
  // check if API_BASE_PATH is found at index 0
  return path.rfind(API_BASE_PATH, 0) == 0;
}

shared_ptr<http_response> const RestEndpointHandler::render(
    http_request const &req) {
  if (!isValidBasePath(req.get_path())) {
    return NotFoundHandler(req);
  }

  // truncate the base path
  auto path = req.get_path().substr(API_BASE_PATH.size());
  return decodeAndDispatch(path, req.get_method(), req.get_content());
}

shared_ptr<http_response> const RestEndpointHandler::decodeAndDispatch(
    string const &path, string const &method, string const &body) {
  stringstream msg;
  msg << "Method  : " << method << std::endl;
  msg << "Path    : " << path << std::endl;
  msg << "Content : " << body << std::endl;
  return make_shared<string_response>(msg.str());
}
