/*****************************************************************************/
/**
 * @file    SpotifyBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class handles Music Playback with a Spotify Backend
 */
/*****************************************************************************/

#include "SpotifyBackend.h"

#include <vector>

#include "Track.h"

using namespace std;

TResultOpt SpotifyBackend::initBackend() {
  return nullopt;
}

TResult<vector<Track>> SpotifyBackend::queryTracks(string const &pattern,
                                                   size_t const num) {
  auto tmp = vector<Track>();
  tmp.emplace_back(Track());
  return tmp;
}

TResultOpt SpotifyBackend::setPlayback(Track const &track) {
  return nullopt;
}

TResult<PlaybackTrack> SpotifyBackend::getCurrentPlayback(void) {
  return PlaybackTrack(Track());
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
