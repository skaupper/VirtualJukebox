/*****************************************************************************/
/**
 * @file    main.cpp
 * @author  Team Server
 * @brief   Main program for project "Virtual JukeBox"
 * @details Usage: ./executable [<filepath-to-config-file.ini>]
 */
/*****************************************************************************/

#include <glog/logging.h>

#include <iostream>
#include <string>

#include "JukeBox.h"
#include "Types/Result.h"
#include "Utils/ConfigHandler.h"
#include "Utils/LoggingHandler.h"

using namespace std;

int main(int argc, char* argv[]) {
  string configFilePath = "../jukebox_config.ini";
  if (argc > 1) {
    configFilePath = argv[1];
  } else {
    cout << "INFO: No filename was specified for *.ini configuration file. "
         << "Using '" << configFilePath << "' as a default fallback." << endl;
  }
  JukeBox jukebox;

  // clang-format off
  LOG(INFO) << "#########################################################################\n";
  LOG(INFO) << " __     ___      _               _     _       _        _                ";
  LOG(INFO) << " \\ \\   / (_)_ __| |_ _   _  __ _| |   | |_   _| | _____| |__   _____  __ ";
  LOG(INFO) << "  \\ \\ / /| | '__| __| | | |/ _` | |_  | | | | | |/ / _ \\ '_ \\ / _ \\ \\/ / ";
  LOG(INFO) << "   \\ V / | | |  | |_| |_| | (_| | | |_| | |_| |   <  __/ |_) | (_) >  <  ";
  LOG(INFO) << "    \\_/  |_|_|   \\__|\\__,_|\\__,_|_|\\___/ \\__,_|_|\\_\\___|_.__/ \\___/_/\\_\\ ";
  LOG(INFO) << "";
  LOG(INFO) << "#########################################################################";
  LOG(INFO) << "Compiled at: " << __DATE__ << " " << __TIME__;
  LOG(INFO) << "#########################################################################";
  // clang-format on

  if (!jukebox.start(argv[0], configFilePath)) {
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: Exiting program due to fatal error in Jukebox.start()"
         << endl;
    return 1;
  }

  return 0;
}
