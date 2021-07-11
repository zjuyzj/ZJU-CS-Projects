#!/bin/bash
if [ $# == 1 ] && [ -d "$1" ]
then
	nfile=`ls -l "$1"|grep "^-"|wc -l`
	echo Normal File:$nfile
	ndir=`ls -l "$1"|grep "^d"|wc -l`
	echo Directory:$ndir
	nexec=0
	tsize=0
	for files in $(ls "$1")
	do
		if [ -f $1"/"$files ] && [ -x $1"/"$files ]
		then
			nexec=`expr $nexec + 1`
		fi
		if [ -f $1"/"$files ]
		then
			tmpinfo=(`ls -l $1"/"$files`)
			tsize=`expr $tsize + ${tmpinfo[4]}`
		fi
	done
	echo Executable Normal File:$nexec
	echo Total Size of Normal File:$tsize
	exit 0
else
	echo "Wrong Parameter! The Only Parameter MUST BE a Directory!"
	exit 1
fi
