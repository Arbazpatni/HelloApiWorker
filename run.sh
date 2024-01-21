#!/bin/bash

/app/Cli "3000" & #absolute path is very important otherwise error
until /app/TCli "start" "accept" "--token" "76Pfhh/YHVMWe07SZsp6dO88Jcq27uOuAIU9K1TGsLI="
do
    echo "Worker Process Crashed with exit code $?. Respawning ..."
    sleep 1
done
