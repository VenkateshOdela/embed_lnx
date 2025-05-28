#!/bin/sh


echo "Time Difference in seconds"

# date +%s   seconds since 1970-01-01 00:00:00 UTC

START_TIME=`date +%s`

sleep 5

END_TIME=`date +%s`

TIME_DIFF=$((END_TIME-START_TIME))

echo "delay = ${TIME_DIFF}seconds"
