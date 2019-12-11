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

  string const pw = "awesome4711password";
  string const nickname = "theNickname";
  auto ret = jb.generateSession(pw, nickname);
  ASSERT_EQ(checkAlternativeError(ret), false);

  /* This check is assuming a fast enough execution time,
   * so that the session ID generation happens within the same
   * second as the check here. */
  string value = get<string>(ret);
  string expected = "ID0" + to_string(time(nullptr));
  EXPECT_EQ(value, expected);
}
