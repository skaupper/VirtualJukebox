/**
 * @file    empty_network_listener.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Minimal example using the REST interface.
 *
 * @details This example is meant to be a lightweight testing program for client
 * development. In contrast to the main application this example does not
 * actually do anything with the incoming data. Use the logging mechanism to get
 * information about recognized requests.
 *
 * Listens to connections on port`8080`.
 */

#include <iostream>

#include "ConfigHandler.h"
#include "RestAPI.h"

using namespace std;

class EmptyNetworkListener : public NetworkListener {
  TResult<TSessionID> generateSession(TPassword const &pw) override {
    cout << "generateSession" << endl;
    return static_cast<TSessionID>("12345678");
  }

  TResult<vector<Track>> queryTracks(string const &searchPattern,
                                     size_t const nrOfEntries) override {
    cout << "queryTracks" << endl;
    return {};
  }

  //   TResult<QueueStatus> getCurrentQueues() override{
  //     cout << "getCurrentQueues" << endl;
  //   }

  TResultOpt addTrackToQueue(TSessionID const &sid,
                             TTrackID const &trkid,
                             QueueType type) override {
    cout << "addTrackToQueue" << endl;
    return nullopt;
  }

  TResultOpt voteTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       TVote vote) override {
    cout << "voteTrack" << endl;
    return nullopt;
  }

  TResultOpt controlPlayer(TSessionID const &sid,
                           PlayerAction action) override {
    cout << "controlPlayer" << endl;
    return nullopt;
  }

  TResultOpt removeTrack(TSessionID const &sid,
                         TTrackID const &trkid) override {
    cout << "removeTrack" << endl;
    return nullopt;
  }
  TResultOpt moveTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       QueueType type) override {
    cout << "removeTrack" << endl;
    return nullopt;
  }
};

int main(int argc, char *argv[]) {
  EmptyNetworkListener listener;
  RestAPI api;

  ConfigHandler::getInstance()->setConfigFilePath("../jukebox_config.ini");

  api.setListener(&listener);
  auto result = api.handleRequests();
  if (result.has_value()) {
    cerr << result.value().getErrorMessage() << std::endl;
  }

  return 0;
}
