/*****************************************************************************/
/**
 * @file    SpotifyBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Class handles Music Playback with a Spotify Backend
 */
/*****************************************************************************/

#include "SpotifyBackend.h"

#include <vector>

#include "Utils/ConfigHandler.h"
#include "Utils/LoggingHandler.h"

using namespace SpotifyApi;

SpotifyBackend::~SpotifyBackend() {
  mSpotifyAuth.stopServer();
}

TResultOpt SpotifyBackend::initBackend() {
  // start server for authentication
  auto startServerRet = mSpotifyAuth.startServer();

  return startServerRet;
}

TResult<std::vector<BaseTrack>> SpotifyBackend::queryTracks(
    std::string const &pattern, size_t const num) {
  std::string token = mSpotifyAuth.getAccessToken();

  auto searchRes = mSpotifyAPI.search(token, pattern, QueryType::track, num);
  if (auto res = std::get_if<Error>(&searchRes)) {
    LOG(ERROR) << res->getMessage() << std::endl;
    return res;
  }
  auto page = std::get<SpotifyPage>(searchRes);
  std::vector<BaseTrack> tracks;

  for (auto const &elem : page.getTracks()) {
    BaseTrack track;
    track.artist = "";
    track.iconUri = "";

    track.title = elem.getName();
    track.album = elem.getAlbum().getName();
    track.duration = elem.getDuration();
    track.trackId = elem.getUri();

    if (!elem.getAlbum().getImages().empty()) {
      track.iconUri =
          elem.getAlbum().getImages()[0];  // on first place is the biggest one
    }

    for (auto &artist : elem.getArtists()) {
      track.artist += artist.getName() + " & ";
    }
    std::string test;
    track.artist.erase(track.artist.find_last_of(" & "), 3);

    tracks.emplace_back(track);
  }

  return std::move(tracks);
}

TResultOpt SpotifyBackend::setPlayback(BaseTrack const &track) {
  std::string token = mSpotifyAuth.getAccessToken();

  // check if playing devices are available
  auto devicesRet = mSpotifyAPI.getAvailableDevices(token);
  if (auto value = std::get_if<Error>(&devicesRet)) {
    return *value;
  }
  auto devices = std::get<std::vector<Device>>(devicesRet);
  if (devices.empty()) {
    return Error(ErrorCode::KeyNotFound,
                 "No devices for playing the track available");
  }

  // check if a device has the same name as the one stored in the config (if yes
  // use it, else the activated device gets used)
  auto config = ConfigHandler::getInstance();
  auto playingDeviceRes = config->getValueString("Spotify", "playingDevice");

  Device device;
  if (auto value = std::get_if<std::string>(&playingDeviceRes)) {
    auto dev =
        std::find_if(devices.cbegin(), devices.cend(), [&](auto const &elem) {
          return (elem.getName() == *value);
        });
    if (dev != devices.cend()) {
      device = *dev;
    }
  }

  auto playRes =
      mSpotifyAPI.play(token, std::vector<std::string>{track.trackId}, device);
  if (auto value = std::get_if<Error>(&playRes)) {
    LOG(ERROR) << value->getErrorMessage() << std::endl;
    return *value;
  }

  return std::nullopt;
}

TResult<PlaybackTrack> SpotifyBackend::getCurrentPlayback(void) {
  std::string token = mSpotifyAuth.getAccessToken();

  auto playbackRes = mSpotifyAPI.getCurrentPlayback(token);
  if (auto value = std::get_if<Error>(&playbackRes)) {
    LOG(ERROR) << value->getErrorMessage() << std::endl;
    return *value;
  }
  auto playback = std::get<Playback>(playbackRes);

  PlaybackTrack playbackTrack;
  playbackTrack.trackId = playback.getCurrentPlayingTrack().getUri();
  playbackTrack.artist = "";
  playbackTrack.iconUri = "";
  playbackTrack.duration = playback.getCurrentPlayingTrack().getDuration();
  playbackTrack.album = playback.getCurrentPlayingTrack().getAlbum().getName();
  playbackTrack.isPlaying = playback.isPlaying();
  playbackTrack.title = playback.getCurrentPlayingTrack().getName();
  playbackTrack.progressMs = playback.getProgressMs();

  if (!playback.getCurrentPlayingTrack().getAlbum().getImages().empty()) {
    playbackTrack.iconUri =
        playback.getCurrentPlayingTrack()
            .getAlbum()
            .getImages()[0];  // on first place is the biggest one
  }

  for (auto &artist : playback.getCurrentPlayingTrack().getArtists()) {
    playbackTrack.artist += artist.getName() + " & ";
  }
  std::string test;
  playbackTrack.artist.erase(playbackTrack.artist.find_last_of(" & "), 3);

  return playbackTrack;
}

TResultOpt SpotifyBackend::pause() {
  std::string token = mSpotifyAuth.getAccessToken();
  auto ret = mSpotifyAPI.pause(token);

  if (auto value = std::get_if<Error>(&ret)) {
    LOG(ERROR) << value->getErrorMessage() << std::endl;
    return *value;
  }

  return std::nullopt;
}

TResultOpt SpotifyBackend::play() {
  std::string token = mSpotifyAuth.getAccessToken();
  auto ret = mSpotifyAPI.play(token);

  if (auto value = std::get_if<Error>(&ret)) {
    LOG(ERROR) << value->getErrorMessage() << std::endl;
    return *value;
  }

  return std::nullopt;
}

TResult<size_t> SpotifyBackend::getVolume() {
  std::string token = mSpotifyAuth.getAccessToken();

  auto playbackRes = mSpotifyAPI.getCurrentPlayback(token);
  if (auto value = std::get_if<Error>(&playbackRes)) {
    LOG(ERROR) << value->getErrorMessage() << std::endl;
    return *value;
  }
  auto playback = std::get<Playback>(playbackRes);

  return playback.getDevice().getVolume();
}

TResultOpt SpotifyBackend::setVolume(size_t const percent) {
  std::string token = mSpotifyAuth.getAccessToken();

  // check if playing devices are available
  auto devicesRet = mSpotifyAPI.getAvailableDevices(token);
  if (auto value = std::get_if<Error>(&devicesRet)) {
    return *value;
  }
  auto devices = std::get<std::vector<Device>>(devicesRet);
  if (devices.empty()) {
    return Error(ErrorCode::KeyNotFound,
                 "No devices for playing the track available");
  }

  // check if a device has the same name as the one stored in the config (if yes
  // use it, else the activated device gets used)
  auto config = ConfigHandler::getInstance();
  auto playingDeviceRes = config->getValueString("Spotify", "playingDevice");

  Device device;
  if (auto value = std::get_if<std::string>(&playingDeviceRes)) {
    auto dev =
        std::find_if(devices.cbegin(), devices.cend(), [&](auto const &elem) {
          return (elem.getName() == *value);
        });
    if (dev != devices.cend()) {
      device = *dev;
    }
  }

  auto ret = mSpotifyAPI.setVolume(token, percent, device);
  if (auto value = std::get_if<Error>(&ret)) {
    LOG(ERROR) << value->getErrorMessage() << std::endl;
    return *value;
  }

  return std::nullopt;
}
