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

//#include "Track.h"
//#include "User.h"
//#include "PlayerAction.h"

typedef enum TQueueType { ADMIN, USER };

class JukeBox {
 public:
  void start();

 private:
  std::variant<bool, std::string> generateSession();
  std::variant<bool, std::string> authenticateAdmin(std::string password);
  std::variant<std::vector<Track>, std::string> queryMusic(std::string name);
  std::variant<std::vector<Track>, std::string> getCurrentQueue(
      TQueueType queue);
  std::variant<bool, std::string> addTrackToQueue(User user, Track track);
  std::variant<bool, std::string> voteTrack(User user, Track track);
  std::variant<bool, std::string> controlPlayer(User user,
                                                TPlayerAction action);
};

#endif /* _JUKEBOX_H_ */
