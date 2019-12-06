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
      "BQDecD1tigDxy5OFUpR28hImVaXOFhG1LDuLaw9sPKs8kKFGcNfAxCciICGiIHkK5R5gaSdAg08PmIK-Be7cjCFOJcwAPrKhQwcppGobQB_FwbwKcUySaXpT0-EdIEUpZ6kPaCkhs128RCJHVtU5Okucpe8UkJa3xEhA08IWp5AStvPquXARb8VhaxucHbKHAw");
  SpotifyApi::SpotifyAPI api;
  api.getAvailableDevices(accessToken);

  TResult<SpotifyApi::Playback> ret;
  ret = api.getCurrentPlayback(accessToken);
  if (auto val = std::get_if<Error>(&ret)) {
    cout << "errorMessage: " << val->getErrorMessage() << std::endl;
  } else {
    auto playb = get<SpotifyApi::Playback>(ret);
    std::cout
        << "playing:" << playb.isPlaying() << endl
        << "progress_ms" << playb.getProgressMs() << endl
        << "Device: " << playb.getDevice().getName() << endl
        << "Track Name:" << playb.getCurrentPlayingTrack().getName() << endl
        << "Artist Names "
        //<< playb.getCurrentPlayingTrack().getArtists()[0].getName() << endl
        << "ArtistFromAlbum: "
        //<< playb.getCurrentPlayingTrack().getAlbum().getArtists()[0].getName()
        << endl
        << "AlbumName: " << playb.getCurrentPlayingTrack().getAlbum().getName()
        << endl
        << "image url: "
        //<< playb.getCurrentPlayingTrack().getAlbum().getImages()[0].getUrl()
        << endl;
  }
  auto searRet =
      api.search(accessToken, "*park*", SpotifyApi::QueryType::track, 2);
  if (auto val = std::get_if<Error>(&searRet)) {
    cout << "errorMessage: " << val->getErrorMessage() << std::endl;
  } else {
    auto paging = get<SpotifyApi::SpotifyPaging>(searRet);

    cout << "QuerySearch" << paging.getTracks()[0].getName() << endl;
  }

  api.setVolume(accessToken,100);
  api.pause(accessToken);

  return 0;
}
