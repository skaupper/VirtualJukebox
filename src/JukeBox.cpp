/*****************************************************************************/
/**
 * @file    JukeBox.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox implementation
 */
/*****************************************************************************/

#include "JukeBox.h"

#include <iostream>
#include <memory>

#include "ConfigHandler.h"
#include "GlobalTypes.h"
#include "Result.h"

using namespace std;

void JukeBox::start(string configFilePath) {
  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  TResultOpt ret = conf->setConfigFilePath(configFilePath);
  checkOptionalError(ret);
}

TResult<TSessionID> JukeBox::generateSession(
    optional<TPassword> const &pw, std::optional<std::string> const &nickname) {
  TResult<TSessionID> ret;

  //  ret = mMemory.generateSession();

  return ret;
}

TResult<vector<Track>> JukeBox::queryTracks(string const &searchPattern,
                                            size_t const nrOfEntries) {
  TResult<vector<Track>> ret;

  return ret;
}

// TResult<QueueStatus> JukeBox::getCurrentQueues() {
//}

TResultOpt JukeBox::addTrackToQueue(TSessionID const &sid,
                                    TTrackID const &trkid,
                                    QueueType type) {
  return Error(ErrorCode::AccessDenied, "this is an example error message");
}

TResultOpt JukeBox::voteTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              TVote vote) {
  return nullopt;
}

TResultOpt JukeBox::removeTrack(TSessionID const &sid, TTrackID const &trkid) {
  return nullopt;
}

TResultOpt JukeBox::moveTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              QueueType type) {
  return nullopt;
}

TResultOpt JukeBox::controlPlayer(TSessionID const &sid, PlayerAction action) {
  return nullopt;
}
