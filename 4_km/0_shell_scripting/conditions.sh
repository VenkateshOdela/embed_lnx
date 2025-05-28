#!/bin/sh


# if control statment example

echo "enter two numbers"
read var1
read var2

if [ $var1 -eq $var2 ]
then
	echo "Numbers are equal"
else
	echo "Numbers are not equal"
fi
