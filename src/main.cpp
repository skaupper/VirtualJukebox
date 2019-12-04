/*****************************************************************************/
/**
 * @file    main.cpp
 * @author  Team Server
 * @brief   Main program for project "Virtual JukeBox"
 */
/*****************************************************************************/

#include <glog/logging.h>

#include <iostream>
#include <string>

#include "ConfigHandler.h"
#include "JukeBox.h"
#include "LoggingHandler.h"
#include "Result.h"

using namespace std;

int main() {
  JukeBox jukebox;

  initLogging();

  LOG(INFO) << "Hello world from JukeBox main.cpp !";

  string configFilePath = "../jukebox_config.ini";
  jukebox.start(configFilePath);

  DLOG(INFO) << "Logging the first DEBUG message!";
  LOG(INFO) << "Logging the first INFO message!";
  LOG(WARNING) << "Logging the first WARNING message!";
  LOG(ERROR) << "Logging the first ERROR message!";

  return 0;
}
