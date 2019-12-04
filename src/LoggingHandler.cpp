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

void initLogging() {
  FLAGS_log_dir = "./";
  FLAGS_alsologtostderr = true;
  FLAGS_colorlogtostderr = true;
  google::InitGoogleLogging("VirtualJukebox");

  isGlogInitialized = true;
}
