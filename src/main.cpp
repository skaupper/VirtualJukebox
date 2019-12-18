/*****************************************************************************/
/**
 * @file    main.cpp
 * @author  Team Server
 * @brief   Main program for project "VirtualJukeBox"
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
  // LINTING IS MESSED UP HERE:
  }

  else
     {
    cout << "INFO: No filename was specified for *.ini configuration file. "
         << "Using '" << configFilePath << "' as a default fallback." << endl;
  }





  JukeBox jukebox;
  if (!jukebox.start(argv[0], configFilePath)) {
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: Exiting program due to fatal error in Jukebox.start()"
         << endl;
    return 1;
  }

  return 0;
}
