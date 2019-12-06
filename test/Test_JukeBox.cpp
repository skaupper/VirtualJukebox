/*****************************************************************************/
/**
 * @file    Test_ConfigHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 *          Team Server
 * @brief   Test implementation for class JukeBox
 */
/*****************************************************************************/

#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <sstream>

#include "../src/JukeBox.h"
#include "../src/Types/Result.h"
#include "../src/Utils/ConfigHandler.h"

using namespace std;

string const configFilePath = "../test/test_config.ini";
string const exeName = "./testVirtualJukebox";

TEST(JukeBox, generateSession) {
  JukeBox jb;
  bool start = jb.start(exeName, configFilePath);
  ASSERT_EQ(start, true);

  string const pw = "awesome4711password";
  string const nickname = "theNickname";
  auto ret = jb.generateSession(pw, nickname);
  ASSERT_EQ(checkAlternativeError(ret), false);

  /* This check is assuming a fast enough execution time,
   * so that the session ID generation happens within the same
   * second as the check here. */
  string value = get<string>(ret);
  EXPECT_EQ(value, to_string(time(nullptr)));
}
