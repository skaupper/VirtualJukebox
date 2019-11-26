/** ----------------------------------------------------------------------------
 * @file    JukeBox.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox implementation
 * ---------------------------------------------------------------------------*/

#include "JukeBox.h"

#include "GlobalTypes.h"
#include "Result.h"

using namespace std;

void JukeBox::start(string configFilePath) {
}

TResult<TSessionID> generateSession(TPassword pw) {
  TResult<TSessionID> ret;

  //  ret = mMemory.generateSession();

  return ret;
}

// TResult<std::vector<Track>> queryMusic(std::string searchPattern) {
//   TResult<std::vector<Track>> ret;
//
//   return ret;
// }

// TResult<Queues> getCurrentQueues() {
//  TResult<Queues> ret;
//
//  return ret;
//}

TResultOpt addTrackToQueue(TSessionID sid, TTrackID trkid, QueueType type) {
  return Error(ErrorCode::AccessDedied, "this is an example error message");
}

TResultOpt voteTrack(TSessionID sid, TTrackID trkid) {
}

TResultOpt controlPlayer(TSessionID sid, PlayerAction action) {
}
