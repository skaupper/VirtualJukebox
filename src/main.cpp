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
  if (!jukebox.start(argv[0], configFilePath)) {
    cerr << "ERROR: Exiting program due to fatal error in Jukebox.start()"
         << endl;
    return 1;
  }

  DLOG(INFO) << "Logging a DEBUG message!";
  LOG(INFO) << "Logging an INFO message!";
  LOG(WARNING) << "Logging a WARNING message!";
  LOG(ERROR) << "Logging an ERROR message!";

  return 0;
}
