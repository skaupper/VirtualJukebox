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
#include "RequestInformation.h"

typedef std::shared_ptr<httpserver::http_response> const (*TEndpointHandler)(
    NetworkListener *, RequestInformation const &);

std::shared_ptr<httpserver::http_response> const generateSessionHandler(
    NetworkListener *, RequestInformation const &);

std::shared_ptr<httpserver::http_response> const queryTracksHandler(
    NetworkListener *, RequestInformation const &);

#endif  // _REST_ENDPOINT_HANDLERS_H_
