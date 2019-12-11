/*****************************************************************************/
/**
 * @file    TrackScheduler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class TrackScheduler implementation
 */
/*****************************************************************************/

#include "TrackScheduler.h"

using namespace std;

TrackScheduler::TrackScheduler(DataStore* const datastore,
                               MusicBackend* const musicbackend) {
  if (datastore == nullptr)
    LOG(ERROR) << "TrackScheduler Ctor: datastore is nullptr!";
  if (musicbackend == nullptr)
    LOG(ERROR) << "TrackScheduler Ctor: datastore is nullptr!";

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
  mThread = thread(&TrackScheduler::doSchedule, this);
  return true;
}

TResultOpt TrackScheduler::doSchedule() {
  static unsigned counter = 0;
  while (counter < 10) {
    LOG(INFO) << "Hello World Scheduler [" << counter++ << "]";
    this_thread::sleep_for(chrono::seconds(1));
  }
  return nullopt;
}
