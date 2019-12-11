/*****************************************************************************/
/**
 * @file    TrackScheduler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class TrackScheduler implementation
 */
/*****************************************************************************/

#include "TrackScheduler.h"

TrackScheduler::TrackScheduler(DataStore* const datastore,
                               MusicBackend* const musicbackend) {
  mDataStore = datastore;
  mMusicBackend = musicbackend;
}

bool TrackScheduler::start() {
  return true;
}
