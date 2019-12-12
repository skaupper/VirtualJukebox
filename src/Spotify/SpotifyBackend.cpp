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

#define SPOTIFYCALL_WITH_REFRESH(returnValue, functionCall, tokenString) \
  returnValue = functionCall;                                            \
  if (auto error = std::get_if<Error>(&returnValue)) {                   \
    auto ret = errorHandler(*error);                                     \
    if (ret.has_value()) {                                               \
      LOG(ERROR) << error->getErrorMessage() << std::endl;               \
      return *error;                                                     \
    } else {                                                             \
      tokenString = mSpotifyAuth.getAccessToken();                       \
      returnValue = functionCall;                                        \
                                                                         \
      if (auto err = std::get_if<Error>(&returnValue)) {                 \
        LOG(ERROR) << error->getErrorMessage() << std::endl;             \
                                                                         \
        return *err;                                                     \
      }                                                                  \
    }                                                                    \
  }

#define SPOTIFYCALL_WITH_REFRESH_OPT(returnValue, functionCall, tokenString) \
  returnValue = functionCall;                                                \
  if (returnValue.has_value()) {                                             \
    auto ret = errorHandler(returnValue.value());                            \
    if (ret.has_value()) {                                                   \
      LOG(ERROR) << ret.value().getErrorMessage() << std::endl;              \
      return ret.value();                                                    \
    } else {                                                                 \
      tokenString = mSpotifyAuth.getAccessToken();                           \
      returnValue = functionCall;                                            \
                                                                             \
      if (returnValue.has_value()) {                                         \
        LOG(ERROR) << returnValue.value().getErrorMessage() << std::endl;    \
                                                                             \
        return returnValue.value();                                          \
      }                                                                      \
    }                                                                        \
  }

TResultOpt SpotifyBackend::initBackend() {
  // start server for authentication
  auto startServerRet = mSpotifyAuth.startServer();

  return startServerRet;
}

TResult<std::vector<BaseTrack>> SpotifyBackend::queryTracks(
    std::string const &pattern, size_t const num) {
  std::string token = mSpotifyAuth.getAccessToken();

  TResult<SpotifyPaging> retVal;
  SPOTIFYCALL_WITH_REFRESH(
      retVal, mSpotifyAPI.search(token, pattern, QueryType::track, num), token);

  auto page = std::get<SpotifyPaging>(retVal);
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
      track.iconUri = elem.getAlbum()
                          .getImages()[0]
                          .getUrl();  // on first place is the biggest one
    }

    for (auto &artist : elem.getArtists()) {
      track.artist += artist.getName() + " & ";
    }

    track.artist.erase(track.artist.find_last_of(" & "), 3);

    tracks.emplace_back(track);
  }

  return tracks;
}

TResultOpt SpotifyBackend::setPlayback(BaseTrack const &track) {
  std::string token = mSpotifyAuth.getAccessToken();

  // check if playing devices are available
  TResult<std::vector<Device>> devicesRet;
  SPOTIFYCALL_WITH_REFRESH(
      devicesRet, mSpotifyAPI.getAvailableDevices(token), token);

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

  TResultOpt playRes;
  SPOTIFYCALL_WITH_REFRESH_OPT(
      playRes,
      mSpotifyAPI.play(token, std::vector<std::string>{track.trackId}, device),
      token);

  return std::nullopt;
}

TResult<std::optional<PlaybackTrack>> SpotifyBackend::getCurrentPlayback(void) {
  std::string token = mSpotifyAuth.getAccessToken();

  TResult<Playback> playbackRes;
  SPOTIFYCALL_WITH_REFRESH(
      playbackRes, mSpotifyAPI.getCurrentPlayback(token), token);

  auto playback = std::get<Playback>(playbackRes);
  if (!playback.getCurrentPlayingTrack().has_value()) {
    return std::nullopt;
  }
  auto const &spotifyPlayingTrack = playback.getCurrentPlayingTrack().value();

  PlaybackTrack playbackTrack;
  playbackTrack.trackId = spotifyPlayingTrack.getUri();
  playbackTrack.artist = "";
  playbackTrack.iconUri = "";
  playbackTrack.duration = spotifyPlayingTrack.getDuration();
  playbackTrack.album = spotifyPlayingTrack.getAlbum().getName();
  playbackTrack.isPlaying = playback.isPlaying();
  playbackTrack.title = spotifyPlayingTrack.getName();
  playbackTrack.progressMs = playback.getProgressMs();

  if (!spotifyPlayingTrack.getAlbum().getImages().empty()) {
    playbackTrack.iconUri = spotifyPlayingTrack.getAlbum()
                                .getImages()[0]
                                .getUrl();  // on first place is the biggest one
  }

  for (auto &artist : spotifyPlayingTrack.getArtists()) {
    playbackTrack.artist += artist.getName() + " & ";
  }
  std::string test;
  playbackTrack.artist.erase(playbackTrack.artist.find_last_of(" & "), 3);

  return playbackTrack;
}

TResultOpt SpotifyBackend::pause() {
  std::string token = mSpotifyAuth.getAccessToken();
  TResultOpt pauseRes;
  SPOTIFYCALL_WITH_REFRESH_OPT(pauseRes, mSpotifyAPI.pause(token), token);

  return std::nullopt;
}

TResultOpt SpotifyBackend::play() {
  std::string token = mSpotifyAuth.getAccessToken();
  TResultOpt playRes;
  SPOTIFYCALL_WITH_REFRESH_OPT(playRes, mSpotifyAPI.play(token), token);

  return std::nullopt;
}

TResult<size_t> SpotifyBackend::getVolume() {
  std::string token = mSpotifyAuth.getAccessToken();

  TResult<Playback> playbackRes;
  SPOTIFYCALL_WITH_REFRESH(
      playbackRes, mSpotifyAPI.getCurrentPlayback(token), token);

  auto playback = std::get<Playback>(playbackRes);
  return playback.getDevice().getVolume();
}

TResultOpt SpotifyBackend::setVolume(size_t const percent) {
  std::string token = mSpotifyAuth.getAccessToken();

  // check if playing devices are available
  TResult<std::vector<Device>> devicesRet;
  SPOTIFYCALL_WITH_REFRESH(
      devicesRet, mSpotifyAPI.getAvailableDevices(token), token);

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

  TResultOpt volRes;
  SPOTIFYCALL_WITH_REFRESH_OPT(
      volRes, mSpotifyAPI.setVolume(token, percent, device), token);

  return std::nullopt;
}

TResult<BaseTrack> SpotifyBackend::createBaseTrack(TTrackID const &trackID) {
  std::string token = mSpotifyAuth.getAccessToken();

  // remove spotify uri header (spotify:track: )
  auto pos = trackID.rfind(":");
  std::string trackNameId;
  if (pos != std::string::npos) {
    trackNameId = trackID.substr(pos + 1);
  }

  TResult<Track> trackRes;
  SPOTIFYCALL_WITH_REFRESH(
      trackRes, mSpotifyAPI.getTrack(token, trackNameId), token);

  auto track = std::get<Track>(trackRes);
  BaseTrack baseTrack;
  baseTrack.artist = "";
  baseTrack.iconUri = "";

  baseTrack.title = track.getName();
  baseTrack.album = track.getAlbum().getName();
  baseTrack.duration = track.getDuration();
  baseTrack.trackId = track.getUri();

  if (!track.getAlbum().getImages().empty()) {
    baseTrack.iconUri = track.getAlbum()
                            .getImages()[0]
                            .getUrl();  // on first place is the biggest one
  }

  for (auto &artist : track.getArtists()) {
    baseTrack.artist += artist.getName() + " & ";
  }
  return baseTrack;
}

TResultOpt SpotifyBackend::errorHandler(Error const &error) {
  if (error.getErrorCode() == ErrorCode::SpotifyAccessExpired) {
    // refresh access token if expired
    auto ret = mSpotifyAuth.refreshAccessToken();
    if (ret.has_value()) {
      return ret.value();
    } else {
      return std::nullopt;  // if went well, return nothing
    }
  }

  // return error
  return error;
}
