/*****************************************************************************/
/**
 * @file    Test_ConfigHandler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Test implementation for class TrackScheduler
 */
/*****************************************************************************/

#include <gtest/gtest.h>

#include <iostream>
#include <memory>

#include "../src/Types/Result.h"
#include "../src/Utils/TrackScheduler.h"

using namespace std;

TEST(TestScheduler, start) {
  TrackScheduler scheduler(nullptr, nullptr);
  // scheduler.start();
}
