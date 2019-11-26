/** ----------------------------------------------------------------------------
 * @file    main.cpp
 * @author  Team Server
 * @brief   Main program for project "Virtual JukeBox"
 * ---------------------------------------------------------------------------*/

#include <iostream>
#include <string>

#include "ConfigHandler.h"
#include "JukeBox.h"
#include "LoggingHandler.h"

using namespace std;

int main() {
  JukeBox jukebox;
  LoggingHandler log;
  ConfigHandler conf;

  cout << "Hello world from JukeBox main.cpp !" << endl;

  string configFilePath = "config_file.ini";
  jukebox.start(configFilePath);

  log.logInfo("Logging the first info message!");
  log.logError("Logging the first error message!");

  conf.loadConfig("./jukebox_config.ini");

  return 0;
}
