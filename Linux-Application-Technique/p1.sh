#!/bin/bash
if [ $# != 1 ]
then
	echo "Wrong Parameter! The Only Parameter MUST BE a File Name!"
	exit 1
elif [ -f "$1" ]
then
	fileinfo=(`ls -l $0`)
	echo -n "Owner:"
	echo ${fileinfo[2]}
	echo -n "Last Modified:"
	echo ${fileinfo[5]}${fileinfo[6]}"日"
	exit 0
else
	exit 1
fi
