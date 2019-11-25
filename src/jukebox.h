//------------------------------------------------------------------------------
// File        : jukebox.h
// Author      : Michael Wurm <wurm.michael95@gmail.com>
// Description : Class JukeBox definition
//------------------------------------------------------------------------------

#ifndef _JUKEBOX_H_
#define _JUKEBOX_H_

#include <string>
#include <variant>
#include <vector>

//#include "Memory.h"
//#include "PlayerAction.h"
//#include "Track.h"
//#include "User.h"

typedef std::string TError;

class JukeBox {
 public:
  void start();

 private:
  std::variant<std::string, TError> generateSession();
  std::variant<bool, TError> authenticateAdmin(std::string password);
  std::variant<std::vector<Track>, TError> queryMusic(std::string name);
  std::variant<std::vector<Track>, TError> getCurrentQueue(TQueueType queue);
  std::variant<bool, TError> addTrackToQueue(User user, Track track);
  std::variant<bool, TError> voteTrack(User user, Track track);
  std::variant<bool, TError> controlPlayer(User user, TPlayerAction action);

  std::string const mAdminPassword = "admin";

  //  Memory mMem;
};

#endif /* _JUKEBOX_H_ */
