#!/bin/bash
action=$1;
LOGFILE=/tmp/wittysharetest.log
base_dir=$PWD
#tests_dirs=$(find tests -name tests | grep -v .git )
tests_dirs=$(ls tests | grep -v testdir | grep -v latestNewsTest)
export LD_LIBRARY_PATH=/usr/local/mnogosearch/lib
echo $tests_dirs
for d in $tests_dirs ; do 
    d="tests/"$d
    echo $d
	echo "-----------------------------"
	case $action in
	run)
		if [[ ! -d $d/build ]] ; then
			mkdir $d/build;
		fi
		cd $d
		if [[ -x prepare.sh ]] ; then
			echo "running prepare with output sent to $LOGFILE"
			./prepare.sh >> $LOGFILE;
		fi

		cd build

		if ! out=$(cmake -DCMAKE_BUILD_TYPE:STRING=Debug .. 2>&1) ; then
			echo $out
			exit 1;
		fi
		if ! out=$(make) ; then
			echo $out
			exit 2;
		fi
		executable=$(find . -maxdepth 1 -executable -type f)
		echo -n will execute $executable
		if ! out=$(./$executable) ; then
			echo -e "\032[31m   FAIL! \033[m"
			echo $out
			exit 3;
		else
			echo -e "\033[32m   SUCCESS! \033[m"
		fi
		echo
		cd $base_dir
		;;
	clean)
		rm -rf $d/build;
		;;
	*)
		echo "arguments recornised: run and clean"
		exit 99
	esac
done
