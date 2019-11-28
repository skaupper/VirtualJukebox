/**
 * @file    RestAPI.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class RestAPI
 */

#ifndef REST_API_H_INCLUDED
#define REST_API_H_INCLUDED

#include <httpserver.hpp>
#include <memory>

#include "NetworkAPI.h"

/**
 * @class   RestAPI
 * @brief   Implementation of the a REST API.
 */
class RestAPI : public NetworkAPI {
 public:
  TResultOpt handleRequests() override;

 private:
  void decodeAndDispatch(std::string, std::string, std::string);

  std::unique_ptr<httpserver::webserver> ws;
};

#endif  // REST_API_H_INCLUDED
