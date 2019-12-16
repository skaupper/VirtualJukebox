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

/**
 * @brief Template class which can be used as a central class for serialization
 * routines for different data types.
 */
class Serializer {
 public:
  template <class T>
  static nlohmann::json serialize(T const &);
};

#endif /* _SERIALIZER_H_ */
