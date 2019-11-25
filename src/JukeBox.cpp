/** ----------------------------------------------------------------------------
 * @file    JukeBox.cpp
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class JukeBox implementation
 * ---------------------------------------------------------------------------*/

#include "JukeBox.h"

using namespace std;

variant<bool, Error> JukeBox::start() {
  variant<bool, Error> ret;

  ret = true;
  return ret;
}

variant<string, Error> JukeBox::generateSession() {
  variant<string, Error> ret;

  //  ret = mMemory.generateSession();

  return ret;
}

variant<bool, Error> JukeBox::authenticateAdmin(string password) {
  variant<bool, Error> ret;

  if (password == mAdminPassword)
    ret = true;

  return ret;
}

/*
variant<vector<Track>, Error> JukeBox::queryMusic(string name) {
  variant<vector<Track>, Error> ret;

  return ret;
}

variant<vector<Track>, Error> JukeBox::getCurrentQueue(TQueueType queue) {
  variant<vector<Track>, Error> ret;

  return ret;
}

variant<bool, Error> JukeBox::addTrackToQueue(User user, Track track) {
  variant<bool, Error> ret;

  return ret;
}

variant<bool, Error> JukeBox::voteTrack(User user, Track track) {
  variant<bool, Error> ret;

  return ret;
}

variant<bool, Error> JukeBox::controlPlayer(User user, TPlayerAction action) {
  variant<bool, Error> ret;

  return ret;
}
*/
