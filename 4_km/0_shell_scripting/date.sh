#!/bin/sh


   # get date and time
   DATE=`date +"%F %X"`
   # note: 2>&1 | tee ${LOG_FILE} will send stdout and stderr 
   # to console and file
   echo "time : ${DATE}" 