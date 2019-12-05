/*****************************************************************************/
/**
 * @file    Serializer.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class Serializer
 */
/*****************************************************************************/
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "json/json.hpp"

class Serializer {
 public:
  template <class T>
  static nlohmann::json serialize(T const &);
};

#endif /* _SERIALIZER_H_ */
