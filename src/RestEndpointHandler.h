/*****************************************************************************/
/**
 * @file    RestEndpointHandler.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class RestEndpointHandler
 */
/*****************************************************************************/

#ifndef _REST_ENDPOINT_HANDLER_H_
#define _REST_ENDPOINT_HANDLER_H_

#include <httpserver.hpp>

#include "NetworkListener.h"

/**
 * @class RestEndpointHandler
 * @brief Provides REST endpoints for incoming connections.
 * @details Is used to validate and dispatch incoming requests. If a valid
 * request has found the assigned `NetworkListener` is notified.
 */
class RestEndpointHandler : public httpserver::http_resource {
 public:
  RestEndpointHandler(NetworkListener *listener);

  static std::shared_ptr<httpserver::http_response> const NotFoundHandler(
      httpserver::http_request const &req);
  static std::shared_ptr<httpserver::http_response> const NotAllowedHandler(
      httpserver::http_request const &req);
  static std::shared_ptr<httpserver::http_response> const InternalErrorHandler(
      httpserver::http_request const &req);

 private:
  NetworkListener *listener;

  bool isValidBasePath(std::string const &path) const;
  std::shared_ptr<httpserver::http_response> const render(
      httpserver::http_request const &req) override;
  std::shared_ptr<httpserver::http_response> const decodeAndDispatch(
      std::string const &path,
      std::string const &method,
      std::string const &body);
};

#endif  // _REST_ENDPOINT_HANDLER_H_
