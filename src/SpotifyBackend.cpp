/** ----------------------------------------------------------------------------
 * @file    SpotifyBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class handles Music Playback with a Spotify Backend
 * ---------------------------------------------------------------------------*/

#include <vector>
#include "SpotifyBackend.h"
#include "Track.h"

TResultOpt SpotifyBackend::initBackend() {
    return std::nullopt;
}

TResult<std::vector<Track>> SpotifyBackend::queryTracks(std::string const &pattern, size_t const num) {
    auto tmp = std::vector<Track>();
    tmp.emplace_back(Track());
    return tmp;
}

TResultOpt SpotifyBackend::setPlayback(Track const &track) {
    return std::nullopt;
}

TResult<PlaybackTrack> SpotifyBackend::getCurrentPlayback(void){

    return PlaybackTrack(Track());
}

TResultOpt SpotifyBackend::pause() {
    return std::nullopt;
}

TResultOpt SpotifyBackend::play() {
    return std::nullopt;
}

TResult<size_t> SpotifyBackend::getVolume() {
    return 50;
}

TResultOpt SpotifyBackend::setVolume(size_t const percent) {
    return Error(ErrorCode::SessionExpired,"Error Message Test");
}
