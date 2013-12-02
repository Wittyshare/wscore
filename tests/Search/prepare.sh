#!/bin/sh

# Clean
/usr/local/mnogosearch/sbin/indexer -Edrop indexer.conf 2>&1
# Create
/usr/local/mnogosearch/sbin/indexer -Ecreate indexer.conf 2>&1
# Put files
rsync -aLv directories /tmp/
# Index
/usr/local/mnogosearch/sbin/indexer indexer.conf 2>&1
/usr/local/mnogosearch/sbin/indexer --index indexer.conf 2>&1

