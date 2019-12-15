/*****************************************************************************/
/**
 * @file    SimpleScheduler.h
 * @authors Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class SimpleScheduler definition
 */
/*****************************************************************************/

#ifndef SIMPLE_SCHEDULER_H_INCLUDED
#define SIMPLE_SCHEDULER_H_INCLUDED

#include <memory>
#include <shared_mutex>
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

  /**
   * @brief returns the last polled playback status
   * @return returns playback status
   */
  TResult<std::optional<PlaybackTrack>> const& getLastPlayback();

  TResultOpt nextTrack();

  /**
   * @brief returns true if actual track from datastore and playback needs to be
   * checked if they are equal
   * @return true if check is required else false
   */
  bool checkForInconsistency();

  /* TODO: functions below */
  /* enable() */
  /* disable() */

 private:
  /**
   * @brief enumaration represents state of the scheduler
   */
  enum class SchedulerState { Idle, PlayNextSong, CheckPlaying, Playing };

  /**
   * @brief Schedules one track after another.
   * @details The next track is set to play, when the currently playing track
   * reaches its' end. This thread continuously polls the actual playback.
   * @return nullopt on success, otherwise Error object
   */
  TResultOpt doSchedule();

  /**
   * @brief threadfunction which handles the doSchedule task
   */
  void threadFunc();

  int const cScheduleIntervalTimeMs = 1000;

  DataStore* mDataStore;
  MusicBackend* mMusicBackend;
  SchedulerState mSchedulerState = SchedulerState::Idle;
  TResult<std::optional<PlaybackTrack>> mLastPlaybackTrack;

  TResult<bool> areQueuesEmpty(void);
  TResult<bool> isTrackPlaying(std::optional<PlaybackTrack> const& currentOpt);
  TResult<bool> isTrackFinished(std::optional<PlaybackTrack> const& currentOpt);

  std::thread mThread;
  std::shared_mutex mMtxPlayback;
  std::shared_mutex mMtxModifySchedulerState;
};

#endif /* SIMPLE_SCHEDULER_H_INCLUDED */