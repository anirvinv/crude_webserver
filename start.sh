#!/bin/bash

FLAG=false

if [ "$1" = "M" ]; then
    make && ./builds/mthread_server.out
elif [ "$1" = "S" ]; then
    make && ./builds/mthread_server.out
elif [ "$1" = "TM" ]; then
    make
    ./builds/mthread_server.out &
    sleep 1 && ./runtests.sh
    FLAG=true
elif [ "$1" = "TS" ]; then
    make
    ./builds/sthread_server.out &
    sleep 1 && ./runtests.sh
    FLAG=true
else
    echo "Please check the readme on how to use this script"
fi

function handleSIGINT() {
    echo $'\n'---------------------Stopping server--------------$'\n'
    sleep 1
    if [ "$1" = "TM" ] || [ "$1" = "TS" ]; then
        kill -s SIGINT %1
    fi
    exit
}

trap 'handleSIGINT' SIGINT
if [ $FLAG = true ]; then
    while true; do
        sleep 1
    done
fi
