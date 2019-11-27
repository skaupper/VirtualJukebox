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

TResult<std::vector<std::shared_ptr<Track>>> SpotifyBackend::queryTracks(std::string const &pattern, size_t const num) {
    auto tmp = std::vector<std::shared_ptr<Track>>();
    tmp.emplace_back(std::make_shared<Track>());
    return tmp;
}

TResultOpt SpotifyBackend::setPlayback(std::shared_ptr<Track> const track) {
    return std::nullopt;
}

TResult<std::shared_ptr<PlaybackTrack>> SpotifyBackend::getCurrentPlayback(void){

    return std::make_shared<PlaybackTrack>(Track());
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
