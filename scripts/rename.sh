#!/bin/bash


#find $1 -type d -name ".config" -exec \ ls {} \;
set -x
#find $1 -type d -name ".config" -exec \ ls {} \;

function okdir {
  while read p; do
    echo "$p"
    cd "$p"
    mkdir nodes
    mv * nodes
    mv nodes/properties.json .
    cd -
  done
  

}

find $1 -type d -name ".config" | okdir 
