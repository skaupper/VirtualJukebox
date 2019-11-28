/** ----------------------------------------------------------------------------
 * @file    ConfigHandler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class ConfigHandler definition
 * ---------------------------------------------------------------------------*/

#ifndef _CONFIGHANDLER_H_
#define _CONFIGHANDLER_H_

#include <memory>
#include <string>

#include "../lib/SimpleIni/SimpleIni.h"
#include "Result.h"

class ConfigHandler {
 public:
  ~ConfigHandler() = default;
  static std::shared_ptr<ConfigHandler> const getInstance();

  void setConfigFilePath(std::string filepath);
  TResult<std::string> getValue(std::string section, std::string key);

 private:
  ConfigHandler() = default;                                // hide default ctor
  ConfigHandler(ConfigHandler const&) = delete;             // delete copy ctor
  ConfigHandler& operator=(ConfigHandler const&) = delete;  // assignment ctor

  static std::shared_ptr<ConfigHandler> mInstance;
  std::string mConfigFilePath;
};

#endif /* _CONFIGHANDLER_H_ */
