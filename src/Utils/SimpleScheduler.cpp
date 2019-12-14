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
    auto ret = doSchedule();
    if (ret.has_value()) {
      LOG(ERROR) << "SimpleScheduler.doSchedule: "
                 << ret.value().getErrorMessage();
    }
  }
}

TResult<std::optional<PlaybackTrack>> const &
SimpleScheduler::getLastPlayback() {
  return mLastPlaybackTrack;
}

bool SimpleScheduler::checkForInconsistency() {
  auto emptyRet = areQueuesEmpty();
  if (auto error = std::get_if<Error>(&emptyRet)) {
    LOG(ERROR) << "SimpleScheduler: " << error->getErrorMessage();
    // if error occurs, check for incosistence, so client can notify it
    return true;
  }
  bool empty = std::get<bool>(emptyRet);

  if (empty && mSchedulerState == Idle) {
    return false;
  }
  return true;
}

/* TODO:
 * return an int (0..success, 1..restart, -1..error) and handle in threadFunc()
 */
TResultOpt SimpleScheduler::doSchedule() {
  if (mDataStore == nullptr || mMusicBackend == nullptr) {
    return Error(ErrorCode::InvalidValue,
                 "SimpleScheduler.doSchedule: nullpointer Fatal Error");
  }

  this_thread::sleep_for(chrono::milliseconds(cScheduleIntervalTimeMs));

  TResult<std::optional<PlaybackTrack>> playbackTrackRet;
  //  if(mSchedulerState!=Idle) {
  // only poll when not in idle state
  playbackTrackRet = mMusicBackend->getCurrentPlayback();
  if (auto error = std::get_if<Error>(&playbackTrackRet)) {
    mLastPlaybackTrack = playbackTrackRet;
    return *error;
  }
  //  }
  auto playbackTrackOpt =
      std::get<std::optional<PlaybackTrack>>(playbackTrackRet);

  switch (mSchedulerState) {
    case Idle: {
      VLOG(100) << "SimpleScheduler: Idle";
      auto emptyValRet = areQueuesEmpty();
      if (auto error = std::get_if<Error>(&emptyValRet)) {
        return *error;
      }
      bool emptyVal = std::get<bool>(emptyValRet);
      if (!emptyVal) {
        mSchedulerState = PlayNextSong;
      }

    } break;

    case PlayNextSong: {
      VLOG(1000) << "SimpleScheduler: PlayNextSong";
      auto nextTrack = mDataStore->nextTrack();
      if (nextTrack.has_value()) {
        LOG(ERROR) << "SimpleScheduler: "
                   << nextTrack.value().getErrorMessage();
        mSchedulerState = Idle;  // return to idle state on failure;
        return nextTrack.value();
      }

      auto actualTrackRet = mDataStore->getPlayingTrack();
      if (auto error = std::get_if<Error>(&actualTrackRet)) {
        LOG(ERROR) << "SimpleScheduler: " << error->getErrorMessage();
        mSchedulerState = Idle;
        return *error;  // do nothing
      }
      auto actualTrack = std::get<QueuedTrack>(actualTrackRet);

      auto setTrackRet = mMusicBackend->setPlayback(actualTrack);
      if (setTrackRet.has_value()) {
        LOG(ERROR) << "SimpleScheduler: "
                   << nextTrack.value().getErrorMessage();
        mSchedulerState = Idle;
        return setTrackRet.value();  // do nothing
      }

      mSchedulerState = CheckPlaying;

    } break;

    case CheckPlaying: {
      VLOG(100) << "SimpleScheduler: CheckPlaying";

      auto isPlayingRet = isTrackPlaying(playbackTrackOpt);
      if (auto error = std::get_if<Error>(&isPlayingRet)) {
        mSchedulerState = Idle;
        return *error;
      }
      bool isPlaying = std::get<bool>(isPlayingRet);

      if (isPlaying) {
        mSchedulerState = Playing;
      }
    } break;

    case Playing: {
      VLOG(100) << "SimpleScheduler: Playing";
      if (!playbackTrackOpt.has_value()) {
        mSchedulerState = Idle;
        return Error(ErrorCode::InvalidValue,
                     "Someone closed Spotify Player on Playback Error");
      }
      auto trackFinishedRet = isTrackFinished(playbackTrackOpt);
      if (auto error = std::get_if<Error>(&trackFinishedRet)) {
        mSchedulerState = Idle;
        return *error;
      }
      bool trackFinished = std::get<bool>(trackFinishedRet);

      if (trackFinished) {
        LOG(INFO) << "SimpleScheduler: Track finished";
        auto emptyValRet = areQueuesEmpty();
        if (auto error = std::get_if<Error>(&emptyValRet)) {
          return *error;
        }
        bool emptyVal = std::get<bool>(emptyValRet);
        if (!emptyVal) {
          mSchedulerState = PlayNextSong;
        } else {
          mSchedulerState = Idle;
        }
      }
    } break;
  }
  mLastPlaybackTrack = playbackTrackRet;

  return nullopt;
}

TResult<bool> SimpleScheduler::areQueuesEmpty() {
  auto adminQueRet = mDataStore->getQueue(QueueType::Admin);
  if (auto error = std::get_if<Error>(&adminQueRet)) {
    return *error;
  }
  auto adminQueue = std::get<Queue>(adminQueRet);
  if (!adminQueue.tracks.empty()) {
    return false;
  }

  auto normalQueueRet = mDataStore->getQueue(QueueType::Normal);
  if (auto error = std::get_if<Error>(&normalQueueRet)) {
    return *error;
  }
  auto normalQueue = std::get<Queue>(normalQueueRet);
  if (!normalQueue.tracks.empty()) {
    return false;
  }

  return true;
}

TResult<bool> SimpleScheduler::isTrackPlaying(
    std::optional<PlaybackTrack> const &currentOpt) {
  auto playingTrackRet = mDataStore->getPlayingTrack();
  if (auto error = std::get_if<Error>(&playingTrackRet)) {
    return *error;  // do nothing
  }

  if (!currentOpt.has_value()) {
    return false;
  }

  auto playingTrack = std::get<QueuedTrack>(playingTrackRet);

  if (!currentOpt.value().isPlaying) {
    mMusicBackend->play();
    return false;
  }

  // check if playing song is same as requested playing song
  if (currentOpt.value().trackId != playingTrack.trackId) {
    return Error(
        ErrorCode::InvalidValue,
        "Requested playing track not the same as actual playing track");
  }

  if (currentOpt.value().isPlaying && (currentOpt.value().progressMs > 0)) {
    return true;
  }

  return false;
}

TResult<bool> SimpleScheduler::isTrackFinished(
    std::optional<PlaybackTrack> const &currentOpt) {
  if (!currentOpt.has_value()) {
    return ErrorCode::InvalidValue,
           "Someone closed Spotify Player on playing Error";
  }

  auto playingTrackRet = mDataStore->getPlayingTrack();
  if (auto error = std::get_if<Error>(&playingTrackRet)) {
    return *error;
  }
  auto playingTrack = std::get<QueuedTrack>(playingTrackRet);

  if (currentOpt.value().progressMs == 0 &&
      currentOpt.value().isPlaying == false) {
    return true;
  }

  return false;
}