#!/bin/bash

# gdwtcore a besoin d'un lien vers le repertoire des resources de wt
#===================================================================
WhereIsWt=/usr/local/wt
if [ ! -d "/var/www/wt" ]
 then
  $CMDROOT mkdir /var/www/wt/
  res1=$?
  [ "$res1" != "0" ] && exit 1;
fi

if [ ! -h "/var/www/wt/resources" ]
 then
  $CMDROOT ln -s "$WhereIsWt/share/Wt/resources/" /var/www/wt/resources
  res1=$?
  [ "$res1" != "0" ] && exit 1;
fi



[ "$cleanUpGit" = ON ] && [ -d "$APPNAME" ]  && rm -Rf $APPNAME/

if [ ! -d "$APPNAME" ] 
 then
   git clone git@vsGitInt.eurofer.be:$APPNAME
   res1=$?
   [ "$res1" != "0" ] && exit 1;
fi
cd $APPNAME/

git pull
res1=$?
[ "$res1" != "0" ] && exit 1;

# le Nom du repertoire de build
#==============================
DIRBUILD=`pwd`/"build-"`uname -m`"-"`uname -s`

[ "$cleanUpBuild" = "ON" ] && [ -d "$DIRBUILD" ] && $CMDROOT rm -Rf $DIRBUILD
[ ! -d "$DIRBUILD" ] && mkdir $DIRBUILD
cd $DIRBUILD

varCmake="-D GDWT_PREFIX=$WhereIsWt -D HARU_PREFIX=/usr/local/libharu -D GDCORE_PREFIX=/usr/local/gdcore ../../"
#varCmake="-D GDWT_PREFIX=$WhereIsWt -D GDCORE_PREFIX=/usr/local/gdcore ../../"

if [ "$buildDebug" = "ON" ]
 then
   [ ! -d "Debug" ] && mkdir Debug
   cd Debug
   cmake -D APPNAME=$APPNAME -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -D BUILD_LIB_STATIC=false -D BUILD_LIB_SHARED=true -D CMAKE_BUILD_TYPE=Debug $varCmake
   res1=$?
   [ "$res1" != "0" ] && exit 1;
   make
   res1=$?
   [ "$res1" != "0" ] && exit 1;
   $CMDROOT make install
   res1=$?
   [ "$res1" != "0" ] && exit 1;
   
   cd ../
fi

[ ! -d "Release" ] && mkdir Release
cd Release
cmake -D APPNAME=$APPNAME -D CMAKE_INSTALL_PREFIX:PATH=$IPREFIX/$APPNAME -D BUILD_LIB_STATIC=false -D BUILD_LIB_SHARED=true -D CMAKE_BUILD_TYPE=Release $varCmake
res1=$?
[ "$res1" != "0" ] && exit 1;
make
res1=$?
[ "$res1" != "0" ] && exit 1;
$CMDROOT make install
res1=$?
[ "$res1" != "0" ] && exit 1;

echo "$IPREFIX/$APPNAME"/lib | $CMDROOT tee /etc/ld.so.conf.d/$APPNAME.conf
$CMDROOT ldconfig

exit 0

