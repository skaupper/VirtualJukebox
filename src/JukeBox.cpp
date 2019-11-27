/** ----------------------------------------------------------------------------
 * @file    JukeBox.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox implementation
 * ---------------------------------------------------------------------------*/

#include "JukeBox.h"

#include <iostream>

#include "ConfigHandler.h"
#include "GlobalTypes.h"
#include "Result.h"

using namespace std;

void JukeBox::start(string configFilePath) {
  ConfigHandler& conf = ConfigHandler::GetInstance();
  conf.setConfigFilePath(configFilePath);
}

TResult<TSessionID> JukeBox::generateSession(TPassword pw) {
  TResult<TSessionID> ret;

  //  ret = mMemory.generateSession();

  return ret;
}

TResult<std::vector<Track>> JukeBox::queryTracks(std::string searchPattern) {
  TResult<std::vector<Track>> ret;

  return ret;
}

// TResult<QueueStatus> JukeBox::getCurrentQueues() {
//}

TResultOpt JukeBox::addTrackToQueue(TSessionID sid,
                                    TTrackID trkid,
                                    QueueType type) {
  return Error(ErrorCode::AccessDedied, "this is an example error message");
}

TResultOpt JukeBox::voteTrack(TSessionID sid, TTrackID trkid, TVote vote) {
}

TResult<Track> JukeBox::removeTrack(TSessionID sid, TTrackID trkid) {
}

TResult<Track> JukeBox::moveTrack(TSessionID sid, TTrackID trkid) {
}

TResultOpt JukeBox::controlPlayer(TSessionID sid, PlayerAction action) {
}
