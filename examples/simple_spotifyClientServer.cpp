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
      "BQCM5WSJQ9fukHgk-_7cEQTtFrUOGmTJLZw5gmAfYrSjBMq_"
      "GodWRIXIMv0UEfFhFPG61Ma9wHILgXev8N1oljF4YykOfI1aMvz1kUZ6INk6zO3hkPNWp0pA"
      "xv7AeDu7e5b-"
      "KQxBUHodjKbNgueaKU1pN885KBP4di9Vlw7KLwpIJo34JvVGqnPsQ2hMhR8T6Q");
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

  return 0;
}
