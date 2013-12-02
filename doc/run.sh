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
#       CREATED: 03/08/2012 02:28:26 PM CET
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

/var/www/wt/wittyshare/wittyshare.wthttpd --docroot /var/www/ --http-port 8080 --http-address 0.0.0.0 --approot /var/www/wt/
