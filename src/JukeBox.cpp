/*****************************************************************************/
/**
 * @file    JukeBox.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox implementation
 */
/*****************************************************************************/

#include "JukeBox.h"

#include <ctime>
#include <memory>

#include "Datastore/RAMDataStore.h"
#include "Network/RestAPI.h"
#include "Spotify/SpotifyBackend.h"
#include "Types/User.h"
#include "Utils/ConfigHandler.h"
#include "Utils/LoggingHandler.h"
#include "Utils/TrackScheduler.h"

using namespace std;

JukeBox::JukeBox() {
  mDataStore = new RAMDataStore();
  mNetwork = new RestAPI();
  mMusicBackend = new SpotifyBackend();

  mNetwork->setListener(this);
}

JukeBox::~JukeBox() {
  delete mDataStore;
  mDataStore = nullptr;
  delete mNetwork;
  mNetwork = nullptr;
  delete mMusicBackend;
  mMusicBackend = nullptr;
}

bool JukeBox::start(string const &exeName, string const &configFilePath) {
  auto conf = ConfigHandler::getInstance();
  auto ret = conf->setConfigFilePath(configFilePath);
  if (ret.has_value()) {
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: " << ret.value().getErrorMessage() << endl;
    return false;
  }

  initLoggingHandler(exeName);

  ret = mMusicBackend->initBackend();
  if (ret.has_value()) {
    LOG(ERROR) << "Failed to initialize music backend";
    return false;
  }

  // TODO: check for available devices here? only works if initBackend blocks
  // until an access token has been acquired

  TrackScheduler scheduler(mDataStore, mMusicBackend);
  scheduler.start();

  mNetwork->handleRequests();

  return true;
}

TResult<TSessionID> JukeBox::generateSession(optional<TPassword> const &pw,
                                             optional<string> const &nickname) {
  static int userID = 0;
  static mutex userIDMutex;
  User user;
  auto conf = ConfigHandler::getInstance();
  auto adminPw = conf->getValueString("MainParams", "adminPassword");
  if (holds_alternative<Error>(adminPw))
    return get<Error>(adminPw);

  string name = "(no nickname given)";
  if (nickname.has_value())
    name = nickname.value();
  user.Name = name;

  if (pw.has_value() && pw.value() == get<string>(adminPw)) {
    LOG(INFO) << "JukeBox.generateSession: User '" << name << "' is admin!";
    user.isAdmin = true;
  }

  /* Generate a unique ID, consisting of a counter and the number of seconds
   * since 1970.
   * Using a mutex, since this function may be called in multiple
   * threads and 'userID' is a static variable. */
  {
    unique_lock<mutex> lck(userIDMutex);
    user.SessionID = "ID" + to_string(userID) + to_string(time(nullptr));
    userID++;
  }

  mDataStore->addUser(user);

  return static_cast<TSessionID>(user.SessionID);
}

TResult<vector<BaseTrack>> JukeBox::queryTracks(string const &searchPattern,
                                                size_t const nrOfEntries) {
  auto tracks = mMusicBackend->queryTracks(searchPattern, nrOfEntries);
  return tracks;
}

TResult<QueueStatus> JukeBox::getCurrentQueues(TSessionID const &) {
  QueueStatus qs;

  auto ret = mDataStore->getQueue(QueueType::Admin);
  if (holds_alternative<Error>(ret))
    return get<Error>(ret);
  qs.adminQueue = get<Queue>(ret);

  ret = mDataStore->getQueue(QueueType::Normal);
  if (holds_alternative<Error>(ret))
    return get<Error>(ret);
  qs.normalQueue = get<Queue>(ret);

  /* Construct current PlaybackTrack through combining of information
   * in DataStore and Spotify */

  // TODO: the datastore should support optional playing tracks too
  auto trackStore = mDataStore->getPlayingTrack();
  if (holds_alternative<Error>(trackStore))
    return get<Error>(trackStore);

  QueuedTrack tmp = get<QueuedTrack>(trackStore);
  PlaybackTrack pbt;
  pbt.addedBy = tmp.addedBy;
  pbt.album = tmp.album;
  pbt.artist = tmp.artist;
  pbt.durationMs = tmp.durationMs;
  pbt.iconUri = tmp.iconUri;
  pbt.title = tmp.title;
  pbt.trackId = tmp.trackId;

  auto trackSpotify = mMusicBackend->getCurrentPlayback();
  if (holds_alternative<Error>(trackSpotify))
    return get<Error>(trackSpotify);

  auto pbtSpotifyOpt = get<optional<PlaybackTrack>>(trackSpotify);
  if (!pbtSpotifyOpt.has_value()) {
    // TODO: should double check with data store object
    qs.currentTrack = nullopt;
    return qs;
  }

  PlaybackTrack pbtSpotify = pbtSpotifyOpt.value();
  pbt.progressMs = pbtSpotify.progressMs;
  pbt.isPlaying = pbtSpotify.isPlaying;

  if (!(pbt == pbtSpotify)) {
    string msg =
        "Jukebox.getCurrentQueues: Inconsistency between current playback "
        "track in Spotify and DataStore";
    LOG(WARNING) << msg;
    return Error(ErrorCode::InvalidValue, msg);
  }

  qs.currentTrack = pbt;

  return qs;
}

TResultOpt JukeBox::addTrackToQueue(TSessionID const &sid,
                                    TTrackID const &trkid,
                                    QueueType type) {
  auto retUser = mDataStore->getUser(sid);
  if (holds_alternative<Error>(retUser))
    return get<Error>(retUser);
  User user = get<User>(retUser);

  if (type == QueueType::Admin && !user.isAdmin) {
    LOG(WARNING) << "JukeBox.addTrackToQueue: User with session ID '" << sid
                 << "' and nickname '" << user.Name
                 << "' is not priviledged to add a track to the admin queue.";
    return Error(ErrorCode::AccessDenied, "User is not an admin.");
  }

  auto query = mMusicBackend->createBaseTrack(trkid);
  if (holds_alternative<Error>(query)) {
    LOG(WARNING) << "JukeBox.addTrackToQueue: Could not add track for TrackID '"
                 << trkid << "'.";
    return get<Error>(query);
  }

  return mDataStore->addTrack(get<BaseTrack>(query), type);
}

TResultOpt JukeBox::voteTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              TVote vote) {
  return mDataStore->voteTrack(sid, trkid, vote);
}

TResultOpt JukeBox::removeTrack(TSessionID const &sid, TTrackID const &trkid) {
  auto retUser = mDataStore->getUser(sid);
  if (holds_alternative<Error>(retUser))
    return get<Error>(retUser);
  User user = get<User>(retUser);

  if (!user.isAdmin) {
    LOG(WARNING) << "JukeBox.removeTrack: User with session ID '" << sid
                 << "' and nickname '" << user.Name
                 << "' is not priviledged to remove a track.";
    return Error(ErrorCode::AccessDenied, "User is not an admin.");
  }

  /* Check, if TrackID exists in any queue */
  /* Admin queue */
  auto retQueue = mDataStore->getQueue(QueueType::Admin);
  if (holds_alternative<Error>(retQueue))
    return get<Error>(retQueue);
  Queue queue = get<Queue>(retQueue);
  bool trackFoundAdmin =
      get<bool>(mDataStore->hasTrack(trkid, QueueType::Admin));

  /* Normal queue */
  retQueue = mDataStore->getQueue(QueueType::Normal);
  if (holds_alternative<Error>(retQueue))
    return get<Error>(retQueue);
  queue = get<Queue>(retQueue);
  bool trackFoundNormal =
      get<bool>(mDataStore->hasTrack(trkid, QueueType::Normal));

  if (!trackFoundAdmin && !trackFoundNormal) {
    LOG(WARNING) << "Jukebox.moveTrack: TrackID '" << trkid
                 << "' could not be found.";
    return Error(ErrorCode::DoesntExist, "Track not found.");
  }
  if (trackFoundAdmin && trackFoundNormal) {
    /* TODO: This situation needs to be avoided in function DataStore.addTrack!
     */
    LOG(ERROR) << "Jukebox.moveTrack: TrackID '" << trkid
               << "' was found in both queues.";
    return Error(ErrorCode::InvalidFormat, "Track found in both queues.");
  }

  QueueType q;
  if (trackFoundAdmin)
    q = QueueType::Admin;
  if (trackFoundNormal)
    q = QueueType::Normal;

  auto retTrack = mDataStore->removeTrack(trkid, q);
  if (holds_alternative<Error>(retTrack))
    return get<Error>(retTrack);

  return nullopt;
}

TResultOpt JukeBox::moveTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              QueueType toQueue) {
  auto retUser = mDataStore->getUser(sid);
  if (holds_alternative<Error>(retUser))
    return get<Error>(retUser);
  User user = get<User>(retUser);

  if (!user.isAdmin) {
    LOG(WARNING) << "JukeBox.moveTrack: User with session ID '" << sid
                 << "' and nickname '" << user.Name
                 << "' is not priviledged to move a track.";
    return Error(ErrorCode::AccessDenied, "User is not an admin.");
  }

  /* Moving a track to another queue means deleting it
   * in the respective other */
  QueueType fromQueue;
  if (toQueue == QueueType::Admin)
    fromQueue = QueueType::Normal;
  if (toQueue == QueueType::Normal)
    fromQueue = QueueType::Admin;

  /* Query the source queue for the track that is to be deleted */
  bool trackFound = get<bool>(mDataStore->hasTrack(trkid, fromQueue));

  if (!trackFound) {
    LOG(WARNING) << "Jukebox.moveTrack: TrackID '" << trkid
                 << "' could not be found.";
    return Error(ErrorCode::DoesntExist, "Track not found.");
  }

  /* Remove from source queue */
  auto retTrack = mDataStore->removeTrack(trkid, fromQueue);
  if (holds_alternative<Error>(retTrack))
    return get<Error>(retTrack);

  /* Add to target queue */
  return mDataStore->addTrack(get<BaseTrack>(retTrack), toQueue);
}

TResultOpt JukeBox::controlPlayer(TSessionID const &sid, PlayerAction action) {
  int const volChangePercent = 10;
  auto retUser = mDataStore->getUser(sid);
  if (holds_alternative<Error>(retUser))
    return get<Error>(retUser);
  User user = get<User>(retUser);

  if (!user.isAdmin) {
    LOG(WARNING) << "JukeBox.controlPlayer: User with session ID '" << sid
                 << "' and nickname '" << user.Name
                 << "' is not priviledged to control the player.";
    return Error(ErrorCode::AccessDenied, "User is not an admin.");
  }

  TResultOpt ret = nullopt;
  TResult<size_t> volume;
  TResult<QueuedTrack> nextTrk;

  switch (action) {
    case PlayerAction::Play:
      ret = mMusicBackend->play();
      break;
    case PlayerAction::Pause:
      ret = mMusicBackend->pause();
      break;
    case PlayerAction::Stop:
      /* TODO: Implement an actual stop functionality? */
      ret = mMusicBackend->pause();
      break;
    case PlayerAction::Skip:
      ret = mDataStore->nextTrack();
      if (ret.has_value())
        return ret.value();

      nextTrk = mDataStore->getPlayingTrack();
      if (holds_alternative<Error>(nextTrk))
        return get<Error>(nextTrk);

      ret = mMusicBackend->setPlayback(get<QueuedTrack>(nextTrk));
      break;
    case PlayerAction::VolumeUp:
      volume = mMusicBackend->getVolume();
      if (holds_alternative<Error>(volume))
        return get<Error>(volume);

      ret = mMusicBackend->setVolume(get<size_t>(volume) + volChangePercent);
      break;
    case PlayerAction::VolumeDown:
      volume = mMusicBackend->getVolume();
      if (holds_alternative<Error>(volume))
        return get<Error>(volume);

      ret = mMusicBackend->setVolume(get<size_t>(volume) - volChangePercent);
      break;

    default:
      ret = Error(ErrorCode::NotImplemented,
                  "JukeBox.controlPlayer: Unhandled PlayerAction");
      break;
  }
  return ret;
}
