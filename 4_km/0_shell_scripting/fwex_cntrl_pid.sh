

#start_ero
#CMDARCH_DIR=/var/local/config/cmdarch
#CMDARCH_NAME="cmdbackup"
#CMDARCH_TARBALL=${CMDARCH_DIR}/${CMDARCH_NAME}.tar
CMDARCH_TARBALL=tar1.tar
BACKUP_SCRIPT=backup.sh
#BACKUP_SCRIPT="read.sh"

AUTOBACKUP_MARKER=markerfile

#BACKUP_SCRIPT_TAR='/home/root/firmware/backup.sh --autoarch1'
#BACKUP_SCRIPT_ENCRYPT='/home/root/firmware/backup.sh --autoarch2'

#ERO_LOG=/var/ero.log
#end_ero
# ============================= FUNCTIONS ================================




perform_autobackup()
{
   if [ ! -f ${CMDARCH_TARBALL} ]; then
    	echo "Do Auto-Backup, As detected webctrl memory-download" 

   elif [ ! $(pidof -x ${BACKUP_SCRIPT}) ]; then # Enycryption script not running
      echo "script not running...."
      if [ ! -f ${AUTOBACKUP_MARKER} ]; then
         echo "Just now auto-backup compeleted, Which was initiated previously." 
      else
         #Controller may be restared while Encryption. So Clean tarball & Do Auto-Backup 
         #rm -f ${CMDARCH_TARBALL}
         #${BACKUP_SCRIPT_TAR}
         EXIT_CODE=1
         if [ ${EXIT_CODE} -ne 0 ]; then
            echo "Clean tarball & Auto-Backup1(TAR): ERROR, exit code ${EXIT_CODE}." 
         else                                       
            echo "Clean tarball & Auto-Backup1(TAR): OK" 
            #${BACKUP_SCRIPT_ENCRYPT} &
         fi
      fi
   else                                       
      echo "Auto-Backup is already in progress, Which was initiated previously." 
   fi
}

#------------------------------------------------------------------------------
#end_ero

main()
{
   #enable_core_dump
   #cd $FIRMWARE_DIR
   # Power cycle restarts are considered normal.
  perform_autobackup

}






# ============================= SCRIPT BEGIN =============================

#trap "signal_handler" SIGHUP SIGINT SIGTERM

# If for some reason this script lost control
# of the firmware application, kill it!
#nuke_firmware_if_running

main

#exit 0;

