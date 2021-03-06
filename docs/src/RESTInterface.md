# REST interface {#rest_interface}

<!-- only show 2 levels of headings in the table of contents when generating HTML -->
@tableofcontents{HTML:2}

This page describes the available REST endpoints provided for clients and how they can be used.
Each of the following requests define a method, a path and JSON format for the request/response body which is
expected/can be expected.

# Status response {#status_responses}

Since it cannot be guaranteed that every request can be handled as intended, there needs to be some kind of status report.\n
Requests which are handled successfully always respond with a HTTP status code of `200 OK`. Every other code indicates an error.

The known errors are:

- `401 Unauthorized`\n
  If a client requested a new admin session but specified a wrong password.
- `403 Forbidden`\n
  The request was done by an unauthorized user. That includes requests to admin endpoints by non-admins or an attempt
  to do multiple votes on the same track. Additionally this error may be returned if the MusicBackend has an permission problem.
- `404 Not found`\n
  The requested endpoint has not been found.\n
  For now, unknown `track_id`s also trigger this error.
- `422 Unprocessable Entity`\n
  The content of the request (JSON body) has an unexpected/invalid format.
- `440 Login-Time-out`\n
  The currently used session is invalid or has expired.
- `500 Internal Server Error`\n
  This code may be encountered if the server catched an internal error (which may as well just have crashed the server)
  and was still able to send a response.\n
  If a client gets that status code, please notify the server team!
- `502 Bad Gateway`\n
  If a third party service responds with any unexpected error this error code is returned.

**Note**: More errors may be added in the future!

In case of an error (i.e. HTTP status code is not `200`) the response body looks like this:

~~~~~{.c}
{
    "status": <status_code>,
    "error": "<error_message>"
}
~~~~~

`status_code` will be the repeated HTTP status code (403 for example).\n
`error` contains a short description which can be used to display an error message to the end user.

# Available requests {#available_requests}

This section lists all available REST endpoints and their intended usages. Additionally the needed HTTP method
as well as the path and the body layout is specified.

Requests using the `GET` method should pass their parameters in the [query string](https://en.wikipedia.org/wiki/Query_string)
instead of the body.

The bodies of other methods and all responses are expected/can be expected in the JSON format. Field values can be of type
`int` (32-bit signed integer), `string` (quoted with double quotes) or `boolean` (true or false).

**Note**: Invalid JSON (or missing required fields) will trigger an `422` error!

## Generating a session {#generate_session}

Before doing other requests clients need to get a session ID. This ID is used to identify the user between multiple requests,
which is needed if the user wants to undo a vote or prevents him from voting twice on the same track.

**Note**: When an invalid/expired session ID is used in any request a `440` error is triggered!

**Note**: If the client drops the session ID on purpose the voting system can be exploited!

### Request

- Method:   \n
  `POST`
- Path:     \n
  `/api/v1/generateSession`
- Body:     \n

~~~~~{.c}
{
    "password": "<admin_password>",
    "nickname": "<nickname>"
}
~~~~~

`password` is used to authorize a client as admin. If no password is given a normal (non-privileged) user session
is generated. If a wrong password is given an `401` error is returned.
The `nickname` can (optionally) be set to have a readable name associated with a session. This name is also returned for
each track when using [getCurrentQueues](#get_current_queues).

**Attention**: The `nickname` is explicitly NOT guaranteed to be unique, so do not use it anywhere to uniquely identify users!

**TODO**: Allow refreshing an old session.

### Response

- Statuscode:   \n
  200 OK
- Body:         \n

~~~~~{.c}
{
    "session_id": "<session_id>"
}
~~~~~

## Querying tracks {#query_tracks}

Before the user can add tracks to the queue he has to query the available tracks. The available tracks are queried with
a user given pattern and are returned in an backend-specific order. To increase flexibility clients may specify the
maximum number of tracks returned.

**Note**: In the future some kind of paging may be implemented.

### Request

- Method:   \n
  `GET`
- Path:     \n
  `/api/v1/queryTracks`
- Parameters:
  - `pattern`: A search pattern for filtering (and sorting) the tracks.
  - `max_entries`: Specifies the maximum number of returned tracks. Optional, defaults to `50`.

### Response

~~~~~{.c}
{
    "tracks": [
        {
            "track_id": "<track_id>",
            "title": "<track_title>",
            "album": "<album_name>",
            "artist": "<artist_name>",
            "duration": "<duration>",
            "icon_uri": "<uri>"
        },
        ...
    ]
}
~~~~~

If a track has no associated `album` and/or `artist` these fields may be omitted.

## Get current queues {#get_current_queues}

Queries the current queues (normal and admin queue) as well as the currently playing track.

**Attention**: In the first version this endpoint won't block until a new queue update is available!

**Note**: For now this endpoint does not provide information on since when or how long a track is still playing!

### Request

- Method:   \n
  `GET`
- Path:     \n
  `/api/v1/getCurrentQueues`
- Parameters:
  - `session_id`: The generated session ID for the user.

The parameter `session_id` may be omitted in this version.

### Response

~~~~~{.c}
{
    "currently_playing": {
        "track_id": "<track_id>",
        "title": "<track_title>",
        "album": "<album_name>",
        "artist": "<artist_name>",
        "duration": <duration>,
        "icon_uri": "<uri>",
        "added_by": "<nickname>",
        "playing": true|false,
        "playing_for": <playing_for_ms>
    },
    "normal_queue": [
        {
            "track_id": "<track_id>",
            "title": "<track_title>",
            "album": "<album_name>",
            "artist": "<artist_name>",
            "duration": <duration>,
            "icon_uri": "<uri>",
            "added_by": "<nickname>",
            "votes": <nr_of_votes>,
            "current_vote": <vote>
        },
        ...
    ],
    "admin_queue": [
        {
            "track_id": "<track_id>",
            "title": "<track_title>",
            "album": "<album_name>",
            "artist": "<artist_name>",
            "duration": <duration>,
            "icon_uri": "<uri>",
            "added_by": "<nickname>"
        },
        ...
    ],
}
~~~~~

The entries in the normal queue are sorted in the same order as they will be played (no client-side sorting needed!). The field
`current_vote` indicates if the user has already voted for a track (in the normal queue). For now this can either be `1` or `0`.\n
The track listed in `currently_playing` has an additional field for its current playback status (playing or paused)
and the time it has already been played (in milliseconds).
The nickname of the user who added a specific track can be found in the `added_by`.

**Note**: The fields `votes` and `current_vote` are only relevant for tracks in the normal queue. While the order of tracks
in the normal queue depends on the vote count (and insertion date) the admin queue is ordered only using the insertion date.
The currently playing track also does not contain the vote fields because he will not be requeued afterwards anyway.

## Add track to queue {#add_track}

Adds a track to the specified queue on the server.

**Note**: The same track can only be queued once at a time (i.e. as long as a track is returned by [getCurrentQueues](#get_current_queues),
it cannot be queued again).

### Request

- Method:   \n
  `POST`
- Path:     \n
  `/api/v1/addTrackToQueue`
- Body:     \n

~~~~~{.c}
{
    "session_id": "<session_id>",
    "track_id": "<track_id>",
    "queue_type": "<queue_type>"
}
~~~~~

`session_id` is used to authorize the admin when adding a track to the admin queue.\n
The content for `track_id` has to be queried with a call to [queryTracks](#query_tracks).\n
`queue_type` indicates in which queue the track should be added. Valid values are `admin` or `normal`. If omitted it is set to `normal`.

### Response

~~~~~{.c}
{
}
~~~~~

A successful call responds with an empty JSON object.

## Vote track {#vote_track}

Vote for a track or revoke a vote.

**Note**: A user cannot vote for the same track twice, nor can he revoke non existing votes causing a downvote.

### Request

- Method:   \n
  `PUT`
- Path:     \n
  `/api/v1/voteTrack`
- Body:     \n

~~~~~{.c}
{
    "session_id": "<session_id>",
    "track_id": "<track_id>",
    "vote": <vote>
}
~~~~~

The content for `track_id` has to be queried with a call to [queryTracks](#query_tracks).\n
If `vote` is a `0` an already given vote is revoked, while setting `vote` to a non-zero value votes for the track.

**Note**: The actual value of non-zero values are not important (it will always count as `1`)!

### Response

~~~~~{.c}
{
}
~~~~~

A successful call responds with an empty JSON object.

## Control player {#control_player}

Using this endpoint the client can cause the player behaviour to change.

**Note**: This endpoint can only be used by the admin.

**Note**: For now querying the current player state is not possible!

### Request

- Method:   \n
  `PUT`
- Path:     \n
  `/api/v1/controlPlayer`
- Body:     \n

~~~~~{.c}
{
    "session_id": "<session_id>",
    "player_action": "<player_action>"
}
~~~~~

The value of `player_action` controls which action the server should take. Valid values are:

- `"play"`: Starts or resumes playback.
- `"pause"`: Pauses the playback. If it is already paused or stopped nothing happens.
- `"skip"`: Removes the currently playing (or paused) track and play the next one in the queue. If playback is stopped
  remove the next queued track without resuming playback.
- `"volume_up"`: Raises the volume by a fixed amount if it is not already at its maximum value.
- `"volume_down"`: Lowers the volume by a fixed amount if it is not already at its minimum value.

**Note**: The current volume level cannot be queried by now.

### Response

~~~~~{.c}
{
}
~~~~~

A successful call responds with an empty JSON object.

## Move tracks between queues {#move_track}

Moving a track between the admin and the normal queue.

**Note**: A move is equivalent to a call to [removeTrack](#remove_track) followed by [addTrack](#add_track) meaning when
moving a track all its fields get reset (i.e. the vote count is set to zero, the nickname is set to the
nickname of the admin etc.).

**Note**: This endpoint can only be used by the admin.

### Request

- Method:   \n
  `PUT`
- Path:     \n
  `/api/v1/moveTrack`
- Body:     \n

~~~~~{.c}
{
    "session_id": "<session_id>",
    "track_id": "<track_id>",
    "queue_type": "<queue_type>"
}
~~~~~

`track_id` specifies the track to be moved. This ID can be received by [getCurrentQueues](#get_current_queues).\n
`queue_type` may either be `admin` or `normal` depending whether the track should be moved to the admin queue or the
normal queue. When `queue_type` specifies the queue the track is already in, nothing happens (no votes are lost).

### Response

~~~~~{.c}
{
}
~~~~~

A successful call (the track was moved or was already in the right queue) responds with an empty JSON object.

## Remove tracks from queues {#remove_track}

Removes a track from either the admin or the normal queue.

**Note**: This endpoint can only be used by the admin.

### Request

- Method:   \n
  `DELETE`
- Path:     \n
  `/api/v1/removeTrack`
- Body:     \n

~~~~~{.c}
{
    "session_id": "<session_id>",
    "track_id": "<track_id>"
}
~~~~~

`track_id` specifies the track to be removed. Since one track can only be in one queue at a time no ambiguities are possible!
This ID can be received by [getCurrentQueues](#get_current_queues).

### Response

~~~~~{.c}
{
}
~~~~~

A successful call responds with an empty JSON object.
