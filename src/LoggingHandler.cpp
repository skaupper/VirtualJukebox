/*****************************************************************************/
/**
 * @file    LoggingHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class LoggingHandler implementation
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
//   FLAGS_timestamp_in_logfile_name = false;
  google::InitGoogleLogging("VirtualJukebox");
  isGlogInitialized = true;
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
