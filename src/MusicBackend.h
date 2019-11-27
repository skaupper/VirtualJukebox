/** ----------------------------------------------------------------------------
 * @file    JukeBox.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Interface for MusicBackends
 * ---------------------------------------------------------------------------*/

#ifndef MUSICBACKEND_H_INCLUDED
#define MUSICBACKEND_H_INCLUDED

#include <vector>
#include <memory>
#include "GlobalTypes.h"
#include "Queue.h"
#include "Result.h"
#include "Track.h"
#include "PlaybackTrack.h"

class MusicBackend{
public:
    virtual TResultOpt initBackend(void) = 0;
    virtual TResult<std::vector<std::shared_ptr<Track>>> queryTracks(std::string const &pattern, size_t const num) = 0;
    virtual TResultOpt setPlayback(std::shared_ptr<Track> const track) = 0;
    virtual TResult<std::shared_ptr<PlaybackTrack>> getCurrentPlayback(void) = 0;
    virtual TResultOpt pause(void) = 0;
    virtual TResultOpt play() = 0;
    virtual TResult<size_t> getVolume(void) = 0;
    virtual TResultOpt setVolume(size_t const percent) = 0;
};

#endif //MUSICBACKEND_H_INCLUDED
