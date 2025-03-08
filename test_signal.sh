#!/bin/bash

# gener child process
sleep 10 &
pid=$!

# send signal SIGTERM (signal 15) to child process
kill -15 $pid

# wait child process terminal and take exit code
wait $pid
exit $?

