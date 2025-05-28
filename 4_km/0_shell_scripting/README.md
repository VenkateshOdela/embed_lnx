# shell-scripting

Shell Scripting class notes with examples

Command Description syntax
--------------------------
angle brackets for required parameters: ping <hostname>
square brackets for optional parameters: mkdir [-p] <dirname>
ellipses for repeated items: cp <source1> [source2...] <dest>
vertical bars for choice of items: netstat {-t|-u}

What is Shell?
-------------
Shell is a tool to execute commands.

Shell Script:
------------
A shell script is a computer program designed to be run by the Unix shell, a command line interpreter. 

Shell Syntax:
-------------
variables: strings,numbers,environments and parameters
Conditions: Shell Booleans
Program control: if, elif,for,while,untile,case
Lists
Functions


The below examples explain shell programming basics:
---------------------------------------------------

Hello World Schell Program:
---------------------------
hello.sh: Hello World Shell Program (echo and read commands).



Quotations & Substitutions:
---------------------------
quotations.sh: How to use environment variables and quotations in shell.


Shell Parameters:
----------------
parameters.sh: shell command line arguments.

$0	The name of the shell script
$#	The number of parameters passed
$$	The process ID of the shell script

$1,$2,...	The parameters given to the script
$*		list of all the parameters, in a single variable, separated by the first character in the environment variable IFS.
$@ 		A subtle variation on $*; it doesn’t use the IFS environment variable, so parameters are not run together even if IFS is empty



Shell Conditions: [conditions.sh]
-----------------
3 types of conditions 

	String Comparison Result
	---------------------
	string1 = string2	True if the strings are equal
	string1 != string2	True if the strings are not equal
	-n string		True if the string is not null
	-z string		True if the string is null (an empty string

	Arithmetic Comparison Result
	----------------------------
	expression1 -eq		expression2 True if the expressions are equal
	expression1 -ne		expression2 True if the expressions are not equal
	expression1 -gt		expression2 True if expression1 is greater than expression2
	expression1 -ge		expression2 True if expression1 is greater than or equal to
	expression2
	expression1 -lt		expression2 True if expression1 is less than expression2
	expression1 -le		expression2 True if expression1 is less than or equal to
	expression2
	! expression		True if the expression is false, and vice versa.


	File Conditional Result
	-----------------------
	-d file		 True if the file is a directory
	-e file		 True if the file exists. Note that historically the -e option has not been portable, so -f is usually used.
	-f file		 True if the file is a regular file
	-g file		 True if set-group-id is set on file
	-r file		 True if the file is readable
	-s file		 True if the file has nonzero size
	-u file		 True if set-user-id is set on file
	-w file		 True if the file is writable
	-x file		 True if the file is executable



Shell Control Statements:
------------------------

	control.sh: control statements.

while condition
do
	statements
done
While execute the statements until the condition FAILS.
until execute the statements until the condition SUCCESS.


Shell ERROR Handling:
--------------------
script-error.sh: Proper ERROR handling in shell.



Shell Debugging:
---------------
script7.sh: debugging in shell
	$ sh -x ./<script_name>






Functions in Shell: [ Example: functions.sh ]
-------------------

You can define functions in the shell.

To define a shell function, simply write its name followed by empty parentheses and enclose the statements in braces:


function_name () 
{
	echo $1
	statements
}

function_name 12


Export user defined environment variables: [ Example: export1.sh, export2.sh ]
-----------------------------------------
By default, variables created in a shell are not available in further (sub)shells invoked from that shell. 
The export command creates an environment variable from its parameter that can be seen by other scripts and programs invoked from the current program.

The commands set -a or set -allexport will export all variables thereafter.







Usage of Shell scripting:
------------------------
1. ping.sh: This shell shows Internet status.
2. server-monitor.sh: network monitor application show the below information 
	a OS name
	b. Architecture version
	c. Kernel version
	d. Internet status
	e. IP Address
	f. Memory Usage
	g. Disk file systems usage
	h. system uptime
3. km-bbb-kernel-build.sh: Environment Setup, Configure,Compilation of kernel source code.
4. km-bbb-kernel-install.sh: Installation of kernel source code.
  





Background Process vs foreground Process:
-----------------------------------------

	What is Process: 
	---------------
	Process is an execution of a program (or) is an instance of a program.

	Create a process:
	-----------------
	Command line interface [CLI] or Graphical interface [GUI].

	List of running process: 
	-----------------------
	$ ps -eaf

	Process Execution:
	----------------- 
	Foreground process [ Interactive Process ] and Background Process [ Non-Interactive Process / Daemon Process]


	Create a process in Foreground:
		$ gnome-calculator 

	Create a process in background [Daemon]
		$ gnome-calculator &


	List of background process:
		$ jobs

	Move process from bg ->  fg:
		$ fg 
	
	Move Particular process from bg -> fg:
		$ fg <job_id>

