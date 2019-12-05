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

#include <glog/logging.h>

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
    LOG(INFO) << "generateSession";
    if (pw.has_value()) {
      LOG(INFO) << "Password: " << pw.value();
    } else {
      LOG(INFO) << "No password";
    }
    if (nickname.has_value()) {
      LOG(INFO) << "Nickname: " << nickname.value();
    } else {
      LOG(INFO) << "No nickname";
    }
    return static_cast<TSessionID>("12345678");
  }

  TResult<vector<BaseTrack>> queryTracks(string const &searchPattern,
                                         size_t const nrOfEntries) override {
    LOG(INFO) << "Pattern: " << searchPattern;
    LOG(INFO) << "Number of entries: " << nrOfEntries;

    return TRACK_LIST;
  }

  TResult<QueueStatus> getCurrentQueues(TSessionID const &sid) override {
    LOG(INFO) << "Session ID: " << sid;

    QueueStatus status;
    status.normalQueue = NORMAL_QUEUE;
    status.adminQueue = ADMIN_QUEUE;
    status.currentTrack = CURRENT_TRACK;
    return status;
  }

  TResultOpt addTrackToQueue(TSessionID const &sid,
                             TTrackID const &trkid,
                             QueueType type) override {
    LOG(INFO) << "Session ID: " << sid;
    LOG(INFO) << "Track ID: " << trkid;
    if (type == QueueType::Normal) {
      LOG(INFO) << "Normal queue";
    } else if (type == QueueType::Admin) {
      LOG(INFO) << "Admin queue";
    } else {
      return Error(ErrorCode::InvalidValue, "Unknown player action");
    }
    return nullopt;
  }

  TResultOpt voteTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       TVote vote) override {
    LOG(INFO) << "Session ID: " << sid;
    LOG(INFO) << "Track ID: " << trkid;
    if (vote) {
      LOG(INFO) << "Vote set";
    } else {
      LOG(INFO) << "Vote revoked";
    }
    return nullopt;
  }

  TResultOpt controlPlayer(TSessionID const &sid,
                           PlayerAction action) override {
    LOG(INFO) << "Session ID: " << sid;
    switch (action) {
      case PlayerAction::Play:
        LOG(INFO) << "Play";
        break;
      case PlayerAction::Pause:
        LOG(INFO) << "Pause";
        break;
      case PlayerAction::Stop:
        LOG(INFO) << "Stop";
        break;
      case PlayerAction::Skip:
        LOG(INFO) << "Skip";
        break;
      case PlayerAction::VolumeDown:
        LOG(INFO) << "VolumeDown";
        break;
      case PlayerAction::VolumeUp:
        LOG(INFO) << "VolumeUp";
        break;
      default:
        return Error(ErrorCode::InvalidValue, "Unknown player action");
    }
    return nullopt;
  }

  TResultOpt removeTrack(TSessionID const &sid,
                         TTrackID const &trkid) override {
    LOG(INFO) << "Session ID: " << sid;
    LOG(INFO) << "Track ID: " << trkid;
    return nullopt;
  }
  TResultOpt moveTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       QueueType type) override {
    LOG(INFO) << "Session ID: " << sid;
    LOG(INFO) << "Track ID: " << trkid;
    return nullopt;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << string(argv[0]) << " <path_to_config_file>" << endl;
    return 1;
  }

  EmptyNetworkListener listener;
  RestAPI api;

  ConfigHandler::getInstance()->setConfigFilePath(argv[1]);
  initLoggingHandler(argv[0]);

  api.setListener(&listener);
  auto result = api.handleRequests();
  if (result.has_value()) {
    LOG(ERROR) << result.value().getErrorMessage();
  }

  return 0;
}
