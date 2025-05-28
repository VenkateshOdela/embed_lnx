#!/bin/sh

# This example shows how to use functions in shell.

# function_name() {
#  statements
# }

status()
{
	echo "inside status function"
	echo $1
	echo $2
}

echo "script starts here"

status 12 13

echo "script ends here"



