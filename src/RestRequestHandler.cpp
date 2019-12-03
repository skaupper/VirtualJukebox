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
    auto eptr = current_exception();
    if (eptr) {
      rethrow_exception(eptr);
    }
  } catch (exception const &e) {
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
  msg << "lead to an internal server error." << endl;
  msg << "Please contact the server team!" << endl << endl;
  msg << "Request content:" << endl;
  msg << req.get_content() << endl;

  auto exceptionMessageOpt = getCurrentExceptionMessage();
  if (exceptionMessageOpt.has_value()) {
    msg << endl;
    msg << "Exception:" << endl;
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

  cout << "Path: " << req.get_path() << endl;
  cout << "Method: " << req.get_method() << endl;
  cout << "Body: " << req.get_content() << endl;
  cout << "Query parameters: " << req.get_querystring() << endl;

  // truncate the base path
  auto path = req.get_path().substr(API_BASE_PATH.size());
  auto response = decodeAndDispatch(RequestInformation{
      path,               //
      req.get_method(),   //
      req.get_content(),  //
      req.get_args()      //
  });

  if (response) {
    return response;
  }

  return NotFoundHandler(req);
}

shared_ptr<http_response> const RestRequestHandler::decodeAndDispatch(
    RequestInformation const &infos) {
  static const map<pair<string, string>, TEndpointHandler> AVAILABLE_ENDPOINTS =
      {
          {{"/generateSession", "POST"}, generateSessionHandler},   //
          {{"/queryTracks", "GET"}, queryTracksHandler},            //
          {{"/getCurrentQueues", "GET"}, getCurrentQueuesHandler},  //
          {{"/addTrackToQueue", "POST"}, addTrackToQueueHandler},   //
          {{"/voteTrack", "PUT"}, voteTrackHandler},                //
          {{"/controlPlayer", "PUT"}, controlPlayerHandler},        //
          {{"/moveTracks", "PUT"}, moveTracksHandler},              //
          {{"/removeTrack", "DELETE"}, removeTrackHandler}          //
      };

  if (auto handlerIt = AVAILABLE_ENDPOINTS.find({infos.path, infos.method});
      handlerIt != AVAILABLE_ENDPOINTS.cend()) {
    return handlerIt->second(listener, infos);
  }

  return nullptr;
}
