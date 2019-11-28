/** ----------------------------------------------------------------------------
 * @file    Track.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class Track definition
 * ---------------------------------------------------------------------------*/

#ifndef _TRACK_H_
#define _TRACK_H_

#include "GlobalTypes.h"
#include <ctime>

class Track {
public:
    TTrackID TrackID;
    std::string Title;
    std::string Album;
    std::string Author;
    std::time_t Duration;
    std::string iconUrl;
    bool operator==(const Track tr){
        return TrackID == tr.TrackID;
    }
};

#endif /* _TRACK_H_ */
