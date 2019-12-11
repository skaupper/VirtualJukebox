/*****************************************************************************/
/**
 * @file    TrackScheduler.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class TrackScheduler implementation
 */
/*****************************************************************************/

#include "TrackScheduler.h"

#include "Types/GlobalTypes.h"

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
  mThread.join();

  /* Memory needs to be freed by the creator of this object.
   * Just de-initializing the pointers here for safety reasons.
   */
  mDataStore = nullptr;
  mMusicBackend = nullptr;
}

bool TrackScheduler::start() {
  mThread = thread(&TrackScheduler::doSchedule, this);
  return true;
}

bool TrackScheduler::doSchedule() {
  static unsigned counter = 0;
  while (counter < 10) {
    LOG(INFO) << "Hello World Scheduler [" << counter++ << "]";
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  auto ret = checkCurrentTrackConsistency();
  if (checkAlternativeError(ret))
    return false;

  return true;
}

TResult<bool> TrackScheduler::checkCurrentTrackConsistency() {
  /* Get current track from DataStore */
  auto retStore = mDataStore->getPlayingTrack();
  if (holds_alternative<Error>(retStore))
    return get<Error>(retStore);
  QueuedTrack trkStore = get<QueuedTrack>(retStore);

  /* Get current track from Spotify */
  auto retSpotify = mMusicBackend->getCurrentPlayback();
  if (holds_alternative<Error>(retSpotify))
    return get<Error>(retSpotify);
  PlaybackTrack trkSptfy = get<PlaybackTrack>(retSpotify);

  /* Check consistency */
  if (!(trkStore == trkSptfy)) {
    string msg =
        "TrackScheduler.checkCurrentTrackConsistency: Inconsistency between "
        "current playback track in Spotify and DataStore";
    LOG(WARNING) << msg;
    return Error(ErrorCode::InvalidValue, msg);
  }

  /* Check playback progress and sleep.
   * Wakeup with a margin to the end of the track, so be able to set
   * the next track in time.
   */
  if (trkSptfy.progressMs < trkSptfy.duration) {
    const unsigned setNewTrackMarginMs = 1000;
    unsigned sleepMs =
        trkSptfy.duration - trkSptfy.progressMs - setNewTrackMarginMs;
    this_thread::sleep_for(chrono::milliseconds(sleepMs));
  }

  /* Set next track */
  auto ret = mDataStore->nextTrack();
  if (ret.has_value())
    return ret.value();

  auto nextTrk = mDataStore->getPlayingTrack();
  if (holds_alternative<Error>(nextTrk))
    return get<Error>(nextTrk);

  ret = mMusicBackend->setPlayback(get<QueuedTrack>(nextTrk));
  if (ret.has_value())
    return ret.value();

  return true;
}
