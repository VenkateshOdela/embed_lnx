/**=========================================================================
                          AUTOMATED LOGIC CORPORATION
                    Copyright (c) 2015 All Rights Reserved
         This document contains confidential/proprietary information.
============================================================================
  start_fwex.c

  File Description: 
       This file is the entry point for this Linux application.
==========================================================================*/
/*------------------ I N C L U D E   S E C T I O N -----------------------*/

#ifdef PACK_STRUCT2
#pragma pack(push,4)
#endif

#include STDIO_H
#include STDLIB_H
#include <sys/stat.h>

#ifdef PACK_STRUCT2
#pragma pack(pop)
#endif

#include "alctypes.h"
#include "alcos.h"
#include "execapis.h"
#include "boot.h"
#include "errcodes.h"
#include "sw_watchdog.h"
#include "hw.h"
#include "nvrammap.h"
#include "bptypes.h"
#include "archfile.h"

#ifdef PACK_STRUCT2
#pragma pack(push,4)
#endif

#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifdef PACK_STRUCT2
#pragma pack(pop)
#endif

/*------------------ D E F I N E S   S E C T I O N ------------------------
   Locally defined macros, typedefs, structures, enums, etc. for use in 
   this file only.
--------------------------------------------------------------------------*/
// Maximum number of tasks allowed in the deletion queue at one time
#define MAX_TASK_DELETE 16

/*------------------ E X T E R N   D A T A --------------------------------
   Variables defined elsewhere, but declarations not #included above.
   NOTE: Recommend all global data have a declaration in a header file.
--------------------------------------------------------------------------*/


/*------------------ E X T E R N   P R O T O T Y P E S --------------------
   Prototypes of global functions without declarations #included above. 
   NOTE: Recommend all global functions have a declaration in a header file.
--------------------------------------------------------------------------*/
void Main_Cleanup(int sig);
void Application_Initialize(void *pvTaskPars);
extern void CheckNVRam(BOOL fClear);
extern void root(void *pvTaskPars);
extern STATUS ExecCreateHisrWrapper(void); // Initializes the Exec HISR wrapper.
extern void ApplicationShutdown(void);
extern void DeletePacketBufferPool(void);
extern ULONG _ExecTaskCreate(ULONG *pidTask, char *pszName, void (*pfnTask)(void *), \
                             ULONG ulPriority, size_t sizeStack, size_t sizeData, \
                             void *pInitialData, BOOL pidAdd);


/*------------------ S T A T I C   P R O T O T Y P E S --------------------
   Prototypes of functions defined and only used in this file.
--------------------------------------------------------------------------*/
static void SupervisoryTask(void *pvTaskPars);
static void SigInt_SigTerm_Catcher(void *pvTaskPars);
static void SigSegv_Catcher(void *pvTaskPars);
static void CheckRecoveryPartition(void);

/*------------------ D A T A   D E F I N I T I O N S ----------------------
   Static and global variables (including const) defined in this file.  
   NOTE: Recommend all global variables be declared in a header file.
--------------------------------------------------------------------------*/
ULONG qidTaskDelete, tidSupervisory;

FILE *myfp=NULL;

void init_mylog()
{
	myfp = fopen("/var/mylog_fwex", "a");
	if (myfp == NULL)
	{
		printf("\n /var/mylog_fwex: OPEN ERROR");
	}
	else
	{
		fprintf(myfp, "\n-----------------FWEX START, ERROR_RESOURCE_OTHER Version: B12-----------------------------\n");
		fflush(myfp);
	}
}

/*------------------ F U N C T I O N S -----------------------------------*/

/**=== main ==============================================================
   
   This is the C main function where ExecB execution will start.

   Name        Dir   Description
   argc        in    Count of command line arguments. (Not used.)
   argv        in    Command line arguments. (Not used.)
   
   Returns: ExecB application exit status. (Should not happen here.
            See the Main_Cleanup() function below.)
   
   Cautions: None.
   
=========================================================================*/
int main(int argc, char** argv)
{
   ULONG tid, sts = 0;
   struct sigaction new_action;
   pthread_t *pThread;
   sigset_t segset_t_inherit;

   init_mylog();

   // process command line startup arguments
   // this allows setting of the STARTUP_SW_WATCHDOG STARTUP_WATCHDOG
   // STARTUP_COMMANDED STARTUP_FATAL_ERR STARTUP_POWERUP from an
   // external monitoring application
   ulExecStartupFlags = CheckWatchdogStatus(argc, argv);

   // load nvram from disk
   NVRamLoad();

   // OR in the original reason for restart - not determined by CheckWatchdogStatus() call
   ulExecStartupFlags |= nvboot.ulBootStartupFlags;

   // Note: The call to ExecTaskInit() will perform sigaction as SIGUSR1.
   ExecTaskInit();

   // Per-thread signal masks: By default, the first thread in a child process inherits its signal mask
   // from the thread in its parent that called fork. Additional threads inherit the signal mask of the
   // thread that issued the pthread_create (aka ExecTaskCreate) that created them.
   sigemptyset(&segset_t_inherit);
   sigaddset(&segset_t_inherit, SIGSEGV);
   sigaddset(&segset_t_inherit, SIGINT);
   sigaddset(&segset_t_inherit, SIGTERM);
   // The set of blocked signals is the union of the current set and the new pthread_sigmask set.
   sts = pthread_sigmask(SIG_BLOCK, &segset_t_inherit, NULL);
   if (sts != 0)
   {
      if(!CheckSyslogInitFuncPointer())
         printf("main: sigmask sts = %ld\n",sts);
      else
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"main: sigmask sts = %ld",sts);

      return (-1);
   }

   // Create the Application Start task.
   sts = _ExecTaskCreate(&tid,                      // ULONG *pidTask
		                   "APPSTART",                // char *pszName
                         Application_Initialize,    // void (*pfnTask)(void *)
                         RESET_PRIORITY,            // ULONG ulPriority
                         20480,                     // size_t sizeStack
                         0,                         // size_t sizeData
                         NULL,                      // void *pInitialData
                         FALSE);                    // add the task control block to the list

   // ExecTaskCreate returns 0 on success.
   if (sts != 0)
   {
      if(!CheckSyslogInitFuncPointer())
         printf("Create task error: Application_Initialize(); sts = %ld\n",sts);
      else
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"Create task error: Application_Initialize(); sts = %ld",sts);

      return (-1);
   }

   pThread = (pthread_t *)ExecGetPthread(tid);
   if (pThread)
   {
      pthread_join(*pThread, NULL);
   }
   else
   {
      // This should not happen because I just created the thread.
	   if(!CheckSyslogInitFuncPointer())
		  printf("Thread Creation on startup failed!!!\n");
	   else
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"Thread Creation on startup failed!!!");

      return -1;
   }
 
   Main_Cleanup(0);
   return (0);   // Should not get here.
}

/**=== Main_Cleanup ======================================================
   
   This helper function returns the ExecB application status to the
   operating system.

   Name        Dir   Description
   sig         in    The signal causing main to ExecB to exit.
   
   Returns: None.
   
   Cautions: None.
   
=========================================================================*/

void Main_Cleanup(int sig)
{
   static BOOL fShutdownInProgress = FALSE; // If FALSE is zero, this is redundant for static.

   if (sig == 0)
   {
      if (nvboot.ulBootStartupFlags & STARTUP_FATAL_ERR)
      {
         // If fatal error detected, set sig = 1 for indication to app monitor
         sig = 1;
      }
      else if ((nvboot.ulBootStartupFlags & STARTUP_COMMANDED_REBOOT))
      {
         // If commanded to reboot, use SIGUSR2 to inform watchdog
         sig = SIGUSR2;

         if(!CheckSyslogInitFuncPointer())
            printf("Commanded reboot.\n");
         else
            Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"Commanded reboot.");
      }
      else
      {
         if(!CheckSyslogInitFuncPointer())
            printf("Normal exit.\n");
         else
            Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"Normal exit.");
      }
   }
   else if(sig == SIGINT)
   {
      if(!CheckSyslogInitFuncPointer())
         printf("Exit on SIGINT.\n");
      else
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"Exit on SIGINT.");

      NVRamSave(); // Save just the NV, before exit, for the boot message.
   }
   else if(sig == SIGSEGV)
   {
      if(!CheckSyslogInitFuncPointer())
         printf("Exit on SIGSEGV.\n");
      else
         Boot(BOOT_LOG_FATAL,ERR_SOFTWARE,0,"Exit on SIGSEGV.");

      NVRamSave(); // Save just the NV, before exit, for the boot message.
      exit(sig); // SIGSEGV does not save data in the shutdown/cleanup below.
   }
   else if(sig == SIGTERM)
   {
      if(!CheckSyslogInitFuncPointer())
         printf("Exit on SIGTERM.\n");
      else
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"Exit on SIGTERM.");

      NVRamSave(); // Save just the NV, before exit, for the boot message.
   }
   else
   {
      if(!CheckSyslogInitFuncPointer())
         printf("Exit on unknown signal %d.\n", sig);
      else
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"Exit on unknown signal %d.",sig);
   }

   if(!fShutdownInProgress)
   {
      fShutdownInProgress = TRUE; // Only do the shutdown cleanup once, as it is not reentrant.

      ApplicationShutdown();

      // if normal shutdown or commanded reboot
      if(sig == 0 || sig == SIGUSR2)
      {
         // Suspends thread not delete
         ExecDeleteAllThread();
         // Saving the CP memory on exit is now required.
         ArchiveExitNonBlocking();

         // save nvram to disk
         NVRamSave();
         // 2-10-16: We found that explicitly freeing the memory used for the BACnet packet buffer pool can cause an
         //          intermittent SIGSEGV problem during FWEX shutdown with our current exit implementation.
         //          A SIGSEGV occurs when an attempt is made to access the newly freed memory in BpDllEthernetRxTask,
         //          because this task does not yet realize that FWEX is shutting down. See DE975 in Rally for more details.
         //          Efforts are currently underway to refactor the FWEX exit mechanism to avoid this type of issue and more.
         //          Although it is "good form" to explicitly free this memory, Linux will automatically free it for us,
         //          so commenting this out for now should cause no harm and allows us to avoid nuisance SIGSEGV events.
         // Delete Packet memory
         DeletePacketBufferPool();
      }
   }

   exit(sig);
}

/**=== Application_Initialize ============================================

   This function is called by the operating system before the scheduler
   is called for the first time.  Here, we create the system supervisor
   task.
   
   Name        Dir   Description
   argv        In    pvTaskPars (Not used.)
   
   Returns: None.
   
   Cautions: None.
   
=========================================================================*/
void Application_Initialize(void *pvTaskPars)
{
   STATUS error;
   pthread_t *pThread;

   // Create the task deletion queue.
   qidTaskDelete = ExecQCreate("TDEL", MAX_TASK_DELETE);
   if (qidTaskDelete == 0L)
   {
      printf("Application_Initialize: Error from ExecQCreate.\n\n");
      return;
   }

   /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
   // Create SupervisoryTask() task.
   if (_ExecTaskCreate(&tidSupervisory, "SUPV", SupervisoryTask, ROOT_PRIORITY, ROOT_STACK_SIZE, 0, NULL, FALSE) != 0L)
   {
      printf("Application_Initialize: Error from ExecTaskCreate.\n");
      return;
   }

   pThread = (pthread_t *)ExecGetPthread(tidSupervisory);
   if (pThread)
   {
      pthread_join(*pThread, NULL);
   }
   else
   {
      printf("Thread Creation on Application_Initialize failed!!!\n");
      return;
   }
}
/**=== CheckRecoveryPartition ============================================
   Check for existence of recovery partition and log boot warning if not found.
   The external script fwex_cntrl.sh also checks for existence of the recovery
   partition, and if it is not found, dynamically modifies the manifest file to
   force a download and install of the recoveryfiles bundle.

   Name        Dir   Description

   Returns:
      void

   Cautions:
      none

=========================================================================*/
static void CheckRecoveryPartition(void)
{
   const char* szRecoveryDir = "/var/volatile/run/media/mmcblk0p3/home/root/usb_driverx_file_restore";
   struct stat st = {0};

   if (stat(szRecoveryDir, &st) == -1 || !S_ISDIR(st.st_mode))
   {
      Boot(BOOT_WARNING, 0, "911 USB recovery may not work properly");
   }
}
/**=== SupervisoryTask ====================================================
   The supervisory task is responsible for deleting tasks, freeing
   associated memory, and handling reset requests.
   
   Name        Dir   Description
   pvTaskPars  In    Task arguments

   Returns:
      Does not return.

   Caution:
      Nothing called from this task may be allowed to
      do a Boot(BOOT_FATAL... call.  This will likely cause recursion
      and a stack failure.


=========================================================================*/
static void SupervisoryTask(void *pvTaskPars)
{
   extern void InitFeaturesSupported(USHORT usEepromProductType); // Defined in mynode.c
   extern void InitDeviceFamily(); // Defined in mynode.c
   extern INT CreateBGDLSessionTimer(void); // Defined in bgdl_processor.c

   ULONG tid;
   ULONG ulErr;
   ULONG error;

   // Validate Nonvolatile RAM sections.
   // WARNING!! This function must be called before any calls to BootServer(),
   // a.k.a. Boot().  The error history logs may not be usable until
   // CheckNVRam() is called.
   CheckNVRam(FALSE);

   // Initialize hardware.
   HwInit();

   //
   // Initialize features supported based on product type
   // Product type is read from eeprom in HwInit()
   //
   InitFeaturesSupported(Boot(BOOT_GET_PRODUCT_TYPE));

   //
   // Initialize controller device family, like Router, IC, NGZN, NGVAV, etc...
   // Device family is read from setup_device_environment.sh
   //
   InitDeviceFamily();

   //
   // Check for existence of recovery partition
   //
   CheckRecoveryPartition();

   //
   // Create session timer for Background Download
   //
   CreateBGDLSessionTimer();

   // Create root() task.
   if (ExecTaskCreate(&tid, "ROOT", root, ROOT_PRIORITY, ROOT_STACK_SIZE, 0, NULL) == 0)
   {
      // Create the SIGSEGV task.
      //if (ExecTaskCreate(&tid, "SEGV", SigSegv_Catcher, ROOT_PRIORITY, ROOT_STACK_SIZE, 0, NULL) != 0)
      if (_ExecTaskCreate(&tid, "SEGV", SigSegv_Catcher, ROOT_PRIORITY, ROOT_STACK_SIZE, 0, NULL, FALSE) != 0)
      {
         Boot(BOOT_LOG_FATAL, (ULONG)ERR_NUCLEUS, (ULONG)0, "Cannot launch SEGV task.");
         return; // Well, fail!
      }

      // Create the SIGINT/TERM task.
      //if (ExecTaskCreate(&tid, "SIG2", SigInt_SigTerm_Catcher, ROOT_PRIORITY, ROOT_STACK_SIZE, 0, NULL) != 0)
      if (_ExecTaskCreate(&tid, "SIG2", SigInt_SigTerm_Catcher, ROOT_PRIORITY, ROOT_STACK_SIZE, 0, NULL, FALSE) != 0)
      {
         Boot(BOOT_LOG_FATAL, (ULONG)ERR_NUCLEUS, (ULONG)0, "Cannot launch SIG2 task.");
         return; // Well, fail!
      }

      // Now pend and process task deletions.
      // Deletion of this task results in a reset.
      for(;;)
      {
         tid = (ULONG)ExecQPend(qidTaskDelete, 0L, &ulErr);
         if (!ulErr)
         {
            // Deletion of this task results in a reset.
            if (tid == tidSupervisory)
            {
               // Break out of this loop and reset.
               printf("SupervisoryTask: Got tidSupervisory from queue.\n");
               break;
            }
            else
            {
               // Delete the task.
               error = ExecTaskDelete(tid);

               // ExecTaskDelete returns 0 on success.
               if (error != 0)
               {
                  Boot(BOOT_LOG_FATAL, ERR_NUCLEUS, error, 
                       "OS: %d task not deleted.", tid);
                  // Break out of this loop and reset.
                  break;      
               }
            }
         }
      }
   }
   else
   {
      Boot(BOOT_LOG_FATAL, (ULONG)ERR_NUCLEUS, (ULONG)0, "Cannot launch ROOT task.");
   }
}

/**=== SigInt_SigTerm_Catcher ============================================

   This function will receive the SIGINT and SIGTERM signals and cause
   the system to exit.
   
   Name        Dir   Description
   pvTaskPars  In    Task arguments

   Returns:  None.
   
   Cautions: SIGINT and SIGTERM need to be blocked for all threads before
             this thread is created and the inherited mask for this
             task also requires they are blocked.
   
=========================================================================*/
static void SigInt_SigTerm_Catcher(void *pvTaskPars)
{
   sigset_t sigset_t_sigint_sigterm;
   int iSig, iRet;

   // Setup a signal mask of the signals to wait on.
   sigemptyset(&sigset_t_sigint_sigterm);
   sigaddset(&sigset_t_sigint_sigterm, SIGINT);
   sigaddset(&sigset_t_sigint_sigterm, SIGTERM);

   while(1)
   {
      iRet = sigwait(&sigset_t_sigint_sigterm, &iSig);

      // The sigwait() function returns zero on success.
      if(iRet == 0)
      {
         Main_Cleanup(iSig); // Time to go.
      }

      ExecDelay(1); // This task should not loop frequently, but just in case.
   }
}

/**=== SigSegv_Catcher ===================================================

   This function will receive the SIGSEGV signal and cause
   the system to exit.
   
   Name        Dir   Description
   pvTaskPars  In    Task arguments

   Returns:  None.
   
   Cautions: SIGSEGV needs to be blocked for all threads before
             this thread is created and the inherited mask for this
             task also requires they are blocked.

             The handling of SIGSEGV is separate from SIGINT/SIGTERM on
             purpose to allow for the signals to be handled in a nested
             manner.
   
=========================================================================*/
static void SigSegv_Catcher(void *pvTaskPars)
{
   sigset_t sigset_t_sigsegv;
   int iSig, iRet;

   // Setup a signal mask of the signal to wait on.
   sigemptyset(&sigset_t_sigsegv);
   sigaddset(&sigset_t_sigsegv, SIGSEGV);

   while(1)
   {
      iRet = sigwait(&sigset_t_sigsegv, &iSig);

      // The sigwait() function returns zero on success.
      if(iRet == 0)
      {
         Main_Cleanup(iSig); // Time to go.
      }

      ExecDelay(1); // This task should not loop frequently, but just in case.
   }
}


