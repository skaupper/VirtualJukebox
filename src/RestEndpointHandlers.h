/*****************************************************************************/
/**
 * @file    RestEndpointHandlers.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Handler functions for all available REST endpoints
 */
/*****************************************************************************/

#ifndef _REST_ENDPOINT_HANDLERS_H_
#define _REST_ENDPOINT_HANDLERS_H_

#include <httpserver.hpp>

#include "NetworkListener.h"

typedef std::shared_ptr<httpserver::http_response> const (*TEndpointHandler)(
    NetworkListener *, std::string const &body);

std::shared_ptr<httpserver::http_response> const generateSessionHandler(
    NetworkListener *, std::string const &body);

std::shared_ptr<httpserver::http_response> const queryTracksHandler(
    NetworkListener *, std::string const &body);

#endif  // _REST_ENDPOINT_HANDLERS_H_
