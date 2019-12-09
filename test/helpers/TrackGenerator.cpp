/*****************************************************************************/
/**
 * @file    TrackGenerator.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of TrackGenerator
 */
/*****************************************************************************/

#include "TrackGenerator.h"

#include <sstream>

using namespace std;

static string randomString(const int len = 10) {
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  stringstream ss;

  for (int i = 0; i < len; ++i) {
    ss << (char)alphanum[rand() % ((sizeof(alphanum) / sizeof(char)) - 1)];
  }

  return ss.str();
}

vector<BaseTrack> TrackGenerator::generateTracks(int nr) const {
  vector<BaseTrack> result;
  BaseTrack track;

  for (int i = 0; i < nr; ++i) {
    track.trackId = randomString();
    track.title = randomString();
    track.album = randomString();
    track.artist = randomString();
    track.duration = rand();
    track.iconUri = randomString();
    track.addedBy = randomString();

    result.push_back(track);
  }

  return result;
}

std::vector<QueuedTrack> TrackGenerator::generateQueuedTracks(int nr) const {
  vector<QueuedTrack> result;
  QueuedTrack track;

  for (int i = 0; i < nr; ++i) {
    track.trackId = randomString();
    track.title = randomString();
    track.album = randomString();
    track.artist = randomString();
    track.duration = rand();
    track.iconUri = randomString();
    track.addedBy = randomString();

    track.insertedAt = rand();
    track.votes = rand();
    track.currentVote = rand();

    result.push_back(track);
  }

  return result;
}

PlaybackTrack TrackGenerator::generatePlaybackTrack() const {
  PlaybackTrack track;
  track.trackId = randomString();
  track.title = randomString();
  track.album = randomString();
  track.artist = randomString();
  track.duration = rand();
  track.iconUri = randomString();
  track.addedBy = randomString();

  track.progressMs = rand();
  track.isPlaying = rand();

  return track;
}

QueueStatus TrackGenerator::generateQueueStatus(int normalNr,
                                                int adminNr,
                                                bool playback) const {
  QueueStatus status;

  status.normalQueue = Queue{generateQueuedTracks(normalNr)};
  status.adminQueue = Queue{generateQueuedTracks(adminNr)};

  status.currentTrack = nullopt;
  if (playback) {
    status.currentTrack = generatePlaybackTrack();
  }

  return status;
}
