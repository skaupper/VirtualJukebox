/** ----------------------------------------------------------------------------
 * @file    ConfigHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class ConfigHandler implementation
 * ---------------------------------------------------------------------------*/

#include "ConfigHandler.h"

#include <iostream>

using namespace std;

/* Initialize static member */
shared_ptr<ConfigHandler> ConfigHandler::mInstance{nullptr};

shared_ptr<ConfigHandler> const ConfigHandler::getInstance() {
  if (mInstance == nullptr)
    mInstance = shared_ptr<ConfigHandler>(new ConfigHandler);
  return mInstance;
}

/** @brief Configures file path to *.ini file, configures the SimpleIni reader
 * and loads the ini-file.
 */
TResultOpt ConfigHandler::setConfigFilePath(string filepath) {
  mConfigFilePath = filepath;

  mIni.SetUnicode(false);    // use OS native encoding
  mIni.SetMultiKey(false);   // don't support duplicated keys
  mIni.SetMultiLine(false);  // don't support multiline values for a key

  SI_Error rc = mIni.LoadFile(mConfigFilePath.c_str());
  if (rc < 0)
    return Error(ErrorCode::FileNotFound,
                 "ConfigHandler.getValueString: couldn't load file.");
}

/** @brief Returns value of a key as a string
 */
TResult<string> ConfigHandler::getValueString(string section, string key) {
  const char* val = mIni.GetValue(section.c_str(), key.c_str(), nullptr);
  if (!val) {
    string errmsg = "ConfigHandler.getValueString: Key " + key +
                    " not found in section " + section;
    return Error(ErrorCode::KeyNotFound, errmsg);
  }
  return val;
}

/** @brief Returns value of a key as integer
 * @details The following restrictions are given with respect to the format
 * of a key.
 * [MainParams]
 * goodFormat=4711
 * wrongFormat1=xx4711
 * wrongFormat2=4711xx
 * wrongFormat3=47xx11
 */
TResult<int> ConfigHandler::getValueInt(string section, string key) {
  int val = mIni.GetLongValue(section.c_str(), key.c_str(), INT32_MAX);

  if (val == INT32_MAX) {
    string errmsg = "ConfigHandler.getValueInt: section " + section + ", key " +
                    key + " not found, or has invalid parameter format.";
    return Error(ErrorCode::KeyNotFoundOrInvalidKeyFormat, errmsg);
  }
  return val;
}
