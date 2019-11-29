/*****************************************************************************/
/**
 * @file    JukeBox.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox definition
 */
/*****************************************************************************/

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

  TResult<TSessionID> generateSession(
      std::optional<TPassword> const &pw) override;
  TResult<std::vector<Track>> queryTracks(std::string const &searchPattern,
                                          size_t const nrOfEntries) override;
  // TResult<QueueStatus> getCurrentQueues();
  TResultOpt addTrackToQueue(TSessionID const &sid,
                             TTrackID const &trkid,
                             QueueType type) override;
  TResultOpt voteTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       TVote vote) override;
  TResultOpt removeTrack(TSessionID const &sid, TTrackID const &trkid) override;
  TResultOpt moveTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       QueueType type) override;
  TResultOpt controlPlayer(TSessionID const &sid, PlayerAction action) override;

 private:
  //  LoggingHandler mLog;
  //  ConfigHandler mConfigHandler;
  //  DataStore mDataStore;
  //  NetworkAPI mNetwork;
  //  MusicBackend mMusicBackend;
};

#endif /* _JUKEBOX_H_ */
