/** ----------------------------------------------------------------------------
 * @file    Result.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Implements functions
 * ---------------------------------------------------------------------------*/

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
