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
      "BQBoxQyiKxFjmzf78m4uhZSuN5h3yqykqxCn1KJkJ_OmKVWv1wCjKfWjKru54iFhO5iNWO_"
      "U8EnTgMx9FnGm_0C6ZN1HjTTWuwiEUoMGMbVUpM-iu361Zk9rIIk5cKj0VE0ay6mvGo75Et-"
      "MduQpYhm8FTexTLKxR0npMy691_Yh4V0pOWc0IX2Ae85ZVfPZJQ");
  SpotifyApi::SpotifyAPI api;
  api.getAvailableDevices(accessToken);
  api.getCurrentPlayback(accessToken);

  if (auto value = std::get_if<std::string>(&test)) {
    std::cout << *value << std::endl;
  } else {
    auto err = std::get<Error>(test);
    std::cout << err.getErrorMessage();
  }

  return 0;
}
