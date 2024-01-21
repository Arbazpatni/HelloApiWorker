#!/bin/bash
#runslp.sh
function worker() {
    until /app/TCli "start" "accept" "--token" "76Pfhh/YHVMWe07SZsp6dO88Jcq27uOuAIU9K1TGsLI="
    do
        echo "Worker Process Crashed with exit code $?. Respawning ..."
        sleep 1
    done
}
function thread() {
    until /app/Clis "3000" "5" #absolute path is very important otherwise error
    do
        echo "Sleep Thread Process Crasher with exit code $?, Respawning ..."
        sleep 1
    done
}

worker & # start worker as background process
thread   # start thread worker later
