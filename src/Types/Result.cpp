/*****************************************************************************/
/**
 * @file    Result.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implements result helper functions
 */
/*****************************************************************************/

#include "Result.h"


bool checkOptionalError(TResultOpt& ret) {
  if (ret.has_value()) {
    LOG(ERROR) << "Error message is: " << ret.value().getErrorMessage();
    return true;
  }
  return false;
}
