#!/bin/bash

set -x 

/bin/kill -9 `cat /tmp/wittyshare.pid`
echo "" > /tmp/wittyshare.pid
