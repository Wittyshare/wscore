#!/bin/bash

source ./wt_install/include/base.inc


# On installe gdcore, gdwtcore, wittyShare :
#===========================================
for project in gdcore gdwtcore wittyshare
 do
   echo
   echo "Start $project build"
   echo
   sleep 1
   cd $ROOTDEV/
   export APPNAME=$project
   $ROOTDEV/ws_install/scripts/build-$project.sh
   res1=$?
   [ "$res1" != "0" ] && exit 1;
   echo "End $project build"
   echo
 done

exit 0
