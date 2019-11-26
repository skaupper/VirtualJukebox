/** ----------------------------------------------------------------------------
 * @file    Result.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Defines function return values and types
 * ---------------------------------------------------------------------------*/

#ifndef _RESULT_H_
#define _RESULT_H_

#include <string>
#include <variant>

/**
 * @brief A customized error type
 */
class Error {
 public:
  virtual ~Error() {
  }

  virtual int getErrorCode() = 0;
  virtual std::string getErrorMessage() = 0;
};

/**
 * @typedef TResult
 * @brief Used as return type of functions.
 *
 * Abstracts away the need to specify `std::variant` for the default return type
 * and the alternative error type for the respective function (which is always
 * the same).
 */
template <class GOOD_TYPE>
using TResult = std::variant<GOOD_TYPE, TError>;

#endif /* _RESULT_H_ */
