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
   * @brief Initializes ConfigHandler and LoggingHandler.
   * @param exeName Name of the currently running executable.
   * @param configFilePath Path to *.ini config file, relative to executable.
   * @return True on success, false otherwise.
   */
  bool start();

 private:
  bool doSchedule();

  DataStore* mDataStore;
  MusicBackend* mMusicBackend;

  std::thread mThread;
};

#endif /* _TRACKSCHEDULER_H_ */
