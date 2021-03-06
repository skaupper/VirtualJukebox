/*****************************************************************************/
/**
 * @file    LoggingHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   LoggingHandler implementation
 */
/*****************************************************************************/

#include "Utils/LoggingHandler.h"

#include <glog/logging.h>

#include <string>

#include "Utils/ConfigHandler.h"

using namespace std;

static bool isGlogInitialized = false;

static int getMinLogLevel() {
  string const section = "MainParams";
  string const key = "minLogLevel";

  auto ret = ConfigHandler::getInstance()->getValueString(section, key);
  if (holds_alternative<Error>(ret))
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: " << get<Error>(ret).getErrorMessage() << endl;

  string logLevelStr = get<string>(ret);

  if (logLevelStr == "INFO") {
    return google::INFO;
  } else if (logLevelStr == "WARNING") {
    return google::WARNING;
  } else if (logLevelStr == "ERROR") {
    return google::ERROR;
  } else {
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: LoggingHandler.getMinLogLevel: Configuration value '"
         << section << "." << key
         << "' is invalid. Using INFO as a default fallback." << endl;
    return google::INFO;
  }
}

void initLoggingHandler(string const& exe) {
  if (!ConfigHandler::getInstance()->isInitialized()) {
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: LoggingHandler.initLoggingHandler: "
            "ConfigHandler is unitialized "
         << endl;
  } else if (!isGlogInitialized) {
    FLAGS_log_dir = "./";
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;

    FLAGS_minloglevel = getMinLogLevel();

    google::InitGoogleLogging(exe.c_str());

    isGlogInitialized = true;
  } else {
    LOG(ERROR) << "GoogleLogging was already initialized!";
  }
}
