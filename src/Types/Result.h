/*****************************************************************************/
/**
 * @file    Result.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Defines function return values and types
 */
/*****************************************************************************/

#ifndef _RESULT_H_
#define _RESULT_H_

#include <glog/logging.h>

#include <iostream>
#include <optional>
#include <string>
#include <variant>

/**
 * @brief Valid error codes enumerator
 */
enum class ErrorCode {
  AccessDenied,
  SessionExpired,
  FileNotFound,
  InvalidFormat,
  InvalidValue,
  KeyNotFound,
  NotImplemented,
  NotInitialized,
  SpotifyNotFound,
  SpotifyAccessDenied,
  SpotifyAccessExpired,
  SpotifyForbidden,
  SpotifyParseError,
  SpotifyAPIError,
  SpotifyBadRequest,
  SpotifyHttpTimeout,
  SpotifyNoDevice,
  AlreadyExists,
  DoesntExist
};

/**
 * @brief A customized error type
 */
class Error {
 public:
  Error(ErrorCode code, std::string msg) : mCode(code), mMsg(msg) {
  }

  ErrorCode getErrorCode() const {
    return mCode;
  }

  std::string getErrorMessage() const {
    return mMsg;
  }

 private:
  ErrorCode mCode;
  std::string mMsg;
};

/**
 * @typedef TResult
 * @brief Used as return type of functions.
 *
 * Abstracts away the need to specify `std::variant`.
 * By default, functions return the type specified in GOOD_TYPE.
 * In the case of an error, the alternative type can be set.
 */
template <class GOOD_TYPE>
using TResult = std::variant<GOOD_TYPE, Error>;

/**
 * @typedef TResultOpt
 * @brief Used as an optional return type of functions.
 *
 * Abstracts away the need to specify `std::optional`.
 * By default, functions do not return a value.
 * In the case of an error, the optional type can be returned.
 */
using TResultOpt = std::optional<Error>;

/** @brief Checks if given parameter contains optional error value.
 *         If so, it prints the containing error message.
 * @return true if parameter contains error type, false otherwise
 */
static inline bool checkOptionalError(TResultOpt& ret) {
  if (ret.has_value()) {
    LOG(ERROR) << "Error message is: " << ret.value().getErrorMessage();
    return true;
  }
  return false;
}

/** @brief Checks if given parameter contains alternative error value.
 *         If so, it prints the containing error message.
 * @return true if parameter contains error type, false otherwise
 */
template <class GOOD_TYPE>
bool checkAlternativeError(TResult<GOOD_TYPE>& ret) {
  if (std::holds_alternative<Error>(ret)) {
    LOG(ERROR) << "Error message is: "
               << std::get<Error>(ret).getErrorMessage();
    return true;
  }
  return false;
}

#endif /* _RESULT_H_ */
