//------------------------------------------------------------------------------
// File        : ConfigHandler.h
// Author      : Michael Wurm <wurm.michael95@gmail.com>
// Description : Class ConfigHandler definition
//------------------------------------------------------------------------------

#ifndef _CONFIGHANDLER_H_
#define _CONFIGHANDLER_H_

#include <string>

class ConfigHandler {
 public:
  void loadConfig(std::string filename);

 private:
  void readConfigFile(std::string filename);
};

#endif /* _CONFIGHANDLER_H_ */
