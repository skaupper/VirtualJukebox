/** ----------------------------------------------------------------------------
 * @file    User.h
 * @author  Samuel Hick <samuel.hick@gmx.at>
 * @brief   Class User definition
 * ---------------------------------------------------------------------------*/

#ifndef _USER_H_
#define _USER_H_

#include <ctime>
#include <vector>

#include "Types/GlobalTypes.h"

class User {
 public:
  TSessionID SessionID;
  std::time_t ExpirationDate;
  std::string Name;
  bool isAdmin;
  std::vector<TTrackID> votes;
  bool operator==(const User user) {
    return SessionID == user.SessionID;
  }
};

#endif /* _USER_H_ */
