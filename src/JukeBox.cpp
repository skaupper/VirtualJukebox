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
#include <sstream>

#include "Types/GlobalTypes.h"
#include "Types/Result.h"
#include "Utils/ConfigHandler.h"
#include "Utils/LoggingHandler.h"

using namespace std;

bool JukeBox::start(string exeName, string configFilePath) {
  auto conf = ConfigHandler::getInstance();
  auto ret = conf->setConfigFilePath(configFilePath);
  if (ret.has_value()) {
    /* Print to cerr here, since LoggingHandler is uninitialized */
    cerr << "ERROR: " << ret.value().getErrorMessage() << endl;

    return false;
  }

  initLoggingHandler(exeName);
  DLOG(INFO) << "Hello world from JukeBox main.cpp !";
  return true;
}

TResult<TSessionID> JukeBox::generateSession(optional<TPassword> const &pw,
                                             optional<string> const &nickname) {
  // User user;
  auto conf = ConfigHandler::getInstance();
  auto adminPw = conf->getValueString("MainParams", "adminPassword");
  if (holds_alternative<Error>(adminPw))
    return get<Error>(adminPw);

  string name = "(no nickname given)";
  if (nickname.has_value()) {
    name = nickname.value();
    // user.Name = name;
  }

  if (pw.has_value() && pw.value() == get<string>(adminPw)) {
    LOG(INFO) << "JukeBox.generateSession: User '" << name << "' is admin!";
    // user.isAdmin = true;
  }
  // mDataStore.addUser(user);

  stringstream ss;
  ss << time(nullptr);
  return static_cast<TSessionID>(ss.str());
}

TResult<vector<BaseTrack>> JukeBox::queryTracks(string const &searchPattern,
                                                size_t const nrOfEntries) {
  return Error(ErrorCode::NotImplemented, "queryTracks is not implemented yet");
}

TResult<QueueStatus> JukeBox::getCurrentQueues(TSessionID const &) {
  QueueStatus qs;
  //  qs.adminQueue = mDataStore.getQueue(QueueType::Admin);
  //  if (holds_alternative<Error>(qs.adminQueue))
  //    return get<Error>(qs.adminQueue);
  //
  //  qs.normalQueue = mDataStore.getQueue(QueueType::Normal);
  //  if (holds_alternative<Error>(qs.normalQueue))
  //    return get<Error>(qs.normalQueue);
  //
  //  qs.currentTrack = mDataStore.getPlayingTrack();
  //  if (holds_alternative<Error>(qs.currentTrack))
  //    return get<Error>(qs.currentTrack);
  //
  return qs;
}

TResultOpt JukeBox::addTrackToQueue(TSessionID const &sid,
                                    TTrackID const &trkid,
                                    QueueType type) {
  /* TODO:
   * where does this function get the actual Track object/information from?
   */
  // auto ret = mDataStore.addTrack(TRACK ???, type);
  // if (ret.has_value())
  //   return ret.value();

  return Error(ErrorCode::NotImplemented,
               "addTrackToQueue is not implemented yet");
}

TResultOpt JukeBox::voteTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              TVote vote) {
  //  auto ret = mDataStore.voteTrack(sid, trkid, vote);
  //  if (ret.has_value())
  //    return ret.value();

  return Error(ErrorCode::NotImplemented, "voteTrack is not implemented yet");
}

TResultOpt JukeBox::removeTrack(TSessionID const &sid, TTrackID const &trkid) {
  //  auto user = mDataStore.getUser(sid);
  //  if (holds_alternative<Error>(user))
  //    return get<Error>(user);
  //
  //  if (!user.isAdmin) {
  //    LOG(WARNING) << "JukeBox.removeTrack: User with session ID '" << sid
  //                 << "' and nickname '" << user.name
  //                 << "' is not priviledged to remove a track.";
  //    return Error(ErrorCode::AccessDenied, "User is not an admin.");
  //  }

  //  auto ret = mDataStore.removeTrack(trkid);
  //  if (ret.has_value())
  //    return ret.value();
  return Error(ErrorCode::NotImplemented, "removeTrack is not implemented yet");
}

TResultOpt JukeBox::moveTrack(TSessionID const &sid,
                              TTrackID const &trkid,
                              QueueType type) {
  //  auto user = mDataStore.getUser(sid);
  //  if (holds_alternative<Error>(user))
  //    return get<Error>(user);
  //
  //  if (!user.isAdmin) {
  //    LOG(WARNING) << "JukeBox.moveTrack: User with session ID '" << sid
  //                 << "' and nickname '" << user.name
  //                 << "' is not priviledged to move a track.";
  //    return Error(ErrorCode::AccessDenied, "User is not an admin.");
  //  }

  /* TODO: how to implement this?
   *  can't use MusicBackend.queryMusic() for that, since it might
   *  return multiple/no results?
   *
   * logical way for me would be:
   *   - remove track by calling DataStore.removeTrack
   *   - above function returns the deleted object
   *   - add it to other queue with DataStore.addTrack
   *
   * like:
   *  mDataStore.addTrack(mDataStore.removeTrack(trkid));
   */

  return Error(ErrorCode::NotImplemented, "moveTrack is not implemented yet");
}

TResultOpt JukeBox::controlPlayer(TSessionID const &sid, PlayerAction action) {
  int const volChangePercent = 10;
  //  auto user = mDataStore.getUser(sid);
  //  if (holds_alternative<Error>(user))
  //    return get<Error>(user);
  //
  //  if (!user.isAdmin) {
  //    LOG(WARNING) << "JukeBox.controlPlayer: User with session ID '" << sid
  //                 << "' and nickname '" << user.name
  //                 << "' is not priviledged to control the player.";
  //    return Error(ErrorCode::AccessDenied, "User is not an admin.");
  //  }
  TResultOpt ret;
  TResult<size_t> volume;
  switch (action) {
    case PlayerAction::Play:
      ret = mMusicBackend->play();
      if (ret.has_value())
        return ret.value();
      break;
    case PlayerAction::Pause:
      ret = mMusicBackend->pause();
      if (ret.has_value())
        return ret.value();
      break;
    case PlayerAction::Stop:
      ret = mMusicBackend->pause();  // TODO: there is no 'stop' function?
      if (ret.has_value())
        return ret.value();
      break;
    case PlayerAction::Skip:
      // auto next = mDataStore.nextTrack();
      // if (next.has_value())
      //  return next.value();

      // ret = mMusicBackend->setPlayback(next);
      // if (ret.has_value())
      //  return ret.value();
      break;
    case PlayerAction::VolumeUp:
      volume = mMusicBackend->getVolume();
      if (holds_alternative<Error>(volume))
        return get<Error>(volume);

      ret = mMusicBackend->setVolume(get<size_t>(volume) + volChangePercent);
      if (ret.has_value())
        return ret.value();
      break;
    case PlayerAction::VolumeDown:
      volume = mMusicBackend->getVolume();
      if (holds_alternative<Error>(volume))
        return get<Error>(volume);

      ret = mMusicBackend->setVolume(get<size_t>(volume) + volChangePercent);
      if (ret.has_value())
        return ret.value();
      break;

    default:
      return Error(ErrorCode::NotImplemented,
                   "JukeBox.controlPlayer: Unhandled PlayerAction");
      break;
  }

  return Error(ErrorCode::NotImplemented,
               "controlPlayer is not implemented yet");
}
