/**
 * @file    NetworkAPI.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of class NetworkAPI
 */

#include "NetworkAPI.h"

#include <cassert>

NetworkAPI::NetworkAPI() : listener(nullptr) {
}

void NetworkAPI::setListener(NetworkListener *listener) {
  assert(listener);
  this->listener = listener;
}
