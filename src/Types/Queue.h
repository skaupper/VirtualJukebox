/*****************************************************************************/
/**
 * @file    Queue.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class Queue definition
 */
/*****************************************************************************/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <optional>
#include <vector>

#include "Tracks.h"

/**
 * @brief Respresents a queue which is returned to the clients.
 */
struct Queue {
  std::vector<QueuedTrack> tracks;
};

/**
 * @brief Wraps all information to return if a client requests the current queue
 * status.
 */
struct QueueStatus {
  Queue normalQueue;
  Queue adminQueue;

  std::optional<PlaybackTrack> currentTrack;
};

#endif /* _QUEUE_H_ */
