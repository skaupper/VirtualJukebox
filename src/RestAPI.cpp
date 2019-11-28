/***************************************************************************/
/**
 * @file    RestAPI.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of class RestAPI
 */
/***************************************************************************/

#include "RestAPI.h"

#include <sstream>

#include "ConfigHandler.h"

using namespace std;
using namespace httpserver;

static string CONFIG_SECTION = "RestAPI";

static const string getBasePath() {
  static const string API_BASE_PATH = "/api/";
  static const string API_VERSION = "v1";

  return API_BASE_PATH + API_VERSION;
}

static const shared_ptr<http_response> notFoundHandler(
    http_request const &req) {
  stringstream msg;
  msg << "Endpoint '" << req.get_path() << "' was not found!";
  return make_shared<string_response>(msg.str(), 404);
}

static const shared_ptr<http_response> notAllowedHandler(
    http_request const &req) {
  stringstream msg;
  msg << "Method '" << req.get_method() << "' is not allowed at endpoint '"
      << req.get_path() << "'!";
  return make_shared<string_response>(msg.str(), 405);
}

static const shared_ptr<http_response> internalErrorHandler(
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

class GetEndpointHandler : public http_resource {
  const shared_ptr<http_response> render(http_request const &req) {
    stringstream msg;
    msg << "PostEndpointHandler" << std::endl;
    msg << "Method  : " << req.get_method() << std::endl;
    msg << "Path    : " << req.get_path() << std::endl;
    msg << "Content : " << req.get_content() << std::endl;
    return make_shared<string_response>(msg.str());
  }
};

class PostEndpointHandler : public http_resource {
  const shared_ptr<http_response> render(http_request const &req) {
    stringstream msg;
    msg << "PostEndpointHandler" << std::endl;
    msg << "Method  : " << req.get_method() << std::endl;
    msg << "Path    : " << req.get_path() << std::endl;
    msg << "Content : " << req.get_content() << std::endl;
    return make_shared<string_response>(msg.str());
  }
};

TResultOpt RestAPI::handleRequests() {
  //   auto configHandler = ConfigHandler::GetInstance();
  //   auto port = configHandler.getValue("RestAPI");
  auto port = 8080;
  auto webserverParams =
      create_webserver(port)                               //
          .not_found_resource(notFoundHandler)             //
          .method_not_allowed_resource(notAllowedHandler)  //
          .internal_error_resource(internalErrorHandler)   //
          .no_regex_checking()                             //
          .single_resource()                               //
          .no_basic_auth()                                 //
          .no_digest_auth()                                //
          .start_method(http::http_utils::THREAD_PER_CONNECTION);

  // create the webserver
  ws = make_unique<webserver>(webserverParams);

  // configure endpoints
  GetEndpointHandler getHandler;
  PostEndpointHandler postHandler;

  getHandler.disallow_all();
  getHandler.set_allowing("GET", true);
  postHandler.disallow_all();
  postHandler.set_allowing("POST", true);

  ws->register_resource(getBasePath(), &getHandler);
  ws->register_resource(getBasePath(), &postHandler);

  // run the webserver in blocking mode
  ws->start(true);

  return nullopt;
}

void RestAPI::decodeAndDispatch(string, string, string) {
}
