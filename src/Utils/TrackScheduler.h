/*****************************************************************************/
/**
 * @file    TrackScheduler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class TrackScheduler definition
 */
/*****************************************************************************/

#ifndef _TRACKSCHEDULER_H_
#define _TRACKSCHEDULER_H_

#include <memory>
#include <string>
#include <thread>

#include "Datastore/DataStore.h"
#include "MusicBackend.h"
#include "Types/Result.h"

class TrackScheduler {
 public:
  TrackScheduler(DataStore* const datastore, MusicBackend* const musicbackend);
  ~TrackScheduler();

  /**
   * @brief Starts the scheduler thread.
   */
  void start();

  /* TODO: functions below */
  /* enable() */
  /* disable() */
  /* notify() */

 private:
  /**
   * @brief Schedules one track after another.
   * @details The next track is set to play, when the currently playing track
   * reaches its' end. The thread only wakes up shortly before a tracks' end.
   * The function also performs a consistency check between the DataStore and
   * Spotify.
   * @return True on success, false otherwise.
   * Errors are printed within the function.
   */
  bool doSchedule();

  void threadFunc();

  DataStore* mDataStore;
  MusicBackend* mMusicBackend;

  std::thread mThread;
};

#endif /* _TRACKSCHEDULER_H_ */
