#!/bin/bash - 
#===============================================================================
#
#          FILE: run.sh
# 
#         USAGE: ./run.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 10/30/2013 17:03
#      REVISION:  ---
#===============================================================================


for i in $(seq $1); do
  /usr/bin/casperjs casperTest.js&
  echo $! >> pid.pid
done
read




