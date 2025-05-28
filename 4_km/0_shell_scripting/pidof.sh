#!/bin/sh

SH=backup.sh

READ=read.sh


echo "using pidof............"

pid=$(pidof -x ${READ})

echo "pid = $pid"

#if [ $(pidof -x read.sh) ];then
if [ ! $(pidof -x ${READ} ) ];then # script
 echo "script running.."
else
 echo "no ...."
fi



 #  elif [ ! $(pidof ${SH}) ]; then # Enycryption script not running



