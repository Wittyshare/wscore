export buildRelease=OFF
export buildDebug=ON
export runAptitude=OFF
export cleanUpGit=OFF
export cleanUpBuild=OFF
export cleanUpInstall=OFF

export IPREFIX=/usr/local
export ROOTDEV=/home/guy/Dev

if [ "$UID" != "0" ]
 then
   CMDROOT=`which sudo`
   if [ "$CMDROOT" = "" ]
    then
     CMDROOT="su -c"
   fi
   export CMDROOT
fi

if [ ! -d "$ROOTDEV" ]
 then
  echo "Please create or modify the ROOT DEV Directory"
  exit 1
fi

cd $ROOTDEV

