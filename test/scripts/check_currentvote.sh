#!/usr/bin/env bash

set -euo pipefail

IP=localhost
PORT=8888
PASSWORD=admin
NICKNAME=admin

# generate a session ID
SESSION_ID=$(curl -sS -XPOST -d '{"nickname": "'$NICKNAME'", "password": "'$PASSWORD'"}' http://$IP:$PORT/api/v1/generateSession | jq .session_id -r)
echo "Session ID: $SESSION_ID"


# delete all entries of the normal queue
SUCCESS=true
RET=$(curl -sS -XGET http://$IP:$PORT/api/v1/getCurrentQueues?session_id=$SESSION_ID)
for tid in $(echo $RET | jq -r '.normal_queue[].track_id'); do
    RET=$(curl -sS -XDELETE -d '{"session_id": "'$SESSION_ID'", "track_id": "'$tid'"}' http://$IP:$PORT/api/v1/removeTrack)
    if [ "$(echo $RET | jq '. | length')" != 0 ]; then
        echo $(echo $RET | jq -r '.error');
        SUCCESS=false
    fi
done

if [ "$SUCCESS" != "true" ]; then
    echo "Failed to clear normal queue"
    exit -1
else
    echo "Cleared normal queue"
fi


# add some tracks to the user queue
TRACKS=(spotify:track:6oomEes8hP95cWb8S898xF spotify:track:0aZIlnIkxIO8siYRV5SIkN spotify:track:2ZCCkqminNZiitXgbFArXM)

SUCCESS=true
for t in ${TRACKS[@]}; do
    RET=$(curl -sS -XPOST -d '{"track_id": "'$t'", "session_id": "'$SESSION_ID'", "queue_type": "normal"}' http://$IP:$PORT/api/v1/addTrackToQueue)
    if [ "$(echo $RET | jq '. | length')" != 0 ]; then
        # echo $(echo $RET | jq -r '.error');
        SUCCESS=false;
    fi
done

if [ "$SUCCESS" != "true" ]; then
    echo "Failed to add some tracks"
    exit -1
else
    echo "Added some tracks"
fi


# check the current queues
RET=$(curl -sS -XGET http://$IP:$PORT/api/v1/getCurrentQueues?session_id=$SESSION_ID)
# echo $RET | jq .
COUNT=$(echo $RET | jq '.normal_queue | length')

SUCCESS=true
echo
for ((i=0;i<COUNT;i++)); do
    ENTRY=$(echo $RET | jq '.normal_queue['$i']')
    TITLE=$(echo $ENTRY | jq -r '.title')
    NR_OF_VOTES=$(echo $ENTRY | jq '.votes')
    CURRENT_VOTE=$(echo $ENTRY | jq '.current_vote')

    echo "Title        : $TITLE"
    # how many votes did this track receive in total?
    echo "Total votes  : $NR_OF_VOTES"
    # did the current user vote for this track?
    echo "Current vote : $CURRENT_VOTE"
    echo

    if [ "$NR_OF_VOTES" != "0" ] || [ "$CURRENT_VOTE" != "0" ]; then
        # echo "Unexpected vote counts"
        SUCCESS=false
    fi
done

if [ "$SUCCESS" != "true" ]; then
    echo "Check for current vote counts failed"
    exit -1
else
    echo "Vote counts for normal queue are legit"
fi
