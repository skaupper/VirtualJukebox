/*****************************************************************************/
/**
 * @file    RestEndpointHandlers.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Handler functions for all available REST endpoints
 */
/*****************************************************************************/

#ifndef _REST_ENDPOINT_HANDLERS_H_
#define _REST_ENDPOINT_HANDLERS_H_

#include "NetworkListener.h"
#include "RequestInformation.h"

typedef ResponseInformation const (*TEndpointHandler)(
    NetworkListener *, RequestInformation const &);

ResponseInformation const generateSessionHandler(NetworkListener *,
                                                 RequestInformation const &);

ResponseInformation const queryTracksHandler(NetworkListener *,
                                             RequestInformation const &);

ResponseInformation const getCurrentQueuesHandler(NetworkListener *,
                                                  RequestInformation const &);

ResponseInformation const addTrackToQueueHandler(NetworkListener *,
                                                 RequestInformation const &);

ResponseInformation const voteTrackHandler(NetworkListener *,
                                           RequestInformation const &);

ResponseInformation const controlPlayerHandler(NetworkListener *,
                                               RequestInformation const &);

ResponseInformation const moveTracksHandler(NetworkListener *,
                                            RequestInformation const &);

ResponseInformation const removeTrackHandler(NetworkListener *,
                                             RequestInformation const &);

#endif  // _REST_ENDPOINT_HANDLERS_H_
