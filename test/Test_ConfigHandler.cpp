#include <gtest/gtest.h>

#include <iostream>
#include <memory>

#include "../src/ConfigHandler.h"
#include "../src/Result.h"

using namespace std;

TEST(ConfigHandler, getValueString_HappyCase) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "ip";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  auto setfile = conf->setConfigFilePath(configFilePath);
  ASSERT_EQ(checkOptionalError(setfile), false);

  TResult<string> ret = conf->getValueString(section, key);

  bool error = holds_alternative<Error>(ret);  // Var 1: This works
  ASSERT_EQ(error, false);                     // Var 1: This works
  // ASSERT_EQ(checkAlternativeError(ret), false);  // Var 2: invalid pointer

  string value = get<string>(ret);
  EXPECT_EQ(value, "192.168.0.101");
}

TEST(ConfigHandler, getValueString_FileNotFound) {
  string const configFilePath = "this_file_does_not_exist.ini";
  string const section = "MainParams";
  string const key = "ip";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  auto setfile = conf->setConfigFilePath(configFilePath);
  ASSERT_EQ(checkOptionalError(setfile), true);
}

TEST(ConfigHandler, getValueInt_HappyCase) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "port";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  auto setfile = conf->setConfigFilePath(configFilePath);
  ASSERT_EQ(checkOptionalError(setfile), false);

  TResult<int> ret = conf->getValueInt(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, false);

  int value = get<int>(ret);
  EXPECT_EQ(value, 4711);
}

TEST(ConfigHandler, getValueInt_InvalidKeyFormat) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "wrongFormat";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  auto setfile = conf->setConfigFilePath(configFilePath);
  ASSERT_EQ(checkOptionalError(setfile), false);

  TResult<int> ret = conf->getValueInt(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, true);
  EXPECT_EQ(get<Error>(ret).getErrorCode(),
            ErrorCode::KeyNotFoundOrInvalidKeyFormat);
  cout << "Error message is: " << get<Error>(ret).getErrorMessage() << endl;
}

TEST(ConfigHandler, getValueInt_KeyNotFound) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "this_key_does_not_exist";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  auto setfile = conf->setConfigFilePath(configFilePath);
  ASSERT_EQ(checkOptionalError(setfile), false);

  TResult<int> ret = conf->getValueInt(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, true);
  EXPECT_EQ(get<Error>(ret).getErrorCode(),
            ErrorCode::KeyNotFoundOrInvalidKeyFormat);
  cout << "Error message is: " << get<Error>(ret).getErrorMessage() << endl;
}

TEST(ConfigHandler, getValueString_KeyNotFound) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "this_key_does_not_exist";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  auto setfile = conf->setConfigFilePath(configFilePath);
  ASSERT_EQ(checkOptionalError(setfile), false);

  TResult<string> ret = conf->getValueString(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, true);
  EXPECT_EQ(get<Error>(ret).getErrorCode(), ErrorCode::KeyNotFound);
  cout << "Error message is: " << get<Error>(ret).getErrorMessage() << endl;
}
