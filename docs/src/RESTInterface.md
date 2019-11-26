# REST interface {#rest_interface}

<!-- only show 2 levels of headings in the table of contents when generating HTML -->
@tableofcontents{HTML:2}

This page describes the available REST endpoints provided for clients and how they can be used.
Each of the following requests define a method, a path and JSON format for the request/response body which is
expected/can be expected.

## Status response

Since it cannot be guaranteed that every request can be handled as intended, there needs to be some kind of status report.\n
Requests which are handled successfully always respond with a HTTP status code of `200 OK`. Every other code indicates an error.

The known errors are:

- `403 Forbidden`\n
  The request was done by an unauthorized user. That includes requests to admin endpoints by non-admins or an attempt
  to do multiple votes on the same track.
- `404 Not found`\n
  The requested endpoint has not been found.
- `405 Method not allowed`\n
  The requested endpoint was accessed with an unallowed method.
- `422 Unprocessable Entity`\n
  The content of the request (JSON body) has an unexpected/invalid format.
- `440 Login-Time-out`\n
  The currently used session is invalid or has expired.
- `500 Internal Server Error`\n
  This code may be encountered if the server catched an internal error (which may as well just have crashed the server)
  and was still able to send a response.\n
  If a client gets that status code, please notify the server team!

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

## Available requests

This section lists all available REST endpoints and their intended usages. Additionally the needed HTTP method
as well as the path and the body layout is specified.

Request and response bodies need to be in the JSON format. Field values can be of type `int` (32-bit signed integer),
`string` (quoted with double quotes) or `boolean` (true or false).

**Note**: Invalid JSON (or missing required fields) will trigger an `422` error!

### Generating a session

Before doing other requests clients need to get a session ID. This ID is used to identify the user between multiple requests,
which is needed if the user wants to undo a vote or prevents him from voting twice on the same track.

**Note**: When an invalid/expired session ID is used in any request a `440` error is triggered!

**Note**: If the client drops the session ID on purpose the voting system can be exploited!

#### Request

- Method:   \n
  `GET`
- Path:     \n
  `/api/v1/generateSession`
- Body:     \n

~~~~~{.c}
{
}
~~~~~

#### Response

- Statuscode:   \n
  200 OK
- Body:         \n

~~~~~{.c}
{
    "session_id": "<session_id>"
}
~~~~~

### Querying tracks {#query_tracks}

Before the user can add tracks to the queue he has to query the available tracks. The available tracks are queried with
a user given pattern and are returned in an backend-specific order. To increase flexibility clients may specify the
maximum number of tracks returned.

**Note**: In the future some kind of paging may be implemented.

#### Request

- Method:   \n
  `GET`
- Path:     \n
  `/api/v1/queryTracks`
- Body:     \n

~~~~~{.c}
{
    "session_id": "<session_id>",
    "pattern": "<some_search_pattern>",
    "max_entries": <max_entries>
}
~~~~~

The field `max_entries` is optional. If not specified, it is set to `50`.

#### Response

~~~~~{.c}
{
    "tracks": [
        {
            "track_id": "<track_id>",
            "title": "<track_title>",
            "album": "<album_name>",
            "author": "<author_name>",
            "duration": "<duration>",
            "icon_uri": "<uri>"
        },
        ...
    ]
}
~~~~~

If a track has no associated `album` and/or `author` these fields may be omitted.

### Get current queues

Queries the current queues (normal and admin queue) as well as the currently playing track.

**Attention**: In the first version this endpoint won't block until a new queue update is available!

**Note**: For now this endpoint does not provide information on since when or how long a track is still playing!

#### Request

- Method:   \n
  `GET`
- Path:     \n
  `/api/v1/getCurrentQueues`
- Body:     \n

~~~~~{.c}
{
    "session_id": "<session_id>"
}
~~~~~

The field `session_id` may be omitted in this version (thus empty JSON objects are valid for now).

#### Response

~~~~~{.c}
{
    "currently_playing": {
        "track_id": "<track_id>",
        "title": "<track_title>",
        "album": "<album_name>",
        "author": "<author_name>",
        "duration": "<duration>",
        "icon_uri": "<uri>",
        "playing": true|false,
        "playing_for": <playing_for_ms>
    },
    "normal_queue": [
        {
            "track_id": "<track_id>",
            "title": "<track_title>",
            "album": "<album_name>",
            "author": "<author_name>",
            "duration": "<duration>",
            "icon_uri": "<uri>",
            "votes": <nr_of_votes>
        },
        ...
    ],
    "admin_queue": [
        {
            "track_id": "<track_id>",
            "title": "<track_title>",
            "album": "<album_name>",
            "author": "<author_name>",
            "duration": "<duration>",
            "icon_uri": "<uri>"
        },
        ...
    ],
}
~~~~~

The entries in the normal queue are sorted in the same order as they will be played (no client-side sorting needed!).\n
The track listed in `currently_playing` has an additional field for its current playback status (playing or paused)
and the time it has already been played (in milliseconds).

### Add track to queue

Adds a track to the specified queue on the server.

**Note**: The same track can only be queued once at a time (i.e. as long as a track is in any of the queues, it cannot be queued again).

#### Request

- Method:   \n
  `GET`
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
The content for `track_id` has to be queried with a previous call to [queryTracks](#query_tracks).\n
`queue_type` indicates in which queue the track should be added. Valid values are `admin` or `normal`. If omitted it is set to `normal`.

#### Response

~~~~~{.c}
{
}
~~~~~

A successful call responds with an empty JSON object.

### Vote track

Vote for a track or revoke a vote.

**Note**: A user cannot vote for the same track twice, nor can he revoke non existing votes causing a downvote.

#### Request

- Method:   \n
  `GET`
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

The content for `track_id` has to be queried with a previous call to [queryTracks](#query_tracks).\n
If `vote` is a `0` an already given vote is revoked, while setting `vote` to a non-zero value votes for the track.

#### Response

~~~~~{.c}
{
}
~~~~~

A successful call responds with an empty JSON object.

### Control player

Using this endpoint the client can cause the player behaviour to change.

**Note**: This endpoint can only be used by the admin.

**Note**: For now querying the current player state is not possible!

#### Request

- Method:   \n
  `GET`
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

- Play
- Pause
- Stop
- Skip
- VolumeUp
- VolumeDown

#### Response

~~~~~{.c}
{
}
~~~~~

A successful call responds with an empty JSON object.
