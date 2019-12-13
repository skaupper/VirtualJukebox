/*****************************************************************************/
/**
 * @file    SimpleScheduler.h
 * @authors Michael Wurm <wurm.michael95@gmail.com>
 * Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SimpleScheduler definition
 */
/*****************************************************************************/

#ifndef SIMPLE_SCHEDULER_H_INCLUDED
#define SIMPLE_SCHEDULER_H_INCLUDED

#include <memory>
#include <string>
#include <thread>

#include "Datastore/DataStore.h"
#include "MusicBackend.h"
#include "Types/Result.h"

class SimpleScheduler {
 public:
  SimpleScheduler(DataStore* const datastore, MusicBackend* const musicbackend);
  ~SimpleScheduler();

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

  enum SchedulerState { Idle, PlayNextSong, CheckPlaying, Playing };
  bool doSchedule();

  void threadFunc();

  DataStore* mDataStore;
  MusicBackend* mMusicBackend;
  SchedulerState mSchedulerState = Idle;
  PlaybackTrack mLastPlaybackTrack;

  TResult<bool> areQueuesEmpty(void);
  bool isTrackPlaying(PlaybackTrack const& last, PlaybackTrack const& current);
  bool isTrackFinished(PlaybackTrack const& last, PlaybackTrack const& current);

  std::thread mThread;
};

#endif /* SIMPLE_SCHEDULER_H_INCLUDED */