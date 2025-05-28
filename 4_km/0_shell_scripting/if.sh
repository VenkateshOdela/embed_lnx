#!/bin/sh



# if control statment example
echo "enter two numbers"
#read var1
#read var2

#if [ $var1 -eq $var2 ]
#then
#	echo "Numbers are equal"
#else
#	echo "Numbers are not equal"
#fi

TRUE=1
FALSE=0

SKIP_ENCRYPTION=$FALSE

ACTION="CMDARC"

#if [ $SKIP_ENCRYPTION -eq $FALSE ]
#then
#	echo "ENCRYPTION is done"
#else
#	echo "ENCRYPTION is skipped"
#fi

   if [ ${SKIP_ENCRYPTION} -eq 0 ] && [ ${ACTION} == "CMDARCH" ] ; then  
        echo "cmmanded archieve"                                         
   else                                                                                                      
       echo "auto archieve"
   fi 
