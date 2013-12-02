#!/bin/bash

srcDir="/home/guy/Docs/Dev/wt/src/WsModulesEurofer/resources/wittyshare/web03/News&Media/Media Gallery/Image Gallery/ws.res/Steel Applications/images/"

cd "$srcDir"
ls -l | grep -v "^total" | wc -l

for file in *
 do
#  newFile=`basename "$file" ".jpg|.jpeg|.JPG|.JPEG"`
  newFile=`basename "$file" ".jpg"`
  echo "link=$newFile.nolink"       >> "$newFile.url"
  echo "title=$newFile"             >> "$newFile.url"
  echo "shortDescription="          >> "$newFile.url"
 done
