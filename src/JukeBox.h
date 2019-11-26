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

//#include "ConfigHandler.h"
//#include "LoggingProvider.h"
//#include "MemoryAPI.h"
//#include "MusicBackend.h"
//#include "MusicPlayer.h"
//#include "NetworkAPI.h"
//#include "Track.h"
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
  // TResult<std::vector<Track>> queryMusic(std::string searchPattern);
  // TResult<Queues> getCurrentQueues();
  TResult<void> addTrackToQueue(TSessionID sid, TTrackID trkid, QueueType type);
  TResult<void> voteTrack(TSessionID sid, TTrackID trkid);
  TResult<void> controlPlayer(TSessionID sid, PlayerAction action);

  //  LoggingProvider mLog;
  //  ConfigHandler mConfigHandler;
  //  MemoryAPI mMemory;
  //  NetworkAPI mNetwork;
  //  MusicBackend mMusicBackend;
  //  MusicPlayer mPlayer;
};

#endif /* _JUKEBOX_H_ */
