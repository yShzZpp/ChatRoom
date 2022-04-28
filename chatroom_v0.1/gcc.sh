#!/bin/bash

gcc src/server_tcp.c -o server -I inc -lpthread
gcc src/client_tcp.c -o client -I inc -lpthread
./server
