# Simple webserver
## Building and Running Instructions
You might or might not have to do the second step
1. Clone repo, `cd` into it
2. Run `make` and then `./sthread_server.out` to start the single threaded server or `./mthread_server.out` to start the multithreaded server
3. `make clean` to remove executables

## Tests
1. Run `chmod +x ./runtests.sh`
2. Start either of the servers, and then in a seperate terminal window, run `./runtests.sh`
## To do
1. Containerize using `clone()`

## Features
1. Serves static files including web pages with js and css
2. Supports conccurrent connections --> used pthread library to create worker threads to service each connection

## Problems
1. Sometimes it crashes, might be related to TIME_AWAIT and reusing ports
