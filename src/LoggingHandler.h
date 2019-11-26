/** ----------------------------------------------------------------------------
 * @file    LoggingHandler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class LoggingHandler definition
 * ---------------------------------------------------------------------------*/

#ifndef _LoggingHandler_H_
#define _LoggingHandler_H_

#include <string>

class LoggingHandler {
 public:
  void logInfo(std::string msg);
  void logError(std::string msg);
};

#endif /* _LoggingHandler_H_ */
