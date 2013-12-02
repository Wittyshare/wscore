#!/bin/bash
# build and start daemon if needed
if grep daemon /etc/wt/wittyshare-global.json | grep true > /dev/null ; then
 fsroot=$(grep root_path /etc/wt/wittyshare-global.json | cut -d" "  -f 3  | tr -d '",')
 cd ../../src/fsdaemon/build && cmake .. 2>&1
 ./fsdaemon --root $fsroot & 
 echo "daemon pid = $!"
# start a subshell, sleep 15 seconds and then kill hte daemon that was started
 ( sleep 15 && kill $!)&  
fi 
