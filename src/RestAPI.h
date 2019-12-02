/*****************************************************************************/
/**
 * @file    RestAPI.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class RestAPI
 */
/*****************************************************************************/

#ifndef _REST_API_H_
#define _REST_API_H_

#include <httpserver.hpp>
#include <memory>

#include "NetworkAPI.h"

/**
 * @class RestAPI
 * @brief Implementation of the REST API.
 * @sa    NetworkAPI, NetworkListener
 */
class RestAPI : public NetworkAPI {
 public:
  TResultOpt handleRequests() override;
};

#endif /* _REST_API_H_ */
