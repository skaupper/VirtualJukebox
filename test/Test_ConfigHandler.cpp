#include <gtest/gtest.h>

#include <iostream>
#include <memory>

#include "../src/ConfigHandler.h"
#include "../src/Result.h"

using namespace std;

TEST(ConfigHandler, getValue_HappyCase) {
  string const configFilePath = "../test/test_config.ini";
  // string const configFilePath = "../jukebox_config.ini";
  string const section = "MainParams";
  string const key = "ip";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  conf->setConfigFilePath(configFilePath);
  TResult<string> ret = conf->getValueString(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, false);

  string value = get<string>(ret);
  EXPECT_EQ(value, "192.168.0.101");
}

TEST(ConfigHandler, getValue_FileNotFound) {
  string const configFilePath = "this_file_does_not_exist.ini";
  string const section = "MainParams";
  string const key = "ip";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  conf->setConfigFilePath(configFilePath);

  TResult<string> ret = conf->getValueString(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, true);
  EXPECT_EQ(get<Error>(ret).getErrorCode(), ErrorCode::FileNotFound);
}

TEST(ConfigHandler, getValueInt) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "port";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  conf->setConfigFilePath(configFilePath);
  TResult<int> ret = conf->getValueInt(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, false);

  int value = get<int>(ret);
  EXPECT_EQ(value, 4711);
}

TEST(ConfigHandler, getValueInt_InvalidParameterFormat) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "wrongFormat";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  conf->setConfigFilePath(configFilePath);

  TResult<int> ret = conf->getValueInt(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, true);
  EXPECT_EQ(get<Error>(ret).getErrorCode(), ErrorCode::InvalidParameterFormat);
}

TEST(ConfigHandler, getValueInt_EdgeCaseParameterFormat) {
  string const configFilePath = "../test/test_config.ini";
  string const section = "MainParams";
  string const key = "edgeCaseFormat";

  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  conf->setConfigFilePath(configFilePath);

  TResult<int> ret = conf->getValueInt(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, false);

  int value = get<int>(ret);
  EXPECT_EQ(value, 47);
}
