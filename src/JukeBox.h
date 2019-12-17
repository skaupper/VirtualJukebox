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

#include "DataStore.h"
#include "MusicBackend.h"
#include "NetworkAPI.h"
#include "NetworkListener.h"
#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"
#include "Utils/SimpleScheduler.h"

/**
 * @brief Core class which combines all interface implementations in a working
 * server application.
 * @details This class is responsible for forwarding requests to the
 * MusicBackends as well as accessing data of an DataStore and responding to
 * requests from the NetworkAPI appropriatly.\n\n Additionally the JukeBox
 * contains a (simple) scheduler algorithm to proceed after a track is over.
 */
class JukeBox : public NetworkListener {
 public:
  JukeBox();
  ~JukeBox();

  /**
   * @brief Initializes ConfigHandler, LoggingHandler as well as the
   * MusicBackend.
   * @details After initializing all handlers and interfaces it starts to handle
   * incoming requests using the NetworkAPI.
   * @param exeName Name of the currently running executable.
   * @param configFilePath Path to *.ini config file, relative to executable.
   * @return `true` on success, `false` otherwise.
   */
  bool start(std::string const &exeName, std::string const &configFilePath);

 public:  // needs to be public for tests only!
  /**
   * Implementations of the network endpoints defined in NetworkListener
   */
  TResult<TSessionID> generateSession(
      std::optional<TPassword> const &pw,
      std::optional<std::string> const &nickname) override;
  TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &searchPattern, size_t const nrOfEntries) override;
  TResult<QueueStatus> getCurrentQueues(TSessionID const &sid);
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
};

#endif /* _JUKEBOX_H_ */
