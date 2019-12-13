/*****************************************************************************/
/**
 * @file    TrackScheduler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class TrackScheduler implementation
 */
/*****************************************************************************/

#include "SimpleScheduler.h"

#include "Types/GlobalTypes.h"

using namespace std;

SimpleScheduler::SimpleScheduler(DataStore* const datastore,
                               MusicBackend* const musicbackend) {
  if (datastore == nullptr)
    LOG(ERROR) << "TrackScheduler Ctor: datastore is nullptr!";
  if (musicbackend == nullptr)
    LOG(ERROR) << "TrackScheduler Ctor: datastore is nullptr!";

  mDataStore = datastore;
  mMusicBackend = musicbackend;
}

SimpleScheduler::~SimpleScheduler() {
  if (mThread.joinable())
    mThread.join();

  /* Memory needs to be freed by the creator of this object.
   * Just de-initializing the pointers here for safety reasons.
   */
  mDataStore = nullptr;
  mMusicBackend = nullptr;
}

void SimpleScheduler::start() {
  mThread = thread(&SimpleScheduler::threadFunc, this);
}

void SimpleScheduler::threadFunc() {
  unsigned counter = 0;
  while (counter < 10) {
    LOG(INFO) << "Hello World Scheduler [" << counter++ << "]";
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  while (1) {
    /* TODO: if (scheduler.isEnabled()) over this whole thing*/
    bool ret = doSchedule();
    /* TODO: handle return values (see below) */
    if (ret) {
      LOG(INFO) << "TrackScheduler::threadFunc: Restarted scheduler.";
    }
    if (!ret) {
      LOG(ERROR)
              << "TrackScheduler::threadFunc: doSchedule returned with an error";
      break;
    }
  }
}

/* TODO:
 * return an int (0..success, 1..restart, -1..error) and handle in threadFunc()
 */
bool SimpleScheduler::doSchedule() {
  if (mDataStore == nullptr || mMusicBackend == nullptr) {
    LOG(ERROR) << "TaskScheduler.doSchedule: nullptr";
    return false;
  }
  static PlaybackTrack lastPlaybackTrack;

  this_thread::sleep_for(chrono::milliseconds(1000));

  auto playbackTrackRet = mMusicBackend->getCurrentPlayback();
  if(auto error = std::get_if<Error>(&playbackTrackRet)){
    LOG(ERROR)<< "SimpleScheduler: "<<error->getErrorMessage();
    return false; // do nothing
  }
  auto playbackTrackOpt = std::get<std::optional<PlaybackTrack>>(playbackTrackRet);

  switch(mSchedulerState){
    case Idle:{
      mDataStore->

    }break;

    case PlayNextSong:{

    }break;

    case Playing:{

    }break;
  }


  if(playbackTrackOpt.has_value()){
    lastPlaybackTrack = playbackTrackOpt.value();
  }
  else{
    lastPlaybackTrack = PlaybackTrack();
  }

  return true;
}