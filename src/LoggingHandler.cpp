/*****************************************************************************/
/**
 * @file    LoggingHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   LoggingHandler implementation
 */
/*****************************************************************************/

#include "LoggingHandler.h"

#include <glog/logging.h>

using namespace std;

static bool isGlogInitialized = false;

static void initGlog() {
  FLAGS_log_dir = "./";
  FLAGS_alsologtostderr = true;
  FLAGS_colorlogtostderr = true;
  google::InitGoogleLogging("VirtualJukebox");

  isGlogInitialized = true;
}

void logDebug(std::string const& msg) {
  if (!isGlogInitialized)
    initGlog();
  DLOG(INFO) << msg;
}

void logInfo(std::string const& msg) {
  if (!isGlogInitialized)
    initGlog();
  LOG(INFO) << msg;
}

void logWarning(std::string const& msg) {
  if (!isGlogInitialized)
    initGlog();
  LOG(WARNING) << msg;
}

void logError(std::string const& msg) {
  if (!isGlogInitialized)
    initGlog();
  LOG(ERROR) << msg;
}
