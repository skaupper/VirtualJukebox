/** ----------------------------------------------------------------------------
 * @file    JukeBox.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox implementation
 * ---------------------------------------------------------------------------*/

#include "JukeBox.h"

#include <iostream>
#include <memory>

#include "ConfigHandler.h"
#include "GlobalTypes.h"
#include "Result.h"

using namespace std;

void JukeBox::start(string configFilePath) {
  shared_ptr<ConfigHandler> conf = ConfigHandler::getInstance();
  conf->setConfigFilePath(configFilePath);
}

TResult<TSessionID> JukeBox::generateSession(TPassword pw) {
  TResult<TSessionID> ret;

  //  ret = mMemory.generateSession();

  return ret;
}

TResult<vector<Track>> JukeBox::queryTracks(string searchPattern) {
  TResult<vector<Track>> ret;

  return ret;
}

// TResult<QueueStatus> JukeBox::getCurrentQueues() {
//}

TResultOpt JukeBox::addTrackToQueue(TSessionID sid,
                                    TTrackID trkid,
                                    QueueType type) {
  return Error(ErrorCode::AccessDenied, "this is an example error message");
}

TResultOpt JukeBox::voteTrack(TSessionID sid, TTrackID trkid, TVote vote) {
}

TResult<Track> JukeBox::removeTrack(TSessionID sid, TTrackID trkid) {
}

TResult<Track> JukeBox::moveTrack(TSessionID sid, TTrackID trkid) {
}

TResultOpt JukeBox::controlPlayer(TSessionID sid, PlayerAction action) {
}
