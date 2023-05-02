# Simple webserver
## Building and Running Tutorial
You might or might not have to do the second step
1. Clone repo, `cd` into it
2. `chmod +x ./run.sh`
3. Execute `./run.sh` to build and run the server

## To do
1. Containerize using `clone()`

## Features
1. Serves static files
2. Supports conccurrent connections --> used pthread library to create worker threads to service each connection

## Problems
1. Sometimes it crashes, but idk why
