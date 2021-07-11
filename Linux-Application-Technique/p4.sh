#!/bin/bash
minsize=`expr 100 \* 1024`
for files in ./*
do
	if [ -f $files ]
	then
		fileinfo=(`ls -l $files`)
		filesize=${fileinfo[4]}
		if [ $filesize -gt $minsize ]
		then
			ls ~/tmp >/dev/null  2>&1
			if [ $? != 0 ]
			then
				mkdir ~/tmp
			fi
			mv $files ~/tmp/${files:2:${#files}}
			echo File:$files Size:$filesize has been moved to ~/tmp
	
		fi
	fi	
done
