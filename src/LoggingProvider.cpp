//------------------------------------------------------------------------------
// File        : LoggingProvider.h
// Author      : Michael Wurm <wurm.michael95@gmail.com>
// Description : Class LoggingProvider implementation
//------------------------------------------------------------------------------

#include "LoggingProvider.h"

#include <iostream>

using namespace std;

void LoggingProvider::logInfo(std::string msg) {
  cout << msg << endl;
}

void LoggingProvider::logError(std::string msg) {
  cout << msg << endl;
}
