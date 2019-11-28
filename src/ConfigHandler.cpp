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

shared_ptr<ConfigHandler> const ConfigHandler::getInstance() {
  if (mInstance == nullptr)
    mInstance = shared_ptr<ConfigHandler>(new ConfigHandler);
  return mInstance;
}

void ConfigHandler::setConfigFilePath(string filepath) {
  mConfigFilePath = filepath;
}

TResult<string> ConfigHandler::getValueString(string section, string key) {
  bool const isUtf8 = false;        // use OS native encoding
  bool const useMultiKey = false;   // don't support duplicated keys
  bool const useMultiLine = false;  // don't support multiline values for a key

  CSimpleIniA ini(isUtf8, useMultiKey, useMultiLine);
  SI_Error rc = ini.LoadFile(mConfigFilePath.c_str());
  if (rc < 0)
    return Error(ErrorCode::FileNotFound,
                 "ConfigHandler.getValue: couldn't load file");

  return ini.GetValue(section.c_str(), key.c_str());
}

TResult<int> ConfigHandler::getValueInt(string section, string key) {
  bool const isUtf8 = false;        // use OS native encoding
  bool const useMultiKey = false;   // don't support duplicated keys
  bool const useMultiLine = false;  // don't support multiline values for a key

  CSimpleIniA ini(isUtf8, useMultiKey, useMultiLine);
  SI_Error rc = ini.LoadFile(mConfigFilePath.c_str());
  if (rc < 0)
    return Error(ErrorCode::FileNotFound,
                 "ConfigHandler.getValue: couldn't load file");

  string valStr = ini.GetValue(section.c_str(), key.c_str());

  int valInt;
  if (sscanf(valStr.c_str(), "%d", &valInt) == 1)
    cout << "valInt = " << valInt << endl;
  else
    cerr << "valInt = BAD INPUT! " << endl;

  return valInt;
}
