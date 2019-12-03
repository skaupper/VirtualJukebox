/*****************************************************************************/
/**
 * @file    Serializer.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class Serializer
 */
/*****************************************************************************/
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "GlobalTypes.h"
#include "json/json.hpp"

#include "Queue.h"


class Serializer {
public:
    template <class T>
    static nlohmann::json serialize(T const &);
};

template <>
nlohmann::json Serializer::serialize<BaseTrack>(BaseTrack const &track)
{
    nlohmann::json result;
    result["track_id"] = track.trackId;
    result["title"] = track.title;
    result["album"] = track.album;
    result["artist"] = track.artist;
    result["duration"] = track.duration;
    result["icon_uri"] = track.iconUri;
    result["added_by"] = track.addedBy; // TODO: move to QueuedTrack!
    return result;
}

template <>
nlohmann::json Serializer::serialize<QueuedTrack>(QueuedTrack const &track)
{
    nlohmann::json result = Serializer::serialize<BaseTrack>(track);
    result["votes"] = track.votes;
    result["current_vote"] = (track.currentVote ? 1 : 0);
    return result;
}


template <>
nlohmann::json Serializer::serialize<PlaybackTrack>(PlaybackTrack const &track)
{
    nlohmann::json result = Serializer::serialize<BaseTrack>(track);
    result["playing"] = track.isPlaying;
    result["playing_for"] = track.progressMs;
    return result;
}



#endif /* _SERIALIZER_H_ */
