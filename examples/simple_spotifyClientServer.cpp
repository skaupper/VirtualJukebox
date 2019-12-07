/**
 * @file    simple_spotifyClientServer.cpp
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Minimal example using the spotify api with the spotify authorization
 * @details that this example programm works, you have to specify your client id
 * and client secret from spotfy developers dashboard into the file *.ini file,
 * in our case "jukebox_config.ini" (maybe also the filepath needs to be
 * adapted)
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
  // initialize config handler and logger
  auto config = ConfigHandler::getInstance();
  config->setConfigFilePath("../../jukebox_config.ini");
  initLoggingHandler("app");

  SpotifyApi::SpotifyAuthorization spotifyAuth;
  spotifyAuth.startServer();

  std::string accessToken = "";
  // wait as long until a accestoken was received (connect to
  // http://localhost:8889/spotifyLogin)
  cout << "Connect to : http://localhost:8889/spotifyLogin to login into your "
          "spotify account"
       << endl;
  while (accessToken.empty()) {
    accessToken = spotifyAuth.getAccessToken();
    sleep(1);
  }

  // now test some functions from the spotify api

  SpotifyApi::SpotifyAPI api;

  // so now lets try some functions

  // first get some tracks from spotify
  // so lets find some tracks from linin park (with query type you can also
  // specify if you want fo search for artists, albums, ..)
  auto searchRet = api.search(accessToken, "linkin park", QueryType::track);

  // check if all went well or not..
  if (auto err = std::get_if<Error>(&searchRet)) {
    cerr << "Error occured! Message: " << err->getErrorMessage() << endl;
    return -1;
  }
  auto search = get<SpotifyPaging>(searchRet);

  // now list all tracks with title and artist (a title can have more then one
  // artists)
  auto tracks = search.getTracks();
  cout << "----------------------------------------------------" << endl;
  for (size_t i = 0; i < tracks.size(); ++i) {
    auto const& track = tracks[i];
    cout << "[ " << i << " ] "
         << "Title: " << track.getName() << "  Artist(s): ";
    for_each(track.getArtists().cbegin(),
             track.getArtists().cend(),
             [](Artist const& elem) { cout << elem.getName() << ", "; });
    cout << "  Album: " << track.getAlbum().getName();
    cout << endl;
    cout << "----------------------------------------------------" << endl;
  }

  // hmm now we want to listen to the first track
  cout << endl << endl;
  cout << "Now select which song you want to play (insert number): " << endl;
  string line;
  int d;
  while (std::getline(std::cin, line)) {
    stringstream ss(line);
    if (ss >> d) {
      if (ss.eof()) {  // Success
        if (d >= tracks.size()) {
          cout << "number out of range" << endl;
        }
        break;
      }
    }
    cout << "No Number given!" << std::endl;
  }
  cout << "Selected track: " << tracks[d].getName() << std::endl;

  // now play track on the "active device" in spotify
  // the track id (uri) has to be in a vector, this is, because its possible to
  // play more than one track (put into queue)
  auto playRet = api.play(accessToken, vector<string>{tracks[d].getUri()});

  // check if something went wrong
  if (playRet.has_value()) {
    cerr << "Error occured! Message: " << playRet.value().getErrorMessage()
         << endl;
    return -1;
  }

  return 0;
}
