#!/bin/sh


until who | grep "$1" > /dev/null 
do
	sleep 10
done
echo -e \\a
echo “**** $1 has just logged in ****”

exit 0


