/*****************************************************************************/
/**
 * @file    MockNetworkListener.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of a mock NetworkListener for testing purposes
 */
/*****************************************************************************/

#ifndef _MOCK_NETWORK_LISTENER_H_
#define _MOCK_NETWORK_LISTENER_H_

#include <tuple>

#include "NetworkListener.h"

class MockNetworkListener : public NetworkListener {
 public:
  MockNetworkListener();

  MockNetworkListener(MockNetworkListener const &) = delete;
  MockNetworkListener &operator=(MockNetworkListener const &) = delete;
  MockNetworkListener(MockNetworkListener &&) = delete;
  MockNetworkListener &&operator=(MockNetworkListener &&) = delete;

  //
  // Implementation of the NetworkListener interface
  //
  // Each method should store its last set of parameters,
  // count the number of calls and respond with a value set by the test cases.
  //
 private:
  TResult<TSessionID> generateSession(
      std::optional<TPassword> const &pw,
      std::optional<std::string> const &nickname) override;

  TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &searchPattern, size_t const nrOfEntries) override;

  TResult<QueueStatus> getCurrentQueues(TSessionID const &) override;

  TResultOpt addTrackToQueue(TSessionID const &sid,
                             TTrackID const &trkid,
                             QueueType type) override;

  TResultOpt voteTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       TVote vote) override;

  TResultOpt controlPlayer(TSessionID const &sid, PlayerAction action) override;

  TResultOpt removeTrack(TSessionID const &sid, TTrackID const &trkid) override;

  TResultOpt moveTrack(TSessionID const &sid,
                       TTrackID const &trkid,
                       QueueType type) override;

  //
  // Access functions for the test cases
  //
 public:
  // generateSession
  bool hasParametersGenerateSession();
  void getLastParametersGenerateSession(std::optional<TPassword> &,
                                        std::optional<std::string> &);
  size_t getCountGenerateSession();
  void setResponseGenerateSession(TSessionID const &);

  // queryTracks
  bool hasParametersQueryTracks();
  void getLastParametersQueryTracks(std::string &pattern, int &maxEntries);
  size_t getCountQueryTracks();
  void setResponseQueryTracks(std::vector<BaseTrack> const &tracks);

  // getCurrentQueues
  bool hasParametersGetCurrentQueues();
  void getLastParametersGetCurrentQueues(TSessionID &sid);
  size_t getCountGetCurrentQueues();
  void setResponseGetCurrentQueues(QueueStatus const &queueStatus);

  // TODO ...

  //
  // Store the parameter sets, responses and call counts for each request.
  //
  // Pretty nice types incoming! Each parameter set gets stored as a tuple which
  // is optional, so you can signal that the method hasn't been called.
  //
 private:
  // generateSession
  std::optional<
      std::tuple<std::optional<TPassword>, std::optional<std::string>>>
      mGenerateSessionParameters;
  size_t mGenerateSessionCount;
  TResult<TSessionID> mGenerateSessionResponse;

  // queryTracks
  std::optional<std::tuple<std::string, size_t>> mQueryTracksParameters;
  size_t mQueryTracksCount;
  TResult<std::vector<BaseTrack>> mQueryTracksResponse;

  // getCurrentQueues
  std::optional<std::string> mGetCurrentQueuesParameters;
  size_t mGetCurrentQueuesCount;
  TResult<QueueStatus> mGetCurrentQueuesResponse;

  // addTrackToQueue
  std::optional<std::tuple<TSessionID, TTrackID, QueueType>>
      mAddTrackToQueueParameters;
  size_t mAddTrackToQueueCount;
  TResultOpt mAddTrackToQueueResponse;

  // voteTrack
  std::optional<std::tuple<TSessionID, TTrackID, TVote>> mVoteTrackParameters;
  size_t mVoteTrackCount;
  TResultOpt mVoteTrackResponse;

  // controlPlayer
  std::optional<std::tuple<TSessionID, PlayerAction>> mControlPlayerParameters;
  size_t mControlPlayerCount;
  TResultOpt mControlPlayerResponse;

  // removeTrack
  std::optional<std::tuple<TSessionID, TTrackID>> mRemoveTrackParameters;
  size_t mRemoveTrackCount;
  TResultOpt mRemoveTrackResponse;

  // moveTrack
  std::optional<std::tuple<TSessionID, TTrackID, QueueType>>
      mMoveTrackParameters;
  size_t mMoveTrackCount;
  TResultOpt mMoveTrackResponse;
};

#endif /* _MOCK_NETWORK_LISTENER_H_ */
