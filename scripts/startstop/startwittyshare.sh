#!/bin/bash

echo 'Starting' >> /var/log/wittyshare.log
cd /var/www/
./wt/wittyshare/wittyshared.wt --docroot '/var/www;/web03,/eurofer,/index.html,/users,/ws,/wt' --http-port 8080 --http-address 127.0.0.1 -p /tmp/wittyshare.pid >> /var/log/wittyshare.log 2>&1 &
