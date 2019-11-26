/** ----------------------------------------------------------------------------
 * @file    JukeBox.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox definition
 * ---------------------------------------------------------------------------*/

#ifndef _JUKEBOX_H_
#define _JUKEBOX_H_

#include <string>
#include <variant>
#include <vector>

#include "Result.h"

//#include "ConfigHandler.h"
//#include "LoggingProvider.h"
//#include "MemoryAPI.h"
//#include "MusicBackend.h"
//#include "MusicPlayer.h"
//#include "NetworkAPI.h"
//#include "Track.h"
//#include "User.h"

class JukeBox {
 public:
  std::variant<bool, Error> start();

 private:
  std::variant<std::string, Error> generateSession();
  std::variant<bool, Error> authenticateAdmin(std::string password);
  // std::variant<std::vector<Track>, Error> queryMusic(std::string name);
  // std::variant<std::vector<Track>, Error> getCurrentQueue(TQueueType queue);
  // std::variant<bool, Error> addTrackToQueue(User user, Track track);
  // std::variant<bool, Error> voteTrack(User user, Track track);
  // std::variant<bool, Error> controlPlayer(User user, TPlayerAction action);

  std::string const mAdminPassword = "admin";

  //  LoggingProvider mLog;
  //  ConfigHandler mConfigHandler;
  //  MemoryAPI mMemory;
  //  NetworkAPI mNetwork;
  //  MusicBackend mMusicBackend;
  //  MusicPlayer mPlayer;
};

#endif /* _JUKEBOX_H_ */
