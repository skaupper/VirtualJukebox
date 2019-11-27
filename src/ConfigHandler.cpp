/** ----------------------------------------------------------------------------
 * @file    ConfigHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class ConfigHandler implementation
 * ---------------------------------------------------------------------------*/

#include "ConfigHandler.h"

#include <iostream>

using namespace std;

/*  Initialize static member */
shared_ptr<ConfigHandler> ConfigHandler::mInstance{nullptr};

ConfigHandler& ConfigHandler::GetInstance() {
  if (mInstance == nullptr)
    mInstance = shared_ptr<ConfigHandler>(new ConfigHandler);
  return *mInstance;
}

void ConfigHandler::setConfigFilePath(string filepath) {
  mConfigFilePath = filepath;
}

TResult<string> ConfigHandler::getValue(string section, string key) {
  bool a_bIsUtf8 = false;        // use OS native encoding
  bool a_bUseMultiKey = false;   // don't support duplicated keys
  bool a_bUseMultiLine = false;  // don't support multiline values for a key

  CSimpleIniA ini(a_bIsUtf8, a_bUseMultiKey, a_bUseMultiLine);
  SI_Error rc = ini.LoadFile(mConfigFilePath.c_str());
  if (rc < 0)
    return Error(ErrorCode::FileNotFound,
                 "ConfigHandler.getValue: couldn't load file");

  return ini.GetValue(section.c_str(), key.c_str());
}
