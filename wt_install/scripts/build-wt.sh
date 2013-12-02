#!/bin/bash
# TODO : how to with /usr/share/cmake-2.8/Modules/ and wt install it under $PREFIX/cmake

# on configure apache2 pour wt :
#================================
$CMDROOT cp $ROOTDEV/wt_install/etc/etc_apache2_conf.d_witty /etc/apache2/conf.d/witty

# On cree les répertoires nécessaires à witty en installant un script sous init.d car /var/run/wt est détruit a chaque boot :
#=============================================================================================================================
if [ ! -d "/var/run/wt" ] 
 then
   $CMDROOT cp $ROOTDEV/wt_install/etc/etc_init.d_wtrun /etc/init.d/wtrun
   $CMDROOT update-rc.d wtrun defaults
   $CMDROOT /etc/init.d/wtrun start
fi

[ "$cleanUpGit" = "ON" ] && [ -d "$APPNAME" ]  && rm -Rf $APPNAME/

if [ ! -d "$APPNAME" ] 
 then
  #git clone http://www.webtoolkit.eu/git/wt.git
   git clone https://github.com/kdeforche/wt.git
   res1=$?
   [ "$res1" != "0" ] && exit 1;
fi

cd wt/
git pull
res1=$?
[ "$res1" != "0" ] && exit 1;
sleep 5

# le Nom du repertoire de build
#==============================
DIRBUILD=`pwd`/"build-"`uname -m`"-"`uname -s`

[ "$cleanUpBuild" = "ON" ] && [ -d "$DIRBUILD" ] && $CMDROOT rm -Rf $DIRBUILD
[ ! -d "$DIRBUILD" ] && mkdir $DIRBUILD
cd $DIRBUILD

# If you build your own boost install add :
#         -D Boost_NO_SYSTEM_PATHS=TRUE                             \

# If you want to specify the installation path add :
#         -D CMAKE_INSTALL_PREFIX=/usr/local/wt                     \

# If you build your own gm :
#         -D GM_PREFIX=/usr/local/GraphicsMagick                    \
#         -D DEPLOYROOT=/var/www/wt                                 \
#         -D HARU_ROOT=/usr/local/lib                               \

varCmake=$(cat <<EOF
          -D CONNECTOR_FCGI=ON                                      \
          -D CONNECTOR_HTTP=ON                                      \
          -D BUILD_EXAMPLES=OFF                                     \
          -D BUILD_TESTS=OFF                                        \
          -D RUNDIR=/var/run/wt                                     \
          -D HARU_PREFIX=/usr/local/libharu                         \
          -D BOOST_PREFIX=/usr                                      \
          -D WEBUSER=www-data                                       \
          -D WEBGROUP=www-data                                      \
          -D ENABLE_EXT=OFF                                         \
          -D ENABLE_PANGO=ON                                        \
          -D ENABLE_EXT=OFF                                         \
          -D ENABLE_SQLITE=ON                                       \
          -D ENABLE_POSTGRES=OFF                                    \
          -D ENABLE_FIREBIRD=OFF                                    \
          -D ENABLE_MYSQL=OFF                                       \
          -D ENABLE_QT4=OFF                                         \
          ../../
EOF
)

#echo $varCmake

if [ "buildDebug" = "ON" ]
 then
   [ ! -d "Debug" ] && mkdir Debug
   cd Debug
   cmake -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -D CMAKE_BUILD_TYPE=Debug -D CMAKE_DEBUG_POSTFIX="d" $varCmake
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
cmake -D CMAKE_INSTALL_PREFIX=$IPREFIX/$APPNAME -D CMAKE_BUILD_TYPE=Release   $varCmake
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

# On adapte le fichier de configuration de wt pour nos besoins propres
cat $ROOTDEV/wt/wt_config.xml.in | sed -e "s#<num-threads>1</num-threads>#<num-threads>5</num-threads>#"  \
                                       -e "s#<property name=\"resourcesURL\">resources/</property>#<property name=\"resourcesURL\">/wt/resources/</property>#" \
                                       -e "s#<max-request-size>128</max-request-size>#<max-request-size>8192</max-request-size>#" \
                                       -e "s#<run-directory>\${RUNDIR}</run-directory>#<run-directory>/var/run/wt</run-directory>#" \
                                       -e "s#<timeout>600</timeout>#<timeout>600</timeout>\n <bootstrap-timeout>60</bootstrap-timeout>#" \
                                       > wittyshare.config.xml
$CMDROOT cp wittyshare.config.xml /etc/wt/wt_config.xml
#rm wittyshare.config.xml

cd ..

exit 0

