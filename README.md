# Simple webserver

## Before running
1. You might need to run `chmod +x start.sh` and `chmod +x runtests.sh`

## Building and Running Instructions
1. To run the singlethreaded server, `./start.sh S`
2. To run the multithreaded server, `./start.sh M`
3. To build, run `make`
4. To erase builds, run `make clean`
## Tests
1. Run `./start.sh TS` for testing the single threaded server and `./start.sh TM` to test the multithreaded server

## To do
1. Containerize using `clone()`

## Features
1. Serves static files inside the `static/` directory
2. `mthread_server.c` supports conccurrent connections --> used pthread library to create worker threads to service each connection

## Problems
1. Sometimes it crashes, might be related to TIME_AWAIT and reusing ports
