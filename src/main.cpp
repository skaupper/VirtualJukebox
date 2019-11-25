//------------------------------------------------------------------------------
// File        : main.cpp
// Author      : Team Server
// Description : Main program for project "Virtual JukeBox"
//-----------------------------------------------------------------

#include <iostream>

#include "ConfigHandler.h"
#include "JukeBox.h"
#include "LoggingProvider.h"

int main() {
  JukeBox jukebox;
  LoggingProvider log;
  ConfigHandler conf;

  std::cout << "Hello world from JukeBox main.cpp !" << std::endl;

  jukebox.start();

  log.logInfo("Logging the first info message!");
  log.logError("Logging the first error message!");

  conf.loadConfig("./jukebox_config.ini");

  return 0;
}
