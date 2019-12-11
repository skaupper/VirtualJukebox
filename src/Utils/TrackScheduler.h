/*****************************************************************************/
/**
 * @file    TrackScheduler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class TrackScheduler definition
 */
/*****************************************************************************/

#ifndef _TRACKSCHEDULER_H_
#define _TRACKSCHEDULER_H_

#include <string>
#include <thread>
#include <variant>

#include "Datastore/DataStore.h"
#include "MusicBackend.h"
#include "Types/GlobalTypes.h"
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
  // TODO write threadMainfunc()

  DataStore* mDataStore;
  MusicBackend* mMusicBackend;
  // TODO create thread object
};

#endif /* _TRACKSCHEDULER_H_ */
