#!/bin/bash

for i in {1..50}
do
    {
        echo "looking for bomberstudent servers"
        sleep 1
        echo "Client$i"

    } | nc localhost 49151 &  # allow multiple clients to run in parallel
done

wait
