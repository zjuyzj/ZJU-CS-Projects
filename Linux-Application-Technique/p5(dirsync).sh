#!/bin/bash
if [ $# == 2 ]
then
	cd $1
	path1=`pwd`
	cd $2
	path2=`pwd`
	find $1 -print|cut -b `expr ${#path1} + 1`- >/tmp/dir1.log
	find $2 -print|cut -b `expr ${#path2} + 1`- >/tmp/dir2.log
	chmod 700 /tmp/dir1.log /tmp/dir2.log
	for i in $(comm -23 /tmp/dir2.log /tmp/dir1.log)
	do
		rm -rf "$2$i"
	done
	for i in $(comm -23 /tmp/dir1.log /tmp/dir2.log)
	do
		cp -r "$1$i" "$2$i"
	done
	rm /tmp/dir1.log /tmp/dir2.log
else
	echo "Wrong Parameters! Usage:dirsync dir1 dir2"
	exit 1
fi
