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

#include "GlobalTypes.h"
#include "Result.h"
#include "Track.h"

//#include "ConfigHandler.h"
//#include "LoggingHandler.h"
//#include "MemoryAPI.h"
//#include "MusicBackend.h"
//#include "MusicPlayer.h"
//#include "NetworkAPI.h"
//#include "User.h"
//#include "Queue.h"

/**
 * @brief Type for multiple queues
 */
// using Queues = Queue[2];

class JukeBox {
 public:
  void start(std::string configFilePath);

 private:
  TResult<TSessionID> generateSession(TPassword pw);
  TResult<std::vector<Track>> queryMusic(std::string searchPattern);
  // TResult<Queues> getCurrentQueues();
  TResultOpt addTrackToQueue(TSessionID sid, TTrackID trkid, QueueType type);
  TResultOpt voteTrack(TSessionID sid, TTrackID trkid);
  TResultOpt controlPlayer(TSessionID sid, PlayerAction action);

  //  LoggingHandler mLog;
  //  ConfigHandler mConfigHandler;
  //  MemoryAPI mMemory;
  //  NetworkAPI mNetwork;
  //  MusicBackend mMusicBackend;
  //  MusicPlayer mPlayer;
};

#endif /* _JUKEBOX_H_ */
