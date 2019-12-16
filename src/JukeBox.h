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

#include "Datastore/DataStore.h"
#include "MusicBackend.h"
#include "Network/NetworkAPI.h"
#include "NetworkListener.h"
#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"
#include "Utils/SimpleScheduler.h"

class JukeBox : public NetworkListener {
 public:
  JukeBox();
  ~JukeBox();

  /**
   * @brief Initializes ConfigHandler and LoggingHandler.
   * @param exeName Name of the currently running executable.
   * @param configFilePath Path to *.ini config file, relative to executable.
   * @return True on success, false otherwise.
   */
  bool start(std::string const &exeName, std::string const &configFilePath);

  TResult<TSessionID> generateSession(
      std::optional<TPassword> const &pw,
      std::optional<std::string> const &nickname) override;
  TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &searchPattern, size_t const nrOfEntries) override;
  TResult<QueueStatus> getCurrentQueues(TSessionID const &);
  TResultOpt addTrackToQueue(TSessionID const &sid,
                             TTrackID const &trkid,
                             QueueType type) override;
  TResultOpt voteTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       TVote vote) override;
  TResultOpt removeTrack(TSessionID const &sid, TTrackID const &trkid) override;
  TResultOpt moveTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       QueueType toQueue) override;
  TResultOpt controlPlayer(TSessionID const &sid, PlayerAction action) override;

 private:
  DataStore *mDataStore;
  NetworkAPI *mNetwork;
  MusicBackend *mMusicBackend;
  SimpleScheduler *mScheduler;

  unsigned const sessionTimeoutAfterSeconds = 3600;
};

#endif /* _JUKEBOX_H_ */
