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
 * The listened port can be configured using a key `port` in the section
 * `RestAPI` in the INI file.
 */

#include <iostream>

#include "ConfigHandler.h"
#include "DummyData.h"
#include "LoggingHandler.h"
#include "RestAPI.h"

using namespace std;

class EmptyNetworkListener : public NetworkListener {
  TResult<TSessionID> generateSession(
      optional<TPassword> const &pw,
      optional<string> const &nickname) override {
    logInfo("generateSession");
    if (pw.has_value()) {
      logInfo("Password: " + pw.value());
    } else {
      logInfo("No password");
    }
    if (nickname.has_value()) {
      logInfo("Nickname: " + nickname.value());
    } else {
      logInfo("No nickname");
    }
    return static_cast<TSessionID>("12345678");
  }

  TResult<vector<BaseTrack>> queryTracks(string const &searchPattern,
                                         size_t const nrOfEntries) override {
    logInfo("Pattern: " + searchPattern);
    logInfo("Number of entries: " + nrOfEntries);

    return TRACK_LIST;
  }

  TResult<QueueStatus> getCurrentQueues(TSessionID const &sid) override {
    logInfo("Session ID: " + sid);

    QueueStatus status;
    status.normalQueue = NORMAL_QUEUE;
    status.adminQueue = ADMIN_QUEUE;
    status.currentTrack = CURRENT_TRACK;
    return status;
  }

  TResultOpt addTrackToQueue(TSessionID const &sid,
                             TTrackID const &trkid,
                             QueueType type) override {
    logInfo("Session ID: " + sid);
    logInfo("Track ID: " + trkid);
    if (type == QueueType::Normal) {
      logInfo("Normal queue");
    } else if (type == QueueType::Admin) {
      logInfo("Admin queue");
    } else {
      return Error(ErrorCode::InvalidValue, "Unknown player action");
    }
    return nullopt;
  }

  TResultOpt voteTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       TVote vote) override {
    logInfo("Session ID: " + sid);
    logInfo("Track ID: " + trkid);
    if (vote) {
      logInfo("Vote set");
    } else {
      logInfo("Vote revoked");
    }
    return nullopt;
  }

  TResultOpt controlPlayer(TSessionID const &sid,
                           PlayerAction action) override {
    logInfo("Session ID: " + sid);
    switch (action) {
      case PlayerAction::Play:
        logInfo("Play");
        break;
      case PlayerAction::Pause:
        logInfo("Pause");
        break;
      case PlayerAction::Stop:
        logInfo("Stop");
        break;
      case PlayerAction::Skip:
        logInfo("Skip");
        break;
      case PlayerAction::VolumeDown:
        logInfo("VolumeDown");
        break;
      case PlayerAction::VolumeUp:
        logInfo("VolumeUp");
        break;
      default:
        return Error(ErrorCode::InvalidValue, "Unknown player action");
    }
    return nullopt;
  }

  TResultOpt removeTrack(TSessionID const &sid,
                         TTrackID const &trkid) override {
    logInfo("Session ID: " + sid);
    logInfo("Track ID: " + trkid);
    return nullopt;
  }
  TResultOpt moveTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       QueueType type) override {
    logInfo("Session ID: " + sid);
    logInfo("Track ID: " + trkid);
    return nullopt;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    logError("Usage: " + string(argv[0]) + " <path_to_config_file>");
    return 0;
  }

  EmptyNetworkListener listener;
  RestAPI api;

  ConfigHandler::getInstance()->setConfigFilePath(argv[1]);

  api.setListener(&listener);
  auto result = api.handleRequests();
  if (result.has_value()) {
    cerr << result.value().getErrorMessage();
  }

  return 0;
}
