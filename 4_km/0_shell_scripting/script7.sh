#!/bin/sh

# comparison of two numbers; 
# command usage: ./script7.sh <first_number> <second_number>

echo "Shell Name:\$0:$0"
echo "Shell First Parameter:\$1:$1"
echo "Shell Second Parameter:\$2:$2"

if [ $# -lt 2 ] || [ $# -gt 2 ]
then
	echo "Script Usage:$0 <first_number> <second_number>"
	exit 1
fi
echo "user pass proper syntax"

if [ "$1" = "$2" ] 
then
	echo "Both Strings are EQUAL"
elif [ "$1" != "$2" ]; then
	echo "Both Strings are NOT EQUAL"
fi
exit 0


