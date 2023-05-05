#!/bin/bash

if [ "$1" = "M" ]; then
    ./builds/mthread_server
elif [ "$1" = "S" ]; then
    ./builds/mthread_server
elif [ "$1" = "TM" ]; then
    ./builds/mthread_server &
    sleep 1 && python tests/client.py && kill -s SIGINT %1
elif [ "$1" = "TS" ]; then
    ./builds/sthread_server &
    sleep 1 && python tests/client.py && kill -s SIGINT %1
else
    echo "Please check the readme on how to use this script"
fi
