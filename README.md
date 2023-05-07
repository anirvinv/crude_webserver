# Simple webserver
## Before running
1. Clone/fork the repo
2. You might need to run `chmod +x start.sh`
## Building and Running Instructions
1. To build, run `make`
2. To run the singlethreaded server, `./start.sh S`
3. To run the multithreaded server, `./start.sh M`
4. To erase builds, run `make clean`
## Tests
- Run `./start.sh TS` to run tests for the single threaded server and `./start.sh TM` to test the multithreaded server
## To do
1. Containerize server (`clone()`?)
2. Set up multiple intances of servers and make a load balancer to simulate a distributed system locally. (Is this even possible?)
## Features
1. Webserver serves static files inside the `static/` directory
   -   Can serve html, css, js files (MIME types)
2. `mthread_server.c` supports conccurrent connections --> used pthread library to create worker threads to service each connection