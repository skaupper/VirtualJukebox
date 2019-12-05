/**
 * @file    simple_spotifyClientServer.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Minimal example using the spotify api
 */

#include <iostream>

#include "Spotify/SpotifyAPI.h"
#include "Spotify/SpotifyAPITypes.h"
#include "Spotify/SpotifyAuthorization.h"
#include "Utils/ConfigHandler.h"
#include "Utils/LoggingHandler.h"

using namespace std;

int main(void) {
  auto config = ConfigHandler::getInstance();
  config->setConfigFilePath("../../jukebox_config.ini");
  initLoggingHandler("app");

  auto test = config->getValueString("Spotify", "scopes");

  // SpotifyApi::SpotifyAuthorization auth;
  // auto ret = auth.startServer();
  //  if(ret){
  //   cout << ret.value().getErrorMessage()<<endl;
  // }
  // else{
  //    while(1);
  // }

  std::string accessToken(
      "BQAFMvFgnSfjXzOrQW9FSDa2kP3z_"
      "E6n8JiX4bh4WNlkcEX788dMx5Ieh4jKFCBwaS6wq66g2gjmOPsOgfdwR9VYtmLGFtplXntOO"
      "WYo8trzHTViLqvTAudo3G1FGHIiP60U46F3WbURNJbJA2RbH2R7jCeBpzvXUDob0lIcs_"
      "UdRvM6nQu1dRC3kPk39dEJZw");
  SpotifyApi::SpotifyAPI api;
  api.getAvailableDevices(accessToken);

  TResult<SpotifyApi::Playback> ret;
  ret = api.getCurrentPlayback(accessToken);
  if (auto val = std::get_if<Error>(&ret)) {
    cout << "errorMessage: " << val->getErrorMessage() << std::endl;
  } else {
    auto playb = get<SpotifyApi::Playback>(ret);
    std::cout << "playing:" << playb.isPlaying() << endl
              << "progress_ms" << playb.getProgressMs() << endl
              << "Device: " << playb.getDevice().getName() << endl
              << "Track Name:" << playb.getCurrentPlayingTrack().getName()
              << endl
              << "Artist Names"
              << playb.getCurrentPlayingTrack().getArtists()[0].getName()
              << endl;
  }

  return 0;
}
