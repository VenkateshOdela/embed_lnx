#!/bin/bash
#============================================================================
#                        AUTOMATED LOGIC CORPORATION
#                   Copyright (c) 2015 All Rights Reserved
#        This document contains confidential/proprietary information.
#============================================================================
#
#  This script is responsible for starting and restarting the firmware
#  application (i.e. drv_fwex.elf). Once launched, this script blocks waiting
#  for the exit status. Upon a restart, the "reason for restart" will be
#  appended as an argument to the execution request.
#


FIRMWARE_EXE=drv_fwex.elf
FIRMWARE_DIR=/home/root/firmware
FIRMWARE_APP=$FIRMWARE_DIR/$FIRMWARE_EXE
CMD_FILE='/var/tmp/cmd_file'
FIRMWARE_STATUS_FILE='/var/syslog/fwex_status_file'
WDT_REBOOT_MARKER_FILE='/var/syslog/wdtFlagFile'
WDT_RESTORE_MARKER_FILE='/var/syslog/wdtRestoreFile'
RESTORE_SCRIPT='/home/root/firmware/backup.sh --restore'

# Supported commands
start_cmd="start"
stop_cmd="stop"
exit_cmd="exit"
reboot_cmd="reboot"
SIGUSR2=`kill -l USR2`
# maximum amount of time to wait for reboot
MAX_REBOOT_WAIT="2"

# Supported reasons for restart. These strings are used for setting reset
# counters within the firmware application and must retain their spelling
# to match preprocessor directives defined in /execb/ix86/shared/sw_watchdog.h.
WDT_STARTUP_ARG=$1
WDT_RESTART="wdtRestart"
WDT_RESTART_CORE="wdtRestartCore"
WDT_REBOOT="wdtReboot"
WDT_COMMANDED="wdtCommanded"
WDT_SYSTEM_ERROR="wdtSystemErrors"
WDT_RESTORE="wdtRestore"
REBOOT_REQUESTED="RebootRequested"

CMNET_MSTP_DRIVER=mstp_cmnet
CMNET_MSTP_IF=mstp0
TPI_SERIAL_DRIVER=omap_serial_tpi
TPI_MSTP_DRIVER=mstp_tpi
TPI_MSTP_IF=mstp1
ARCNET_IF=arc0
CMNET_SERIAL_DRIVER=omap_serial_cmnet

PLAT_CAN_DRIVER=c_can_platform
BASE_CAN_DRIVER=c_can
CAN_IF=can0
IONET_DRIVER=ionet
IONET_IF=ionet0
RNET_DRIVER=rnet
RNET_IF=rnet0

CAN_IF_CMD=socketcan_stop
CAN_IF_DOWN_CMD="$FIRMWARE_DIR/$CAN_IF_CMD $CAN_IF"

FWEX_SET_ENV_FILE=$FIRMWARE_DIR/fwex_set_env.source
DEVMEM2=/usr/bin/devmem2
PRM_RSTCTRL=0x44e00f00
SYSRQ=/proc/sys/kernel/sysrq
SYSRQ_TRIGGER=/proc/sysrq-trigger
REBOOT=reboot

CORE_DUMP_LOCATION=/var/local/config
CORE_DUMP_PATTERN=${CORE_DUMP_LOCATION}/%e.core
CORE_DUMP_FILE=${CORE_DUMP_LOCATION}/drv_fwex*.core
CORE_DUMP_TGZ_LOCATION=/var/syslog/core
CORE_DUMP_TGZ_FILE=${CORE_DUMP_TGZ_LOCATION}/drv_fwex.elf.core.tgz

RESTART_IO_SERVER='FALSE'
IO_SERVER_SCRIPT='/etc/init.d/io-serverd'

#start_ero
CMDARCH_DIR=/var/local/config/cmdarch
CMDARCH_NAME="cmdbackup"
#CMDARCH_TARBALL=${CMDARCH_DIR}/${CMDARCH_NAME}.tar
CMDARCH_TARBALL=tar1.tar
#BACKUP_SCRIPT=backup.sh
BACKUP_SCRIPT="read.sh"

AUTOBACKUP_MARKER=markerfile

BACKUP_SCRIPT_TAR='/home/root/firmware/backup.sh --autoarch1'
BACKUP_SCRIPT_ENCRYPT='/home/root/firmware/backup.sh --autoarch2'

ERO_LOG=/var/ero.log
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

