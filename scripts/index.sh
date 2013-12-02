#!/bin/bash
if [[ ! -e "/var/www/wt/resources/wsdb.sqlite" ]]
then
     sudo /usr/local/mnogosearch/sbin/indexer -Edrop   /etc/wt/indexer.conf 2>&1
     sudo /usr/local/mnogosearch/sbin/indexer -Ecreate /etc/wt/indexer.conf 2>&1
fi
sudo /usr/local/mnogosearch/sbin/indexer          /etc/wt/indexer.conf 2>&1
sudo /usr/local/mnogosearch/sbin/indexer --index  /etc/wt/indexer.conf 2>&1
sudo chown www-data.www-data /var/www/wt/resources/wsdb.sqlite
