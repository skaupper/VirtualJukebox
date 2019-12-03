/*****************************************************************************/
/**
 * @file    Result.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Implements functions
 */
/*****************************************************************************/

#include "Result.h"

bool checkOptionalError(TResultOpt ret) {
  if (ret.has_value()) {
    ret.value().getErrorCode();
    std::cerr << "Error message is: " << ret.value().getErrorMessage()
              << std::endl;
    return true;
  }
  return false;
}

template <class GOOD_TYPE>
bool checkAlternativeError(TResult<GOOD_TYPE> ret) {
  if (std::holds_alternative<Error>(ret)) {
    std::get<Error>(ret).getErrorCode();
    std::cerr << "Error message is: " << std::get<Error>(ret).getErrorMessage()
              << std::endl;
    return true;
  }
  return false;
}
