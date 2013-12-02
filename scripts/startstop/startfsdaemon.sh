#!/bin/bash

(/usr/local/wittyshare/bin/fsdaemon & echo $! > /tmp/fsdaemon.pid )&
