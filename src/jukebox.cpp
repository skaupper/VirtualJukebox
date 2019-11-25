//------------------------------------------------------------------------------
// File        : jukebox.cpp
// Author      : Michael Wurm <wurm.michael95@gmail.com>
// Description : Class JukeBox implementation
//------------------------------------------------------------------------------

#include "jukebox.h"

using namespace std;

std::variant<bool, TError> JukeBox::start() {
  variant<bool, TError> ret;

  ret = true;
  return ret;
}

variant<string, TError> JukeBox::generateSession() {
  variant<string, TError> ret;

  //  ret = mMemory.generateSession();

  return ret;
}

variant<bool, TError> JukeBox::authenticateAdmin(string password) {
  variant<bool, TError> ret;

  if (password == mAdminPassword)
    ret = true;

  return ret;
}

/*
variant<vector<Track>, TError> JukeBox::queryMusic(string name) {
  variant<vector<Track>, TError> ret;

  return ret;
}

variant<vector<Track>, TError> JukeBox::getCurrentQueue(TQueueType queue) {
  variant<vector<Track>, TError> ret;

  return ret;
}

variant<bool, TError> JukeBox::addTrackToQueue(User user, Track track) {
  variant<bool, TError> ret;

  return ret;
}

variant<bool, TError> JukeBox::voteTrack(User user, Track track) {
  variant<bool, TError> ret;

  return ret;
}

variant<bool, TError> JukeBox::controlPlayer(User user, TPlayerAction action) {
  variant<bool, TError> ret;

  return ret;
}
*/
