<<<<<<< HEAD
/*****************************************************************************/
=======
>>>>>>> squash for rebase
/**
 * @file    NetworkAPI.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class NetworkAPI
 */
<<<<<<< HEAD
/*****************************************************************************/

#ifndef _NETWORK_API_H_
#define _NETWORK_API_H_
=======

#ifndef NETWORK_API_H_INCLUDED
#define NETWORK_API_H_INCLUDED
>>>>>>> squash for rebase

#include "NetworkListener.h"

/**
 * @class NetworkAPI
 * @brief Base class for all implementations of network interfaces.
<<<<<<< HEAD
 *
 * @details Creates a webserver and notifies a listener about (valid) incoming
 * requests using the observer pattern.
=======
>>>>>>> squash for rebase
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

<<<<<<< HEAD
 protected:
  NetworkListener *listener;
};

#endif /* _NETWORK_API_H_ */
=======
  NetworkListener *listener;
};

#endif  // NETWORK_API_H_INCLUDED
>>>>>>> squash for rebase
