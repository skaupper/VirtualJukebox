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
  JukeBox jukebox;
  string configFilePath = "../jukebox_config.ini";
  jukebox.start(configFilePath);

  auto ret = initLogging(argv[0]);
  if (ret.has_value()) {
    cerr << ret.value().getErrorMessage() << endl;
    return 1;
  }

  LOG(INFO) << "Hello world from JukeBox main.cpp !";

  DLOG(INFO) << "Logging the first DEBUG message!";
  LOG(INFO) << "Logging the first INFO message!";
  LOG(WARNING) << "Logging the first WARNING message!";
  LOG(ERROR) << "Logging the first ERROR message!";

  return 0;
}
