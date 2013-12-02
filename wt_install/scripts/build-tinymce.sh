#!/bin/bash

# ce script doit etre lance depuis root ou sudo

WTRESDIR=/usr/local/wt/share/Wt/resources

if [ ! -d "$ROOTDEV/tiny_mce" ]
 then
  mkdir "$ROOTDEV/tiny_mce"
  res1=$?
  [ "$res1" != "0" ] && exit 1;
fi

cd $ROOTDEV/tiny_mce

if [ ! -f "tinymce_3.5.8.zip" ]
 then
  wget http://cloud.github.com/downloads/tinymce/tinymce/tinymce_3.5.8.zip
  res1=$?
  [ "$res1" != "0" ] && exit 1;
fi


if [ -d "$WTRESDIR/tiny_mce" ]
 then
  $CMDROOT rm -Rf $WTRESDIR/tiny_mce
  res1=$?
  [ "$res1" != "0" ] && exit 1;
fi

unzip tinymce_3.5.8.zip
res1=$?
[ "$res1" != "0" ] && exit 1;

$CMDROOT rsync -av tinymce/jscripts/tiny_mce $WTRESDIR/
res1=$?
[ "$res1" != "0" ] && exit 1;

#$CMDROOT chown -R www-data.www-data /var/www/wt/resources/tiny_mce
#res1=$?
#[ "$res1" != "0" ] && exit 1;

rm -Rf ./tinymce
res1=$?
[ "$res1" != "0" ] && exit 1;

