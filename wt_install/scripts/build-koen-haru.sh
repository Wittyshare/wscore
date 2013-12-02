#!/bin/bash

#$CMDROOT rm -Rf  /usr/local/include/hpdf
#$CMDROOT rm -Rf  /usr/local/lib/libhpdf*

[ "$cleanUpGit" = "ON" ] && [ -d "$APPNAME" ]  && rm -Rf $APPNAME/

[ ! -d "$APPNAME" ] && git clone git://github.com/kdeforche/libharu.git
cd libharu

git pull

# temporaire, Koen fait un push d'un patch que je lui ai envoyé
#==============================================================
# 2013-01-25 22h, récupération de ce patch, je commente les lignes devenues obsolètes mais je les gardent pour mémoire
#cp CMakeLists.txt CMakeLists.txt.orig
#cp ../CMakeLists.txt.guy ./CMakeLists.txt
#echo "diff CMakeLists.txt"
#diff ../CMakeLists.txt.guy CMakeLists.txt
#sleep 3
#sed -i -e "s#install(FILES \${haru_HDRS} DESTINATION include)#install(FILES \${haru_HDRS} DESTINATION \${CMAKE_INSTALL_INCLUDEDIR})#" CMakeLists.txt
#sed -i -e "s#install(FILES README CHANGES INSTALL DESTINATION .)#install(FILES README CHANGES INSTALL DESTINATION \${CMAKE_INSTALL_INCLUDEDIR})#" CMakeLists.txt
#sed -i -e "s#install(DIRECTORY doc DESTINATION .)#install(DIRECTORY doc DESTINATION \${CMAKE_INSTALL_INCLUDEDIR})#" CMakeLists.txt
#sed -i -e "s#install(DIRECTORY if DESTINATION .)#install(DIRECTORY if DESTINATION \${CMAKE_INSTALL_INCLUDEDIR})#" CMakeLists.txt


# le Nom du repertoire de build
#==============================
DIRBUILD=`pwd`/"build-"`uname -m`"-"`uname -s`

[ "$cleanUpBuild" = "ON" ] && [ -d "$DIRBUILD" ] && $CMDROOT rm -Rf $DIRBUILD
[ ! -d "$DIRBUILD" ] && mkdir $DIRBUILD
cd $DIRBUILD

# TODO : Pq doit on ajouter : -DCMAKE_INSTALL_INCLUDEDIR ???
# ==========================================================
#haruVar=" -D LIBHARU_SHARED=ON -D LIBHARU_STATIC=ON -D LIBHARU_EXAMPLES=OFF -D DEVPAK=OFF  -D CMAKE_INSTALL_INCLUDEDIR=$IPREFIX/$APPNAME/include/hpdf ../../"
haruVar=" -D LIBHARU_SHARED=ON -D LIBHARU_STATIC=ON -D LIBHARU_EXAMPLES=OFF -D DEVPAK=OFF  -D CMAKE_INSTALL_INCLUDEDIR=$IPREFIX/$APPNAME/include ../../"

if [ "buildDebug" = "ON" ]
 then
   [ ! -d "Debug" ] && mkdir Debug
   cd Debug
   #cmake -D APPNAME=$APPNAME -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -D BUILD_LIB_STATIC=true -D BUILD_LIB_SHARED=true -DCMAKE_BUILD_TYPE=Debug $haruVar
   cmake -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -DCMAKE_BUILD_TYPE=Debug $haruVar
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
#cmake -D APPNAME=$APPNAME -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -D BUILD_LIB_STATIC=true -D BUILD_LIB_SHARED=true -D CMAKE_BUILD_TYPE=Release $haruVar
cmake -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -D CMAKE_BUILD_TYPE=Release $haruVar
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

