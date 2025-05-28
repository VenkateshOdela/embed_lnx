#!/bin/sh

# control statments in shell
#echo $PWD
#cd /home/kishore
#echo $PWD

# usage of if statment
#if [ -w abc ] ; then
#	echo "abc file have write permission"
#else
#	echo "abc file doen't have write permission"
#fi


#for var1 in 0 1 2 3 4 5
#do
#	echo "\$var1:$var1"
#done

#usage of for statement
#for i in *
#do
#	if grep -rq abc $i 
#	then
#		echo $i
#	fi
#done

#i=1
#k=5
#while [ $i -le $k ]
#do
#	echo $i
#	i=$(expr $i + 1)
#done

#usage of while statement
echo "Enter password"
read trythis

while [ "$trythis" != "secret" ]
do
	echo "sorry, try again"
	read trythis
done

echo "password authentiacation successfully!";
