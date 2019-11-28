/*****************************************************************************/
/**
 * @file    NetworkAPI.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class NetworkAPI
 */
/*****************************************************************************/

#ifndef NETWORK_API_H_INCLUDED
#define NETWORK_API_H_INCLUDED

#include "NetworkListener.h"

/**
 * @class NetworkAPI
 * @brief Base class for all implementations of network interfaces.
 *
 * @details Creates a webserver and notifies a listener about (valid) incoming
 * requests using the observer pattern.
 */
class NetworkAPI {
 public:
  NetworkAPI();

  void setListener(NetworkListener *);
  virtual TResultOpt handleRequests() = 0;

 private:
  // dont allow copying
  NetworkAPI(NetworkAPI const &) = delete;
  NetworkAPI &operator=(NetworkAPI const &) = delete;

 protected:
  NetworkListener *listener;
};

#endif  // NETWORK_API_H_INCLUDED
