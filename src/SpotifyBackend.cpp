/*****************************************************************************/
/**
 * @file    SpotifyBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class handles Music Playback with a Spotify Backend
 */
/*****************************************************************************/

#include "SpotifyBackend.h"

#include <vector>

using namespace std;

TResultOpt SpotifyBackend::initBackend() {
  return nullopt;
}

TResult<vector<BaseTrack>> SpotifyBackend::queryTracks(string const &pattern,
                                                       size_t const num) {
  auto tmp = vector<BaseTrack>();
  tmp.emplace_back();
  return tmp;
}

TResultOpt SpotifyBackend::setPlayback(BaseTrack const &track) {
  return nullopt;
}

TResult<PlaybackTrack> SpotifyBackend::getCurrentPlayback(void) {
  return PlaybackTrack();
}

TResultOpt SpotifyBackend::pause() {
  return nullopt;
}

TResultOpt SpotifyBackend::play() {
  return nullopt;
}

TResult<size_t> SpotifyBackend::getVolume() {
  return 50;
}

TResultOpt SpotifyBackend::setVolume(size_t const percent) {
  return Error(ErrorCode::SessionExpired, "Error Message Test");
}
