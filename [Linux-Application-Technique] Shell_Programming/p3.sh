#!/bin/bash
if [ $# == 1 ] && [ $1 ]
then
	str=$1
	length=${#str}
	for((i=0;i<length;i++))
	do
		str=${str/[!a-zA-Z]/}
	done
	length=${#str}
	isPalindrome=1
	for((i=0;i<length/2;i++))
	do
		if [ ${str:$i:1} != ${str:`expr $length - $i - 1`:1} ]
		then
			isPalindrome=0
			break
		fi
	done
	if [ $isPalindrome = 0 ]
	then
		echo "Not a Palindrome!"
	else
		echo "Is a Palindrome!"
	fi
	exit 0
else
	echo "Wrong Parameter! The Only Parameter MUST BE a NON-EMPTY String!"
	exit 1
fi
