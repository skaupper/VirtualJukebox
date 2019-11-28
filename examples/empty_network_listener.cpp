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

#include "RestAPI.h"

using namespace std;

class EmptyNetworkListener : public NetworkListener {
  TResult<TSessionID> generateSession(TPassword pw) {
    cout << "generateSession" << endl;
    return static_cast<TSessionID>("12345678");
  }

  TResult<vector<Track>> queryMusic(string searchPattern) {
    cout << "queryMusic" << endl;
    return {};
  }

  TResult<Queues> getCurrentQueues() {
    cout << "getCurrentQueues" << endl;
    return Error(ErrorCode::AccessDedied, "currentQueues");
  }

  TResultOpt addTrackToQueue(TSessionID sid, TTrackID trkid, QueueType type) {
    cout << "addTrackToQueue" << endl;
    return nullopt;
  }

  TResultOpt voteTrack(TSessionID sid, TTrackID trkid) {
    cout << "voteTrack" << endl;
    return nullopt;
  }

  TResultOpt controlPlayer(TSessionID sid, PlayerAction action) {
    cout << "controlPlayer" << endl;
    return nullopt;
  }
};

int main(int argc, char *argv[]) {
  EmptyNetworkListener listener;
  RestAPI api;

  api.setListener(&listener);
  api.handleRequests();

  return 0;
}
