/*****************************************************************************/
/**
 * @file    ConfigHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class ConfigHandler implementation
 */
/*****************************************************************************/

#include "ConfigHandler.h"

#include <string>

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
TResultOpt ConfigHandler::setConfigFilePath(string const& filepath) {
  mConfigFilePath = filepath;

  mIni.SetUnicode(false);    // use OS native encoding
  mIni.SetMultiKey(false);   // don't support duplicated keys
  mIni.SetMultiLine(false);  // don't support multiline values for a key

  SI_Error rc = mIni.LoadFile(mConfigFilePath.c_str());
  if (rc < 0)
    return Error(
        ErrorCode::FileNotFound,
        "ConfigHandler.getValueString: Couldn't load file '" + filepath + "'.");
  mIsInitialized = true;
  return nullopt;
}

/** @brief Returns value of a key as a string
 */
TResult<string> ConfigHandler::getValueString(string const& section,
                                              string const& key) {
  if (!mIsInitialized) {
    return Error(ErrorCode::NotInitialized,
                 "ConfigHandler.getValueString: ConfigHandler is not "
                 "initialized. Call setConfigFilePath() first.");
  }
  const char* val = mIni.GetValue(section.c_str(), key.c_str(), nullptr);
  if (!val) {
    return Error(ErrorCode::KeyNotFound,
                 "ConfigHandler.getValueString: Key '" + key +
                     "' not found in section '" + section + "'.");
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
TResult<int> ConfigHandler::getValueInt(string const& section,
                                        string const& key) {
  if (!mIsInitialized) {
    return Error(ErrorCode::NotInitialized,
                 "ConfigHandler.getValueString: ConfigHandler is not "
                 "initialized. Call setConfigFilePath() first.");
  }
  auto valObj = getValueString(section, key);
  if (holds_alternative<Error>(valObj))
    return get<Error>(valObj);

  string valStr = get<string>(valObj);

  int valInt;
  size_t idx;
  try {
    valInt = stoi(valStr, &idx);
  } catch (invalid_argument const&) {
    return Error(ErrorCode::InvalidFormat,
                 "ConfigHandler.getValueInt: Configuration value '" + section +
                     "." + key + "' is not an integer.");
  }

  if (idx != valStr.size()) {
    return Error(ErrorCode::InvalidFormat,
                 "ConfigHandler.getValueInt: Configuration value '" + section +
                     "." + key + "' must only consist of digits.");
  }
  return valInt;
}

bool ConfigHandler::isInitialized() {
  return mIsInitialized;
}
