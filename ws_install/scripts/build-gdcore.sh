#!/bin/bash

# gdcore a besoin de libldap-dev et postgres-server-dev
#======================================================
if [ "$runAptitude" = "ON" ]
 then
  $CMDROOT aptitude install libldap2-dev libldap-2.4-2 libldap-2.4-2-dbg
  $CMDROOT aptitude install postgresql postgresql-client postgresql-contrib postgresql-server-dev-9.1
fi

[ "$cleanUpGit" = ON ] && [ -d "$APPNAME" ] && rm -Rf $APPNAME/

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

varCmake="-D HARU_PREFIX=/usr/local/libharu ../../"

if [ "$buildDebug" = "ON" ]
 then
   [ ! -d "Debug" ] && mkdir Debug
   cd Debug
   # TODO : modify install_libs and add CMAKE_DEBUG_POSTFIX
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
cmake -D APPNAME=$APPNAME -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -D BUILD_LIB_STATIC=false -D BUILD_LIB_SHARED=true -D CMAKE_BUILD_TYPE=Release $varCmake
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

