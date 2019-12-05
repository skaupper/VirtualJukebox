#include "MockNetworkListener.h"

#include <tuple>

using namespace std;

MockNetworkListener::MockNetworkListener()
    : mGenerateSessionCount(0),
      mQueryTracksCount(0),
      mGetCurrentQueuesCount(0),
      mAddTrackToQueueCount(0),
      mVoteTrackCount(0),
      mControlPlayerCount(0),
      mRemoveTrackCount(0),
      mMoveTrackCount(0) {
}

//
// Implementation of the NetworkListener interface
//

TResult<TSessionID> MockNetworkListener::generateSession(
    optional<TPassword> const &pw, optional<string> const &nickname) {
  mGenerateSessionParameters = tuple{pw, nickname};
  mGenerateSessionCount++;
  return mGenerateSessionResponse;
}

TResult<vector<BaseTrack>> MockNetworkListener::queryTracks(
    string const &searchPattern, size_t const nrOfEntries) {
  mQueryTracksParameters = tuple{searchPattern, nrOfEntries};
  mQueryTracksCount++;
  return mQueryTracksResponse;
}

TResult<QueueStatus> MockNetworkListener::getCurrentQueues(
    TSessionID const &sid) {
  mGetCurrentQueuesParameters = sid;
  mGetCurrentQueuesCount++;
  return mGetCurrentQueuesResponse;
}

TResultOpt MockNetworkListener::addTrackToQueue(TSessionID const &sid,
                                                TTrackID const &trkid,
                                                QueueType type) {
  mAddTrackToQueueParameters = tuple{sid, trkid, type};
  mAddTrackToQueueCount++;
  return mAddTrackToQueueResponse;
}

TResultOpt MockNetworkListener::voteTrack(TSessionID const &sid,
                                          TTrackID const &trkid,
                                          TVote vote) {
  mVoteTrackParameters = tuple{sid, trkid, vote};
  mVoteTrackCount++;
  return mVoteTrackResponse;
}

TResultOpt MockNetworkListener::controlPlayer(TSessionID const &sid,
                                              PlayerAction action) {
  mControlPlayerParameters = tuple{sid, action};
  mControlPlayerCount++;
  return mControlPlayerResponse;
}

TResultOpt MockNetworkListener::removeTrack(TSessionID const &sid,
                                            TTrackID const &trkid) {
  mRemoveTrackParameters = tuple{sid, trkid};
  mRemoveTrackCount++;
  return mRemoveTrackResponse;
}

TResultOpt MockNetworkListener::moveTrack(TSessionID const &sid,
                                          TTrackID const &trkid,
                                          QueueType type) {
  mMoveTrackParameters = tuple{sid, trkid, type};
  mMoveTrackCount++;
  return mMoveTrackResponse;
}

//
// Access functions for the test cases
//

// generateSession
bool MockNetworkListener::hasParametersGenerateSession() {
  if (!mGenerateSessionParameters.has_value()) {
    return false;
  }
  return true;
}

void MockNetworkListener::getLastParametersGenerateSession(
    optional<TPassword> &pw, optional<string> &nickname) {
  tie(pw, nickname) = mGenerateSessionParameters.value();
  mGenerateSessionParameters = nullopt;
}

size_t MockNetworkListener::getCountGenerateSession() {
  return mGenerateSessionCount;
}
void MockNetworkListener::setResponseGenerateSession(TSessionID const &resp) {
  mGenerateSessionResponse = resp;
}
