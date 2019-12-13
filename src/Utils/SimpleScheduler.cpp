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

SimpleScheduler::SimpleScheduler(DataStore *const datastore,
                                 MusicBackend *const musicbackend) {
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
  if (auto error = std::get_if<Error>(&playbackTrackRet)) {
    LOG(ERROR) << "SimpleScheduler: " << error->getErrorMessage();
    return false;  // do nothing
  }
  auto playbackTrackOpt =
      std::get<std::optional<PlaybackTrack>>(playbackTrackRet);

  switch (mSchedulerState) {
    case Idle: {
      auto emptyValRet = areQueuesEmpty();
      if (auto error = std::get_if<Error>(&emptyValRet)) {
        return false;
      }
      bool emptyVal = std::get<bool>(emptyValRet);
      if (!emptyVal) {
        mSchedulerState = PlayNextSong;
      }

    } break;

    case PlayNextSong: {
      auto nextTrack = mDataStore->nextTrack();
      if (nextTrack.has_value()) {
        LOG(ERROR) << "SimpleScheduler: "
                   << nextTrack.value().getErrorMessage();
        return false;  // do nothing
      }

      auto actualTrackRet = mDataStore->getPlayingTrack();
      if (auto error = std::get_if<Error>(&actualTrackRet)) {
        LOG(ERROR) << "SimpleScheduler: " << error->getErrorMessage();
        return false;  // do nothing
      }
      auto actualTrack = std::get<QueuedTrack>(actualTrackRet);

      auto setTrackRet = mMusicBackend->setPlayback(actualTrack);
      if (setTrackRet.has_value()) {
        LOG(ERROR) << "SimpleScheduler: "
                   << nextTrack.value().getErrorMessage();
        return false;  // do nothing
      }

      mSchedulerState = CheckPlaying;

    } break;

    case CheckPlaying: {
      if (!playbackTrackOpt.has_value()) {
        return false;  // wait until track is playing
      }
      if (isTrackPlaying(mLastPlaybackTrack, playbackTrackOpt.value())) {
        mSchedulerState = Playing;
      }
    }

    case Playing: {
      if (!playbackTrackOpt.has_value()) {
        mSchedulerState = Idle;
        return false;  // wait until track is playing
      }

      if (isTrackFinished(mLastPlaybackTrack, playbackTrackOpt.value())) {
        mSchedulerState = Idle;
      }

    } break;
  }

  if (playbackTrackOpt.has_value()) {
    lastPlaybackTrack = playbackTrackOpt.value();
  } else {
    lastPlaybackTrack = PlaybackTrack();
  }

  return true;
}

TResult<bool> SimpleScheduler::areQueuesEmpty() {
  auto adminQueRet = mDataStore->getQueue(QueueType::Admin);
  if (auto error = std::get_if<Error>(&adminQueRet)) {
    return *error;
  }
  auto adminQueue = std::get<Queue>(adminQueRet);
  if (adminQueue.tracks.empty()) {
    return true;
  }

  auto normalQueueRet = mDataStore->getQueue(QueueType::Normal);
  if (auto error = std::get_if<Error>(&normalQueueRet)) {
    return *error;
  }
  auto normalQueue = std::get<Queue>(normalQueueRet);
  if (normalQueue.tracks.empty()) {
    return true;
  }

  return false;
}

bool SimpleScheduler::isTrackPlaying(PlaybackTrack const &last,
                                     PlaybackTrack const &current) {
  auto playingTrackRet = mDataStore->getPlayingTrack();
  if (auto error = std::get_if<Error>(&playingTrackRet)) {
    LOG(ERROR) << "SimpleScheduler: " << error->getErrorMessage();
    return false;  // do nothing
  }
  auto playingTrack = std::get<QueuedTrack>(playingTrackRet);

  if ((current.progressMs > 0) && (current.trackId == playingTrack.trackId)) {
    return true;
  }

  return false;
}

bool SimpleScheduler::isTrackFinished(PlaybackTrack const &last,
                                      PlaybackTrack const &current) {
  auto playingTrackRet = mDataStore->getPlayingTrack();
  if (auto error = std::get_if<Error>(&playingTrackRet)) {
    LOG(ERROR) << "SimpleScheduler: " << error->getErrorMessage();
    return false;  // do nothing
  }
  auto playingTrack = std::get<QueuedTrack>(playingTrackRet);
  if ((last.progressMs > 0) && (current.progressMs == 0)) {
    if ((last.isPlaying == true) && (current.isPlaying == false)) {
      return true;
    }
  }

  if (current.progressMs == 0 && current.isPlaying == false) {
    return true;
    // return here true because this shouldnt happen
  }

  return false;
}