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

TrackScheduler::~TrackScheduler() {
  // TODO: stop thread

  /* Memory needs to be freed by the creator of this object.
   * De-initializing the pointers for safety reasons.
   * TODO: Discuss if that makes sense :)
   */
  mDataStore = nullptr;
  mMusicBackend = nullptr;
}

bool TrackScheduler::start() {
  // TODO: start thread
  return true;
}
