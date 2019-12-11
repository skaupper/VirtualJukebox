/*****************************************************************************/
/**
 * @file    Serializer.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of class Serializer
 */
/*****************************************************************************/

#include "Utils/Serializer.h"

#include "Types/GlobalTypes.h"
#include "Types/Queue.h"

using json = nlohmann::json;

template <>
json Serializer::serialize<BaseTrack>(BaseTrack const &track) {
  json result;
  result["track_id"] = track.trackId;
  result["title"] = track.title;
  result["album"] = track.album;
  result["artist"] = track.artist;
  result["duration"] = track.durationMs;
  result["icon_uri"] = track.iconUri;
  result["added_by"] = track.addedBy;  // TODO: move to QueuedTrack!
  return result;
}

template <>
json Serializer::serialize<QueuedTrack>(QueuedTrack const &track) {
  json result = Serializer::serialize<BaseTrack>(track);
  result["votes"] = track.votes;
  result["current_vote"] = (track.currentVote ? 1 : 0);
  return result;
}

template <>
json Serializer::serialize<PlaybackTrack>(PlaybackTrack const &track) {
  json result = Serializer::serialize<BaseTrack>(track);
  result["playing"] = track.isPlaying;
  result["playing_for"] = track.progressMs;
  return result;
}
