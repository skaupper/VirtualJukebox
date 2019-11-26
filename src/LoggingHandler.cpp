/** ----------------------------------------------------------------------------
 * @file    LoggingHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class LoggingHandler implementation
 * ---------------------------------------------------------------------------*/

#include <iostream>

#include "LoggingHandler.h"

using namespace std;

void LoggingHandler::logInfo(string msg) {
  cout << msg << endl;
}

void LoggingHandler::logError(string msg) {
  cout << msg << endl;
}
