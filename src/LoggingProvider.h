//------------------------------------------------------------------------------
// File        : LoggingProvider.h
// Author      : Michael Wurm <wurm.michael95@gmail.com>
// Description : Class LoggingProvider definition
//------------------------------------------------------------------------------

#ifndef _LOGGINGPROVIDER_H_
#define _LOGGINGPROVIDER_H_

#include <string>

class LoggingProvider {
 public:
  void logInfo(std::string msg);
  void logError(std::string msg);
};

#endif /* _LOGGINGPROVIDER_H_ */
