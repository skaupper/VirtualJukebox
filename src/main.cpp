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

int main(int argc, char* argv[]) {
  (void)argc;

  JukeBox jukebox;
  string configFilePath = "../jukebox_config.ini";
  jukebox.start(argv[0], configFilePath);

  LOG(INFO) << "Hello world from JukeBox main.cpp !";

  DLOG(INFO) << "Logging the first DEBUG message!";
  LOG(INFO) << "Logging the first INFO message!";
  LOG(WARNING) << "Logging the first WARNING message!";
  LOG(ERROR) << "Logging the first ERROR message!";

  return 0;
}
