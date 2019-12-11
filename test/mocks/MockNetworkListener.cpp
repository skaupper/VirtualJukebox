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
  return {};
}

TResultOpt MockNetworkListener::voteTrack(TSessionID const &sid,
                                          TTrackID const &trkid,
                                          TVote vote) {
  mVoteTrackParameters = tuple{sid, trkid, vote};
  mVoteTrackCount++;
  return {};
}

TResultOpt MockNetworkListener::controlPlayer(TSessionID const &sid,
                                              PlayerAction action) {
  mControlPlayerParameters = tuple{sid, action};
  mControlPlayerCount++;
  return {};
}

TResultOpt MockNetworkListener::removeTrack(TSessionID const &sid,
                                            TTrackID const &trkid) {
  mRemoveTrackParameters = tuple{sid, trkid};
  mRemoveTrackCount++;
  return {};
}

TResultOpt MockNetworkListener::moveTrack(TSessionID const &sid,
                                          TTrackID const &trkid,
                                          QueueType type) {
  mMoveTrackParameters = tuple{sid, trkid, type};
  mMoveTrackCount++;
  return {};
}

//
// Access functions for the test cases
//

// generateSession
bool MockNetworkListener::hasParametersGenerateSession() {
  return mGenerateSessionParameters.has_value();
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

// queryTracks
bool MockNetworkListener::hasParametersQueryTracks() {
  return mQueryTracksParameters.has_value();
}

void MockNetworkListener::getLastParametersQueryTracks(string &pattern,
                                                       int &maxEntries) {
  tie(pattern, maxEntries) = mQueryTracksParameters.value();
  mQueryTracksParameters = nullopt;
}

size_t MockNetworkListener::getCountQueryTracks() {
  return mQueryTracksCount;
}
void MockNetworkListener::setResponseQueryTracks(
    vector<BaseTrack> const &tracks) {
  mQueryTracksResponse = tracks;
}

// getCurrentQueues
bool MockNetworkListener::hasParametersGetCurrentQueues() {
  return mGetCurrentQueuesParameters.has_value();
}

void MockNetworkListener::getLastParametersGetCurrentQueues(TSessionID &sid) {
  sid = mGetCurrentQueuesParameters.value();
  mGetCurrentQueuesParameters = nullopt;
}

size_t MockNetworkListener::getCountGetCurrentQueues() {
  return mGetCurrentQueuesCount;
}
void MockNetworkListener::setResponseGetCurrentQueues(
    QueueStatus const &queueStatus) {
  mGetCurrentQueuesResponse = queueStatus;
}

// addTrackToQueue
bool MockNetworkListener::hasParametersAddTrackToQueue() {
  return mAddTrackToQueueParameters.has_value();
}

void MockNetworkListener::getLastParametersAddTrackToQueue(
    TSessionID &sid, TTrackID &trkid, QueueType &queueType) {
  tie(sid, trkid, queueType) = mAddTrackToQueueParameters.value();
  mAddTrackToQueueParameters = nullopt;
}

size_t MockNetworkListener::getCountAddTrackToQueue() {
  return mAddTrackToQueueCount;
}

// voteTrack
bool MockNetworkListener::hasParametersVoteTrack() {
  return mVoteTrackParameters.has_value();
}

void MockNetworkListener::getLastParametersVoteTrack(TSessionID &sid,
                                                     TTrackID &trkid,
                                                     TVote &vote) {
  tie(sid, trkid, vote) = mVoteTrackParameters.value();
  mVoteTrackParameters = nullopt;
}

size_t MockNetworkListener::getCountVoteTrack() {
  return mVoteTrackCount;
}

// controlPlayer
bool MockNetworkListener::hasParametersControlPlayer() {
  return mControlPlayerParameters.has_value();
}

void MockNetworkListener::getLastParametersControlPlayer(TSessionID &sid,
                                                         PlayerAction &action) {
  tie(sid, action) = mControlPlayerParameters.value();
  mControlPlayerParameters = nullopt;
}

size_t MockNetworkListener::getCountControlPlayer() {
  return mControlPlayerCount;
}

// moveTrack
bool MockNetworkListener::hasParametersMoveTrack() {
  return mMoveTrackParameters.has_value();
}

void MockNetworkListener::getLastParametersMoveTrack(TSessionID &sid,
                                                     TTrackID &trkid,
                                                     QueueType &queueType) {
  tie(sid, trkid, queueType) = mMoveTrackParameters.value();
  mMoveTrackParameters = nullopt;
}

size_t MockNetworkListener::getCountMoveTrack() {
  return mMoveTrackCount;
}

// removeTrack
bool MockNetworkListener::hasParametersRemoveTrack() {
  return mRemoveTrackParameters.has_value();
}

void MockNetworkListener::getLastParametersRemoveTrack(TSessionID &sid,
                                                       TTrackID &trkid) {
  tie(sid, trkid) = mRemoveTrackParameters.value();
  mRemoveTrackParameters = nullopt;
}

size_t MockNetworkListener::getCountRemoveTrack() {
  return mRemoveTrackCount;
}
