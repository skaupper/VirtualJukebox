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

  TResult<vector<BaseTrack>> queryTracks(string const &searchPattern,
                                         size_t const nrOfEntries) override {
    cout << "Pattern: " << searchPattern << endl;
    cout << "Number of entries: " << nrOfEntries << endl;

    vector<BaseTrack> tracks = {
        {"id1", "title1", "album1", "artist1", 123, "uri1", "me"},
        {"id2", "title2", "album2", "artist2", 223, "uri2", "me"},
        {"id3", "title3", "album3", "artist3", 323, "uri3", "me"},
        {"id4", "title4", "album4", "artist4", 423, "uri4", "me"}};
    return tracks;
  }

  TResult<QueueStatus> getCurrentQueues(TSessionID const &sid) override {
    cout << "Session ID: " << sid << endl;

    Queue normalQueue{{{"id1",
                        "title1",
                        "album1",
                        "artist1",
                        123,
                        "uri1",
                        "me",
                        1234,
                        true,
                        555555},
                       {"id2",
                        "title2",
                        "album2",
                        "artist2",
                        223,
                        "uri2",
                        "me",
                        1234,
                        true,
                        555555},
                       {"id3",
                        "title3",
                        "album3",
                        "artist3",
                        323,
                        "uri3",
                        "me",
                        1234,
                        true,
                        555555},
                       {"id4",
                        "title4",
                        "album4",
                        "artist4",
                        423,
                        "uri4",
                        "me",
                        1234,
                        true,
                        555555}}};

    Queue adminQueue{{{"adminid1",
                       "admintitle1",
                       "album1",
                       "artist1",
                       456,
                       "adminuri1",
                       "me",
                       1234,
                       true,
                       555555},
                      {"adminid2",
                       "admintitle2",
                       "album2",
                       "artist2",
                       336,
                       "adminuri2",
                       "me",
                       1234,
                       true,
                       555555},
                      {"adminid3",
                       "admintitle3",
                       "album3",
                       "artist3",
                       8888,
                       "adminuri3",
                       "me",
                       1234,
                       true,
                       555555},
                      {"adminid4",
                       "admintitle4",
                       "album4",
                       "artist4",
                       0,
                       "adminuri4",
                       "me",
                       1234,
                       true,
                       555555}}};

    PlaybackTrack currentTrack{"adminid1",
                               "admintitle1",
                               "album1",
                               "artist1",
                               456,
                               "adminuri1",
                               "me",
                               10,
                               false};

    QueueStatus status;
    status.normalQueue = normalQueue;
    status.adminQueue = adminQueue;
    status.currentTrack = currentTrack;
    return status;
  }

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
