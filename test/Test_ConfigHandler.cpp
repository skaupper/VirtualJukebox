#include <gtest/gtest.h>

#include <iostream>

#include "../src/ConfigHandler.h"
#include "../src/Result.h"

using namespace std;

TEST(ConfigHandler, getValue_HappyCase) {
  string const configFilePath = "../test/test_config.ini";
  // string const configFilePath = "../jukebox_config.ini";
  string const section = "MainParams";
  string const key = "ip";

  ConfigHandler& conf = ConfigHandler::GetInstance();
  conf.setConfigFilePath(configFilePath);
  TResult<string> ret = conf.getValue(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, false);

  string value = get<string>(ret);
  EXPECT_TRUE(value == "192.168.0.101");
}

TEST(ConfigHandler, getValue_FileNotFound) {
  string const configFilePath = "this_file_does_not_exist.ini";
  string const section = "MainParams";
  string const key = "ip";

  ConfigHandler& conf = ConfigHandler::GetInstance();
  conf.setConfigFilePath(configFilePath);

  TResult<string> ret = conf.getValue(section, key);

  bool error = holds_alternative<Error>(ret);
  ASSERT_EQ(error, true);
  EXPECT_TRUE(get<Error>(ret).getErrorCode() == ErrorCode::FileNotFound);
}
