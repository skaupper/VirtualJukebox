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
- `422 Unprocessable Entity`\n
  The content of the request (JSON body) has an unexpected/invalid format.
- `440 Login-Time-out`\n
  The currently used session is invalid or expired.
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

Request and response bodies need to be in the JSON format. Field values can be of type `int` (32-bit signed integer) or
`string` (quoted with double quotes).

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

### Querying tracks

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
    "max_entries", <max_entries>
}
~~~~~

The field `max_entries` is optional. If not specified, it is set to `50`.

#### Response

~~~~~{.c}
{
    "nr_of_tracks": <nr_of_tracks>,
    "tracks": [
        {
            "title": "<track_title>",
            "album": "<album_name>",
            "author": "<author_name>",
            "duration": "<duration>",
            "track_id": "<track_id>"
        },
        ...
    ]
}
~~~~~

If a track has no associated `album` and/or `author` these fields may be omitted.

The field `nr_of_tracks` always indicates the number of entries in `tracks`.
