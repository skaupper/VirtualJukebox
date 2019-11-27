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
#include "NetworkListener.h"
#include "Queue.h"
#include "Result.h"
#include "Track.h"

//#include "ConfigHandler.h"
//#include "LoggingHandler.h"
//#include "DataStore.h"
//#include "MusicBackend.h"
//#include "NetworkAPI.h"
//#include "User.h"

class JukeBox : public NetworkListener {
 public:
  void start(std::string configFilePath);

  TResult<TSessionID> generateSession(TPassword pw);
  TResult<std::vector<Track>> queryTracks(std::string searchPattern);
  // TResult<QueueStatus> getCurrentQueues();
  TResultOpt addTrackToQueue(TSessionID sid, TTrackID trkid, QueueType type);
  TResultOpt voteTrack(TSessionID sid, TTrackID trkid, TVote vote);
  TResult<Track> removeTrack(TSessionID sid, TTrackID trkid);
  TResult<Track> moveTrack(TSessionID sid, TTrackID trkid);
  TResultOpt controlPlayer(TSessionID sid, PlayerAction action);

 private:
  //  LoggingHandler mLog;
  //  ConfigHandler mConfigHandler;
  //  DataStore mDataStore;
  //  NetworkAPI mNetwork;
  //  MusicBackend mMusicBackend;
};

#endif /* _JUKEBOX_H_ */
