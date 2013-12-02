#!/bin/bash - 
#===============================================================================
#
#          FILE: refactor.sh
# 
#         USAGE: ./refactor.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 07/19/2012 09:19:26 AM CEST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error



function replace {
  from=$1
  to=$2
  echo "Replacing $from with $to in $3"
  sed -i -e "s/$from/$to/g" $3
}
function funcRead {
while true; do
  read -r lineA <&3
  read -r lineB <&4
  if [ -z "$lineA" -o -z "$lineB" ]; then
    break
  fi
  replace $lineA $lineB $3
done 3<$1 4<$2
}

funcRead config_from.txt config_to.txt $1
#replace "hello" "world"
