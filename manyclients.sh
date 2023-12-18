#!/bin/bash

for i in {1..50}
do
    {
        echo "looking for bomberstudent servers"
        sleep 1  # wait for 1 second before sending the next request
        echo "Client$i"
        
        # for each client, read the server's response and print it
        # while true
        # do
        #     read -r response
        #     echo "Received: $response"
        #     sleep 1  # wait for 1 second before sending the next request
        # done

    } | nc localhost 49151 &  # allow multiple clients to run in parallel
done

wait
