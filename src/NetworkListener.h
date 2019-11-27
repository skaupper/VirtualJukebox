/** ----------------------------------------------------------------------------
 * @file    NetworkListener.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Interface Class NetworkListener definition
 * ---------------------------------------------------------------------------*/

#ifndef _NETWORKLISTENER_H_
#define _NETWORKLISTENER_H_

#include <string>
#include <variant>
#include <vector>

#include "GlobalTypes.h"
#include "Queue.h"
#include "Result.h"
#include "Track.h"

//#include "LoggingHandler.h"
//#include "NetworkAPI.h"

/**
 * @brief Type for multiple queues
 */
using Queues = Queue[2];

class NetworkListener {
 public:
  virtual TResult<TSessionID> generateSession(TPassword pw) = 0;
  virtual TResult<std::vector<Track>> queryMusic(std::string searchPattern) = 0;
  virtual TResult<Queues> getCurrentQueues() = 0;
  virtual TResultOpt addTrackToQueue(TSessionID sid,
                                     TTrackID trkid,
                                     QueueType type) = 0;
  virtual TResultOpt voteTrack(TSessionID sid, TTrackID trkid) = 0;
  virtual TResultOpt controlPlayer(TSessionID sid, PlayerAction action) = 0;
};

#endif /* _NETWORKLISTENER_H_ */
