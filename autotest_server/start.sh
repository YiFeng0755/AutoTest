#!/bin/bash

server_script_name=`pwd`"/server.py"

res=`ps -fe | grep 'server.py'| grep -v 'grep'`
#echo $res 
if [ -z "$res" ];then
    python $server_script_name&
    echo "Start success!"
else
    pids=`echo $res | awk '{print $2}'`
    
    for pid in $pids;do
        kill -9 $pid
        echo "kill process: $pid"
    done

    python $server_script_name&
    echo "Start success!"
fi


