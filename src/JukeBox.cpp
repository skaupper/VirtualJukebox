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

#include "Types/GlobalTypes.h"
#include "Types/Result.h"
#include "Utils/ConfigHandler.h"
#include "Utils/LoggingHandler.h"

using namespace std;

bool JukeBox::start(string exeName, string configFilePath) {
  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  TResultOpt ret = conf->setConfigFilePath(configFilePath);
  if (ret.has_value()) {
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: " << ret.value().getErrorMessage() << endl;

    return false;
  }

  initLoggingHandler(exeName);
  DLOG(INFO) << "Hello world from JukeBox main.cpp !";
  return true;
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
