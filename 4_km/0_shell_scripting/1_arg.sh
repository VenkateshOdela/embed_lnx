#!/bin/sh



echo "No. of arguments: $#"
echo "Shell script name:$0"
echo "Shell name:$SHELL"
echo "1st arg:$1"
echo "2nd arg:$2"
echo "All arg:$@"
echo "All arg:$*"


echo "Venkatesh Odela"
LAST_CMND_STATUS=$?
echo "$LAST_CMND_STATUS"



#
#	$#    Stores the number of command-line arguments that 
#	      were passed to the shell program.

#	$0    Stores the first word of the entered command (the 
#	      name of the shell program).

#	$?    Stores the exit value of the last command that was 
#	      executed.

#	"$@"  Stores all the arguments that were entered
#	      on the command line, individually quoted ("$1" "$2" ...)

#	$*    Stores all the arguments that were entered on the
#	      command line ($1 $2 ...).

