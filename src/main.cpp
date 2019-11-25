//------------------------------------------------------------------------------
// File        : main.cpp
// Author      : Team Server
// Description : Main program for project "Virtual JukeBox"
//-----------------------------------------------------------------

#include <iostream>

#include "JukeBox.h"
#include "LoggingProvider.h"

int main() {
  JukeBox jukebox;
  LoggingProvider log;

  jukebox.start();
  log.logInfo("Logging the first info message!");
  log.logError("Logging the first error message!");

  std::cout << "Hello world" << std::endl;
  return 0;
}
