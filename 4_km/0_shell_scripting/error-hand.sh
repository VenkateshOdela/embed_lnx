#!/bin/sh

# This shell example shows that how to use Proper ERROR handling in shell

#echo kernel > 123

ping -c 1 www.google > /dev/null 2> /dev/null

#echo "\$?:$?"

if [ $? -eq 0 ]
then
	echo "Internet is Working"
else
	echo "Internet is not Working"
	exit 4
fi
echo "program starts here"

exit 0

#ls xyz
#echo "ls xyz command status: $?"


#echo kernel > 123
#echo "ls 123 command status: $?"
