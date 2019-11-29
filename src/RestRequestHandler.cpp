/*****************************************************************************/
/**
 * @file    RestRequestHandler.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of class RestRequestHandler
 */
/*****************************************************************************/

#include "RestRequestHandler.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "RestEndpointHandlers.h"
#include "json/json.hpp"

using namespace std;
using namespace httpserver;
using json = nlohmann::json;

static string const API_BASE_PATH = "/api/v1";

//
// Utilities
//

static optional<string> getCurrentExceptionMessage() {
  try {
    auto eptr = std::current_exception();
    if (eptr) {
      std::rethrow_exception(eptr);
    }
  } catch (std::exception const &e) {
    return e.what();
  }

  return nullopt;
}

//
// Default request handlers
//

shared_ptr<http_response> const RestRequestHandler::NotFoundHandler(
    http_request const &req) {
  stringstream msg;
  msg << "Endpoint '" << req.get_path() << "' was not found!";
  return make_shared<string_response>(msg.str(), 404);
}
shared_ptr<http_response> const RestRequestHandler::NotAllowedHandler(
    http_request const &req) {
  stringstream msg;
  msg << "Method '" << req.get_method() << "' is not allowed at endpoint '"
      << req.get_path() << "'!";
  return make_shared<string_response>(msg.str(), 405);
}

shared_ptr<http_response> const RestRequestHandler::InternalErrorHandler(
    http_request const &req) {
  stringstream msg;
  msg << "The request to endpoint '" << req.get_path() << "' ";
  msg << "with method '" << req.get_method() << "' ";
  msg << "lead to an internal server error." << std::endl;
  msg << "Please contact the server team!" << std::endl << std::endl;
  msg << "Request content:" << std::endl;
  msg << req.get_content() << std::endl;

  auto exceptionMessageOpt = getCurrentExceptionMessage();
  if (exceptionMessageOpt.has_value()) {
    msg << std::endl;
    msg << "Exception:" << std::endl;
    msg << exceptionMessageOpt.value();
  }
  return make_shared<string_response>(msg.str(), 500);
}

//
// RestRequestHandler implementation
//

RestRequestHandler::RestRequestHandler(NetworkListener *listener)
    : listener(listener) {
  assert(listener);
}

bool RestRequestHandler::isValidBasePath(string const &path) const {
  // check if API_BASE_PATH is found at index 0
  return path.rfind(API_BASE_PATH, 0) == 0;
}

shared_ptr<http_response> const RestRequestHandler::render(
    http_request const &req) {
  if (!isValidBasePath(req.get_path())) {
    return NotFoundHandler(req);
  }

  // truncate the base path
  auto path = req.get_path().substr(API_BASE_PATH.size());
  return decodeAndDispatch(path, req.get_method(), req.get_content());
}

shared_ptr<http_response> const RestRequestHandler::decodeAndDispatch(
    string const &path, string const &method, string const &body) {
  static const map<pair<string, string>, TEndpointHandler> AVAILABLE_ENDPOINTS =
      {
          {{"/generateSession", "POST"}, generateSessionHandler},  //
          {{"/queryTracks", "GET"}, queryTracksHandler}            //
      };

  if (auto handlerIt = AVAILABLE_ENDPOINTS.find({path, method});
      handlerIt != AVAILABLE_ENDPOINTS.cend()) {
    return handlerIt->second(listener, body);
  }

  stringstream msg;
  msg << "Method  : " << method << endl;
  msg << "Path    : " << path << endl;
  msg << "Content : " << body << endl;
  return make_shared<string_response>(msg.str());
}
