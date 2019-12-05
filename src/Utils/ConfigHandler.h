/*****************************************************************************/
/**
 * @file    Utils/ConfigHandler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class ConfigHandler definition
 */
/*****************************************************************************/

#ifndef _CONFIGHANDLER_H_
#define _CONFIGHANDLER_H_

#include <stdlib.h>

#include <memory>
#include <string>

#include "../lib/SimpleIni/SimpleIni.h"
#include "Types/Result.h"

class ConfigHandler {
 public:
  ~ConfigHandler() = default;
  static std::shared_ptr<ConfigHandler> const getInstance();

  TResultOpt setConfigFilePath(std::string const& filepath);
  TResult<std::string> getValueString(std::string const& section,
                                      std::string const& key);
  TResult<int> getValueInt(std::string const& section, std::string const& key);
  bool isInitialized();

 private:
  ConfigHandler() = default;                                // hide default ctor
  ConfigHandler(ConfigHandler const&) = delete;             // delete copy ctor
  ConfigHandler& operator=(ConfigHandler const&) = delete;  // assignment ctor

  static std::shared_ptr<ConfigHandler> mInstance;
  std::string mConfigFilePath;

  CSimpleIniA mIni;
  bool mIsInitialized = false;
};

#endif /* _CONFIGHANDLER_H_ */
