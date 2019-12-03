/*****************************************************************************/
/**
 * @file    main.cpp
 * @author  Team Server
 * @brief   Main program for project "Virtual JukeBox"
 */
/*****************************************************************************/

#include <iostream>
#include <string>

#include "ConfigHandler.h"
#include "JukeBox.h"
#include "LoggingHandler.h"
#include "Result.h"

using namespace std;

int main() {
  JukeBox jukebox;

  logInfo("Hello world from JukeBox main.cpp !");

  string configFilePath = "../jukebox_config.ini";
  jukebox.start(configFilePath);

  logInfo("Logging the first INFO message!");
  logWarning("Logging the first WARNING message!");
  logError("Logging the first ERROR message!");

  return 0;
}
