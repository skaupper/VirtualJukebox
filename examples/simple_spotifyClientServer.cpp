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
using namespace SpotifyApi;

int main(void) {
  auto config = ConfigHandler::getInstance();
  config->setConfigFilePath("../../jukebox_config.ini");
  initLoggingHandler("app");

  auto test = config->getValueString("Spotify", "scopes");

  // SpotifyApi::SpotifyAuthorization auth;
  // auth.startServer();

  std::string accessToken(
      "BQDhrADt4mVznUhLMYMUeUjVKUQtgnsBJPic2AtzQq4IXjCmxXbcCT9XF7xckBwglyY-"
      "zGRz0uw6018WwSFlYp0FM41XWKJT3CaK1U79DnS8en6nauwrh4OQQWzcWXanCxh7H0m-"
      "Azcs3ITV3gW71r3hFSUX7G4oiThR2lcFPJ8lsrsSEclaqcWgrtijQdUHBA");

  // remove refresh token before commit!!
  std::string refreshToken("");

  SpotifyApi::SpotifyAPI api;
  auto resClientID = config->getValueString("Spotify", "clientID");
  auto resClientSecret = config->getValueString("Spotify", "clientSecret");
  auto resRefresh = api.refreshAccessToken(
      refreshToken, get<string>(resClientID), get<string>(resClientSecret));
  accessToken = get<Token>(resRefresh).getAccessToken();
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
  std::string testContextUri;
  auto searRet =
      api.search(accessToken, "*park*", SpotifyApi::QueryType::track, 2);
  if (auto val = std::get_if<Error>(&searRet)) {
    cout << "errorMessage: " << val->getErrorMessage() << std::endl;
  } else {
    auto paging = get<SpotifyApi::SpotifyPaging>(searRet);

    cout << "QuerySearch" << paging.getTracks()[0].getName() << endl;
    testContextUri = paging.getTracks()[0].getUri();
    std::cout << "uri: " << testContextUri << endl;
  }

  api.setVolume(accessToken, 100);
  //  api.pause(accessToken);
  vector<string> x{testContextUri};
  api.play(accessToken, x);

  return 0;
}
