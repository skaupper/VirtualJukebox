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

struct Queue {
  std::vector<QueuedTrack> tracks;
};

struct QueueStatus {
  Queue normalQueue;
  Queue adminQueue;

  std::optional<PlaybackTrack> currentTrack;
};

#endif /* _QUEUE_H_ */
