/*****************************************************************************/
/**
 * @file    DummyData.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Some dummy data for the stub REST endpoints
 */
/*****************************************************************************/

#ifndef _DUMMY_DATA_H_
#define _DUMMY_DATA_H_

#include "Types/Queue.h"

static const Queue NORMAL_QUEUE{{{"id1",
                                  "title1",
                                  "album1",
                                  "artist1",
                                  123,
                                  "uri1",
                                  "me",
                                  1234,
                                  true,
                                  555555},
                                 {"id2",
                                  "title2",
                                  "album2",
                                  "artist2",
                                  223,
                                  "uri2",
                                  "me",
                                  1234,
                                  true,
                                  555555},
                                 {"id3",
                                  "title3",
                                  "album3",
                                  "artist3",
                                  323,
                                  "uri3",
                                  "me",
                                  1234,
                                  true,
                                  555555},
                                 {"id4",
                                  "title4",
                                  "album4",
                                  "artist4",
                                  423,
                                  "uri4",
                                  "me",
                                  1234,
                                  true,
                                  555555}}};

static const Queue ADMIN_QUEUE{{{"adminid1",
                                 "admintitle1",
                                 "album1",
                                 "artist1",
                                 456,
                                 "adminuri1",
                                 "me",
                                 1234,
                                 true,
                                 555555},
                                {"adminid2",
                                 "admintitle2",
                                 "album2",
                                 "artist2",
                                 336,
                                 "adminuri2",
                                 "me",
                                 1234,
                                 true,
                                 555555},
                                {"adminid3",
                                 "admintitle3",
                                 "album3",
                                 "artist3",
                                 8888,
                                 "adminuri3",
                                 "me",
                                 1234,
                                 true,
                                 555555},
                                {"adminid4",
                                 "admintitle4",
                                 "album4",
                                 "artist4",
                                 0,
                                 "adminuri4",
                                 "me",
                                 1234,
                                 true,
                                 555555}}};

static const PlaybackTrack CURRENT_TRACK{"adminid1",
                                         "admintitle1",
                                         "album1",
                                         "artist1",
                                         456,
                                         "adminuri1",
                                         "me",
                                         10,
                                         false};

static const std::vector<BaseTrack> TRACK_LIST = {
    {"id1", "title1", "album1", "artist1", 123, "uri1", "me"},
    {"id2", "title2", "album2", "artist2", 223, "uri2", "me"},
    {"id3", "title3", "album3", "artist3", 323, "uri3", "me"},
    {"id4", "title4", "album4", "artist4", 423, "uri4", "me"}};

#endif /* _DUMMY_DATA_H_ */
