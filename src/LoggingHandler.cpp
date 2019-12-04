/*****************************************************************************/
/**
 * @file    LoggingHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   LoggingHandler implementation
 */
/*****************************************************************************/

#include "LoggingHandler.h"

#include <glog/logging.h>

#include <string>

#include "ConfigHandler.h"

using namespace std;

static bool isGlogInitialized = false;

static TResult<int> getMinLogLevel() {
  string const section = "MainParams";
  string const key = "minLogLevel";

  auto ret = ConfigHandler::getInstance()->getValueString(section, key);
  if (holds_alternative<Error>(ret))
    return get<Error>(ret);

  string logLevelStr = get<string>(ret);

  if (logLevelStr == "INFO") {
    return google::INFO;
  } else if (logLevelStr == "WARNING") {
    return google::WARNING;
  } else if (logLevelStr == "ERROR") {
    return google::ERROR;
  } else {
    return Error(ErrorCode::InvalidValue,
                 "LoggingHandler.getMinLogLevel: Configuration value '" +
                     section + "." + key + "' is invalid.");
  }
}

TResultOpt initLogging(string const& exe) {
  if (!isGlogInitialized) {
    FLAGS_log_dir = "./";
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;

    auto logLevel = getMinLogLevel();
    if (holds_alternative<Error>(logLevel))
      return get<Error>(logLevel);
    FLAGS_minloglevel = get<int>(logLevel);

    google::InitGoogleLogging("VirtualJukebox");

    isGlogInitialized = true;
  } else {
    LOG(ERROR) << "GoogleLogging was already initialized!";
  }
  return nullopt;
}
