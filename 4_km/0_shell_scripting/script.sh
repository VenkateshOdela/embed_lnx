#!/bin/sh

#This shell shows Internet Status

clear
ping -c 3 www.google.com > /dev/null  2> /dev/null && echo "Internet is Working" || echo "Internet is not Working" 


#if ping -c 3 www.google.com > /dev/null  2> /dev/null 
#then 
#	echo "Internet is Working"
#else
#	echo "Internet is not Working" 
#fi

#ping -c 3 www.google.com > /dev/null 2> /dev/null

#if [ $? != 0 ]
#then
#echo "Internet is not working"
#exit 0
#fi

#echo "Kernel Version:`uname -r`"
#echo "Ubuntu Version:`cat /etc/issue`"
#echo "Host name:`hostname`"
