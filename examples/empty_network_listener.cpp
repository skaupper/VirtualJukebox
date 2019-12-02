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
  TResult<TSessionID> generateSession(
      optional<TPassword> const &pw,
      optional<string> const &nickname) override {
    cout << "generateSession" << endl;
    if (pw.has_value()) {
      cout << "Password: " << pw.value() << endl;
    } else {
      cout << "No password" << endl;
    }
    if (nickname.has_value()) {
      cout << "Nickname: " << nickname.value() << endl;
    } else {
      cout << "No nickname" << endl;
    }
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
    cout << "Session ID: " << sid << endl;
    cout << "Track ID: " << trkid << endl;
    if (type == QueueType::Normal) {
      cout << "Normal queue" << endl;
    } else if (type == QueueType::Admin) {
      cout << "Admin queue" << endl;
    } else {
      return Error(ErrorCode::InvalidValue, "Unknown player action");
    }
    return nullopt;
  }

  TResultOpt voteTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       TVote vote) override {
    cout << "Session ID: " << sid << endl;
    cout << "Track ID: " << trkid << endl;
    if (vote) {
      cout << "Vote set" << endl;
    } else {
      cout << "Vote revoked" << endl;
    }
    return nullopt;
  }

  TResultOpt controlPlayer(TSessionID const &sid,
                           PlayerAction action) override {
    cout << "Session ID: " << sid << endl;
    switch (action) {
      case PlayerAction::Play:
        cout << "Play" << endl;
        break;
      case PlayerAction::Pause:
        cout << "Pause" << endl;
        break;
      case PlayerAction::Stop:
        cout << "Stop" << endl;
        break;
      case PlayerAction::Skip:
        cout << "Skip" << endl;
        break;
      case PlayerAction::VolumeDown:
        cout << "VolumeDown" << endl;
        break;
      case PlayerAction::VolumeUp:
        cout << "VolumeUp" << endl;
        break;
      default:
        return Error(ErrorCode::InvalidValue, "Unknown player action");
    }
    return nullopt;
  }

  TResultOpt removeTrack(TSessionID const &sid,
                         TTrackID const &trkid) override {
    cout << "Session ID: " << sid << endl;
    cout << "Track ID: " << trkid << endl;
    return nullopt;
  }
  TResultOpt moveTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       QueueType type) override {
    cout << "Session ID: " << sid << endl;
    cout << "Track ID: " << trkid << endl;
    return nullopt;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <path_to_config_file>" << endl;
    return 0;
  }

  EmptyNetworkListener listener;
  RestAPI api;

  ConfigHandler::getInstance()->setConfigFilePath(argv[1]);

  api.setListener(&listener);
  auto result = api.handleRequests();
  if (result.has_value()) {
    cerr << result.value().getErrorMessage() << endl;
  }

  return 0;
}
