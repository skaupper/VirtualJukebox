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
  if (ret.has_value())
    cerr << ret.value().getErrorMessage() << endl;
}

TResult<TSessionID> JukeBox::generateSession(
    optional<TPassword> const &pw, std::optional<std::string> const &nickname) {
  return Error(ErrorCode::NotImplemented,
               "generateSession is not implemented yet");
}

TResult<vector<BaseTrack>> JukeBox::queryTracks(string const &searchPattern,
                                                size_t const nrOfEntries) {
  return Error(ErrorCode::NotImplemented, "queryTracks is not implemented yet");
}

TResult<QueueStatus> JukeBox::getCurrentQueues(TSessionID const &) {
  return Error(ErrorCode::NotImplemented,
               "getCurrentQueues is not implemented yet");
}

TResultOpt JukeBox::addTrackToQueue(TSessionID const &sid,
                                    TTrackID const &trkid,
                                    QueueType type) {
  return Error(ErrorCode::NotImplemented,
               "addTrackToQueue is not implemented yet");
}

TResultOpt JukeBox::voteTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              TVote vote) {
  return Error(ErrorCode::NotImplemented, "voteTrack is not implemented yet");
}

TResultOpt JukeBox::removeTrack(TSessionID const &sid, TTrackID const &trkid) {
  return Error(ErrorCode::NotImplemented, "removeTrack is not implemented yet");
}

TResultOpt JukeBox::moveTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              QueueType type) {
  return Error(ErrorCode::NotImplemented, "moveTrack is not implemented yet");
}

TResultOpt JukeBox::controlPlayer(TSessionID const &sid, PlayerAction action) {
  return Error(ErrorCode::NotImplemented,
               "controlPlayer is not implemented yet");
}
