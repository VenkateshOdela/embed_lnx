/**=========================================================================
                          AUTOMATED LOGIC CORPORATION
                    Copyright (c) 2016 All Rights Reserved
         This document contains confidential/proprietary information.
============================================================================
  fwex.c

  File Description:   Essential data structures, definitions and startup
    routines for Router-E based on FWEX platform.

==========================================================================*/
/* This Copyright string is here so that the ELF file contains a copyright
   string. The command "strings" ran on the ELF file displays the ASCII
   text contained inside the binary.*/
volatile static char* copyRightString = "AUTOMATED LOGIC CORPORATION\nCopyright (c) 2016 All Rights Reserved\nThis document contains confidential/proprietary information.";
/*------------------ I N C L U D E   S E C T I O N -----------------------*/
#ifdef PACK_STRUCT2
#pragma pack(push,4)
#endif

#include STDIO_H
#include STRING_H
#include STDLIB_H
#include <fcntl.h>
#include <syslog.h>
#include <semaphore.h>
#include <fcntl.h>
#include <arpa/inet.h>
#ifdef PACK_STRUCT2
#pragma pack(pop)
#endif

#include "assert.h"
#include "alctypes.h"
#include "alccodes.h"
#include "alcos.h"
#include "os_glue.h"
#include "execapis.h"
#include "drv_xxx.h"
#include "alcmath.h"
#include "alctime.h"
#include "headers.h"
#include "boot.h"
#include "bacnet.h"
#include "exec.h"
#include "err_list.h"
#include "db.h"
#include "nvstore.h"
#include "dlink.h"
#include "cp.h"
#include "execinit.h"
#include "mb.h"
#include "mbbma.h"
#include "mbtrend.h"
#include "hw.h"
#include "clock.h"
#include "uttimer.h"
#include "ee_base.h"
#include "ippars.h"
#include "ip100bt.h"
#include "arptest.h"
#include "mynode.h"
#include "sw_watchdog.h"
#include "modstat.h"
#include "nvstruct.h"
#include "bpdll.h"
#include "bpdbind.h"
#include "mbclient.h"
#include "serial.h"
#include "colors.h"
#include "om.h"
#include "bpobj.h"
#include "dllmgmt.h"
#include "data_server.h"
#include "app_data_server.h"
#include "data_functions.h"
#include "rotarysw.h"
#include "nvrammap.h"
#include "archfile.h"
#include "resetc.h"
#include "GpioOps.h"

#include "ringbuf.h"
#include "teldiag.h"
#include "ipd.h"
#include "ipd_xxx.i"
#include "store.h"
#include "autonet.h"
#include "autoid.h"

#include "bpcfg_fwex.h"
#include "bpcfg.h"
#include "bpobj.h"
#include "bptimemaster.h"
#include "upgrade_status.h"
#include "fakedownload.h"
#include "bacnet_firewall.h"
#include "netstats.h"
#include "packetcap.h"
#include "io_client.h"
#include "hw10hz.h"
#include "mbscale.h"

#include "rsheader.h"
#include "rnetif.h"
#include "rs.h"
#include "mbbai.h"
#include "mbrsf.h"
#include "mbrs.h"
#include "telnet.h"
#include "NvTBCalibFileOps.h"
#include "binary_file_ops.h"
#include "ledcontrol.h"
#include "bgdl_processor.h"

// Relative Scheduling

#include "mbsrss.h"
#include "mbbbvp.h"
#include "mbbavp.h"
#include "mbbmsvp.h"
#include "mbclock.h"
#include "relsched.h"

#include "slink.h"
#include "systmr.h"
#include "local_network_data.h"
#include "local_network_page.h"
#include "local_network_config.h"

#include "app_device_codec.h"
#include "app_device_mngmt.h"
#include "sntp_time_update.h"

extern FILE *myfp;

/*------------------ D E F I N E S   S E C T I O N ------------------------
   Locally defined macros, typedefs, structures, enums, etc. for use in
   this file only.
--------------------------------------------------------------------------*/
// This macro is used to access the network number members
// in the drvstat structure.  If the pointer is not initialized
// it will evaluate to 0.
#define NET_NUMBER(x)      (pSysstat?pSysstat->awNet[x]:0)

// For display of BACnet protocol status strings on IPD property pages.
#define BPSS_SIGNATURE     0
#define BPSS_VERSION       1
#define BPSS_DATALINK_0    2

#define ONE_MINUTE_IN_SECONDS    60

// Note: This cannot be in the "DATA DEFINITIONS" section below because it
//       is needed in the macro below.
static char * const aszBACnetProtocolStatusStrings[] =
{
   "BACnet",
   "",
   "BACnet/IP",
   "BACnet/MSTP",
   "BACnet/MSTP",
   "BACnet/Ethernet",
   "BACnet/ARCNET",
   "Service Port",
};
/*
typedef enum
{
   IPD_NONE = 0,
   IPD_GIGE,
   IPD_PORTS1,
   IPD_PORTS2,
   IPD_BAS,
   IPD_DEVICE,
   IPD_INVISIBLE = 255
} IPD_PORT_ENUM;
*/

// Defines for indexing into the array for company specific labeling
#define ALC_CARRIER_PORTS 0
#define OEM_PORTS         1

#define ALC_CAR_PORTS1_NAME "Port S1"
#define ALC_CAR_PORTS2_NAME "Port S2"

// Due to repeated changes in these strings a mechanism
// for differentiating between OEM and ALC/Carrier was created
// and determined should persist based on the fluidity of this requirement
// despite their values being equal....today.
#define OEM_PORTS1_NAME ALC_CAR_PORTS1_NAME
#define OEM_PORTS2_NAME ALC_CAR_PORTS2_NAME


static INT iPortNames = ALC_CARRIER_PORTS;
static char *pszPortS1Name = ALC_CAR_PORTS1_NAME;
static char *pszPortS2Name = ALC_CAR_PORTS2_NAME;

// Number of standard BACnet data links included on
// the protocol status property page.
#define NUM_DATALINKS  (DIM(aszBACnetProtocolStatusStrings)-BPSS_DATALINK_0)
#define NUM_DATALINKS_NON_ROUTER  (NUM_DATALINKS - 1)

// valid baud rates
#define DRV_MSTP_BAUD_9600    9600
#define DRV_MSTP_BAUD_19200   19200
#define DRV_MSTP_BAUD_38400   38400
#define DRV_MSTP_BAUD_57600   57600
#define DRV_MSTP_BAUD_76800   76800
#define DRV_MSTP_BAUD_115200  115200

// module alarm definitions
#define  MA_DEAD_MODULE                   0 // Dead Module alarm
#define  MA_FLOW_CONTROL_TROUBLE          1 // Flow control alarm 
#define  MA_REHEAT_VALVE_TROUBLE          2 // Reheat valve alarm 
#define  MAX_DRIVER_ALARM_OBJS            3 // Max Driver Alarm

#define FWDOWNLOAD "/var/local/config/CheckFirmwareDownload"

#define ETH_PORT_CONFIG_MARKER_FILE "/var/tmp/eth_port_config.marker"

#define MAX_AUTOBAUD_LENGTH         16
/* ----------------------------------- PARS --------------------------------
   File 1 actually consists of the following (4) data structures:
   {
     DEVICE_PARS  (Device Object parameters)
     MPXPARS      (Equivalent to the old SYSPARS, common to all MPX drivers)
     DRVPARS      (Specific to this driver, see below)
     BYTE[4000]   ==>  4000 bytes reserved for free floating BACnet objects
                       to be instantiated by the module at startup.
                       Each object's properties must be contiguous,
                       and the Object ID must be the first property.
                       Currently, we only support Notification Class
                       objects constructed per NCLASS_PARS  in BPOBJ.H.
                       An object ID of 0xFFFFFFFF marks the end of the list.
   };

*/


// Here is the DRVPARS component of File 1:
typedef struct
{
   BYTE  bMstpAutobaud[2];
   ULONG ulPortBaud[2];
   BYTE  bPortMACAddr[2];     // Index PORTS1 is overloaded with configurable Port S1 MSTP MAC if Carrier with IP/Ethernet home network

   BYTE bDisableEthPort;      // This configurable option allows user to enable and disable the "eth0 port- MECPU" and "eth1 port- NGZN/NGVAV"
   BYTE bSpare[3];

   IPNEXTPARS  ipnext;        // next IP configuration to use
   IPNATPARS   ipnat;         // NAT parameters
   CACHEROUTE cachepars;      // color caching parameters
   BOOL  fLocatorLed;         // Locator Blink button state	

   LOCAL_NW_PARS LocalNwPars;
   SNTP_PARS     SntpPars;    // Sntp Pars
   MODULE_ALARM_PARS    aDriverAlarmPars[MAX_DRIVER_ALARM_OBJS];  // module alarms for this driver
   
   ArchivePars ArchiverPars;

   BP_TIME_MASTER_PARS TimeMasterPars;


   BOOL fConfirmBacnetFirewallSettings; // This is a momentary setting that gets cleared WITHOUT Breaking the PARS seal after it is consumed
   BACNET_FIREWALL_PARS bfPars;

   EXPANDER_PARS  ExpanderPars;

   ACTNET_PARS    ACTnetPars[MAX_ACTNET_DEVICES_SUPPORTED];

   CTT ctt[NUM_CTT_TABLES];        // 270 bytes, 3 custom translation tables

   // Global IPD parameters...
   BYTE           fEnableDiag;
   BYTE           bNumTelnetSessions;

   // BMS Stand Alone Timeout feature parameters.
   WORD wStandAloneTimeout;
   BYTE bStandAlonePort;
   BYTE fEnableMSTPonTPIPort; //Added this flag to store Active protocol on
                              // on TPI port ,we will update this from LocalUI

   // IPD parameter space...
   // IPDPARSSIZE may be padded, so keep this member last to avoid problems with XML size mismatches.
   BYTE           abIPD[IPDPARSSIZE_TOTAL];     // Reserved space for IPD's

   // Relative Scheduling
   RTS_PARS rtspars;
} DRVPARS;


/* ----------------------------------- STAT --------------------------------
   File 2 actually consists of the following (3) data structures:
   {
     DEVICE_STAT  (Device Object parameters)
     MPXSTAT      (Equivalent to the old SYSSTAT & DRVREPORT, common to all MPX drivers)
     DRVSTAT      (Specific to this driver, see below)
   };
*/
// Here is the DRVSTAT component of File 2:
typedef struct
{
   char           achAutoStatus[2][MAX_AUTOBAUD_STATUS_LENGTH];
   BYTE           bPortMACAddr[2];              // The MS/TP address if fixed.
   BYTE           bPortDLLType[2];              // 1 byte, Port DLL type (ARCNET(2), MSTP(3) RE:BPDLL.H)
   BOOL           fPortEndOfNetwork[2];         // 4 bytes, Status of the End Of Network Switches
   WORD           wCacheAllNetworksStatus;      // Operational state of wCacheAllNetworks.
   BYTE           bProtocolSelectedOnPort[2];   // Protocol Selected On PortS2, As of now it is not being used for Protocol Selected on PortS1
   BOOL           fHWActnetSupported;           // 2 bytes, ACTnet provided by hardware
   BOOL           fHWPortS1Supported;           // DrvIsHWPortS1Supported :: 2 bytes. (?)
   BOOL           fHWArcnetSupported;           // fHWArcnetSupported :: 2 bytes
   BYTE           bSpares[46];                  // residue from SmartCable Version string.
   IPPARS         ipstat;                       // Actual IP parameters currently in use
   BYTE           abMACAddr[6];                 // Ethernet MAC address
   DLL_COUNTERS   ip;                           // Packet counters for BACnet/IP DLL
   LOCAL_NW_STAT  LocalNwStat;                  // Time remaining to lock the device, Lock device do not accept change in parameter through local network feature
   SNTP_STAT      SntpStat;                     // Sntp State

   MODULE_ALARM_STAT    aDriverAlarmStat[MAX_DRIVER_ALARM_OBJS];  // module alarms for this driver
   ArchiveStat    ArchiverStat;


   BACNET_FIREWALL_STAT bfStat;

   ETHERNET_STATISTICS        EthernetStatistics;
   ARCNET_STATISTICS          ARCnetStatistics;
   MSTP_STATISTICS            MSTP0Statistics;
   MSTP_STATISTICS            MSTP1Statistics;
   ROUTED_PACKETS_STATISTICS  RoutedPacketsStatistic;


   // this structure contains the PORT_CAPTURE structures for the individual
   // physical network interfaces
   ALL_NET_IF_PORT_CAPTURE  PortCapture;

   WORD                          wMaxRealExpanders[2];
   BYTE                          bNumberOfIOnetExpanders;
   BYTE                          bNumberOfXnetExpanders;
   EXPANDER_STATUS               aInternalIOpackInfo[MAX_INTERNAL_IOPACKS_SUPPORTED];   //Internal IOpack info
   EXPANDER_STATUS               aExpanderInfo[MAX_EXPANDERS_SUPPORTED];                //External IOpack info
   EXPANDER_FIRMWARE_INVENTORY   aExpanderFirmwareInventory[MAX_FW_IMAGES];
   ACTNET_STATUS                 aACTnetInfo[MAX_ACTNET_DEVICES_SUPPORTED];
   ACTNET_CONFIG                 aACTnetConfig[MAX_ACTNET_DEVICES_SUPPORTED];

   // T&B parameters
   FLOAT    arTabData[12];       // saved TAB constants
   SHORT    asTabAutoZero[2];    // saved TAB constants - auto zero internal

   BYTE   fFlowTubesReversed[2];     // Onboard & Zasf Airflow input tubes status

   // IPD protocol status
   BYTE                 bIpdStatusCount;
   BYTE                 bSpare;
   IPD_PROTOCOL_STATUS  aIpdStatus[NUM_DATALINKS + NUM_IPDS]; // number of bacnet data links plus number of ipd's

   // IPDSTATSIZE may be padded, so keep this member last to avoid problems with XML size mismatches.
   BYTE           abIPD[IPDSTATSIZE_TOTAL];     // Reserved space for IPD's

   // Relative Scheduling
   RTS_STAT rtsstat;
} DRVSTAT;

/* ----------------------------------- VARS -------------------------------- */
typedef struct
{
   MODULE_ALARM_VARS    aDriverAlarmVars[MAX_DRIVER_ALARM_OBJS];  // module alarms for this driver
   BACNET_FIREWALL_VARS bfVars;
} DRVVARS;

/* ----------------------------------- HIST -------------------------------- */
typedef struct
{
   MODULE_ALARM_HIST aDriverAlarmHist[MAX_DRIVER_ALARM_OBJS];  // module alarms for this driver
   // IPD history space...
   BYTE           abIPD[IPDHISTSIZE_TOTAL];     // Reserved space for IPD's
} DRVHIST;


BACNET_TIME_MASTER_DATA   BACnetTimeMasterData;
BYTE bFwexProfileType = IP_HOME_ROUTER_TYPE;


/*------------------ E X T E R N   D A T A --------------------------------
   Variables defined elsewhere, but declarations not #included above.
   NOTE: Recommend all global data have a declaration in a header file.
--------------------------------------------------------------------------*/
extern IPD_RUN aIpd[NUM_IPDS];

extern ULONG idBpPacketBufferPool;

extern BOOL fDriverEnabled[MAX_NUM_DRIVERS];
extern BOOL fArcnetOnPortS1Enabled;
extern BOOL fBpEthernetDisabled;
extern BYTE fMstpModbusOnPortS2;
/*------------------ E X T E R N   P R O T O T Y P E S --------------------
   Prototypes of global functions without declarations #included above.
   NOTE: Recommend all global functions have a declaration in a header file.
--------------------------------------------------------------------------*/
// This is only use for testing of the BACnet/IP DLL.
extern void BpDllIpChangeNetworkNumber(ULONG ulPort, WORD wNewNetwork);
extern void DrvSetUniqueEepromSerialNumber(void);
extern void AppColorCaching(COLORCACHE pColorCache, BYTE bSendAll);
extern BOOL DrvGetEndOfNetworkSwitch(int iPortIndex);
extern void RefreshNetworkNumbers(void);
extern BOOL ValidateNetworkNumber(int IDX);
extern INT GetAutobaudFromFile(char *pcFilepath);
extern INT SetAutobaudToFile(char *pcFilepath,ULONG ulAutobaud);
void DrvGetBpModelName(CHAR *pszDest, size_t maxlength);
extern void BpDllSetMstpAutobaudFlags(ULONG ulMstpPort, BYTE bMstpAutobaudState);
extern void DrvUpdateAutobaudStats(ULONG ulPort, char *status, ULONG ulAutobaud);
extern BYTE DrvGetDefaultProtocolOnPortS1(USHORT usEepromProductType);
extern BOOL DrvIsPortS1SupportedByProdType(void);
extern BOOL DrvIsHWPortS1Supported(void);
extern BOOL DrvIsHWPortS2Supported(void);
extern void DrvTelnetLogCnt(int FileCount);
extern BOOL ValidateCapturedFiles();
extern  BOOL TelnetSetTaskShutdown();
extern ULONG _ExecTaskCreate(ULONG *pidTask, char *pszName, void (*pfnTask)(void *), \
                             ULONG ulPriority, size_t sizeStack, size_t sizeData, \
                             void *pInitialData, BOOL pidAdd);
extern void RestartFirewall();
extern void BpDllSetMstpAutobaudSyncFlags(ULONG ulMstpPort, BYTE bMstpAutobaudSync);
extern void DisableHomeNetworkonPortS2(void);
extern ULONG IpdGetSlaveId(char *pSignature);

/*------------------ S T A T I C   P R O T O T Y P E S --------------------
   Prototypes of functions defined and only used in this file.
--------------------------------------------------------------------------*/


static void DrvUpdateStatus(CPRUN *pcp);
static void MyLaunchDlls(void);
static void IpInit(void);
static void IpAddressChangeover(void);
static void IpChangeFromNetworkPortObject(void);
static void DecodeDriverOptions(void);
static BOOL (*pfnNetAddModstatChained)(CHAR *pszDest, size_t maxlength);
static void NetAddSerialPortsToModstat(CHAR **ppszDest, size_t *maxsize);
static int GetCurrentAddressValueOnPortS2(void);

static void DrvSetLinuxEnvironmentForNetwork(void);
static BOOL ValidateBaudRate(ULONG ulBaudRate);
static void SavePortHWConfig(void);
static void DrvLoadPortS1Config(void);
static void DrvLoadPortS2Config(void);
static void DrvReinitCommStandaloneConfig(void);
static void ChkSavePortS1MacToNv(void);
static void ValidatePortS1NvMacSettings(void);
static void CheckAlarms(void);

static void ActnetAddressConfig(void);
BYTE fActnetConfigUpdate =FALSE;//To identify actnet config parameters got updated

static void UpdateVendorInfoOnReinit(void);

/*------------------ D A T A   D E F I N I T I O N S ----------------------
   Static and global variables (including const) defined in this file.
   NOTE: Recommend all global variables be declared in a header file.
--------------------------------------------------------------------------*/
#define START         NULL    // For now.
// The "main" header needs to be set for 32-byte alignment to force it to
// have the ulSignature as the first thing in the given s-record.  If the
// gcc compiler changes the number of bytes it is putting into an s3 recored
// it will make the ".driver" file creation fail with getver.awk not producing
// anything in globals.awk.
const HEADER1 __attribute__((aligned(32))) headerMain = {
      /* ulSignature    = */ 0x4E49414D,    /* "MAIN" -- fixed for little-endian */
      /* wCRC16         = */ 0, /* CRC is uncalculated */
      /* wID            = */ REVERSE_WORD(MAIN_ID),
      /* version.bMajor = */ { MAIN_MAJOR_VERSION,        /* Version = 05.00.0001 */
      /* version.bMinor = */   MAIN_MINOR_VERSION,
      /* version.wBuild = */   REVERSE_WORD(MAIN_BUILD) },
      /* CRCSize        = */ 0, /* CRC is uncalculated */
      /* BlockSize      = */ (size_t)0,      // determined by the linker
      /* szName[]       = */ MAIN_NAME,
      /* EntryPoint     = */ START, /* Exec entry point offset - defined in begin.s */
      /* HeaderVersion  = */ 2,
      /* bFlags1        = */ HEADER_PIC,  // indicates position indepedent code
      /* abSpare[0]     = */ 0,
      /* abSpare[1]     = */ 0,
  /* See Note 2 below ==> */ EXTRA_RAM+IPDMEMSIZE_TOTAL,
  /* See Note 3 below ==> */ 0x12000 +
                             EXTRA_RAM+IPDMEMSIZE_TOTAL+
                             offsetof(NVSTORE,awDatabaseSpace),
  /* See Note 4 below ==> */ sizeof(CPRUN),  // size of each CPRUN (minus overhead)
                             (ULONG)&headerDrv,      // Location of module driver RP definition
                             0, //  (ULONG)&awMbMemoryRequirements,   // Individual microblock memory
                                                                      // requirements array.
                             (ULONG)0,     // ulCodeBaseAddr - linked location
                             __DATE__                // Build date
};

// This is a dummy header for BOOT.
const HEADER headerBoot = {
      /* ulSignature    = */ 0x544F4F42,    /* "BOOT" -- fixed for little-endian */
      /* wCRC16         = */ 0,
      /* wID            = */ REVERSE_WORD(BOOT_ID),
      /* version.bMajor = */ { 0,               /* Version = 0.01:001*/
      /* version.bMinor = */   1,
      /* version.wBuild = */   REVERSE_WORD(1) },
};

const DRVHEADER headerDrv =
{
   0,                 // wCRC;               // not used
   sizeof(DRVHEADER), // dwLength;           // total length sizeof(DRVHEADER) + ...
   DRVNAME,           // szName[16];         // DRV name string
   DRVVERSION,        // szVersion[16];      // DRV version string
   0L,                // timeStamp;          // time DRV was created
   DRVTYPE,           // sParType;           // Hardware type
   0L,                // uSequenceSize;      // not used
   0L,                // uSequenceOffset;    // not used
   0L,                // uXDataSize;         // not used
   0L,                // uXDataOffset;       // not used
   0L,                // uTTableSize;        // not used
   0L,                // uTTableOffset;      // not used
   sizeof(DRVPARS),   // sizePars;           // module driver parameter size
   sizeof(DRVSTAT),   // sizeStat;           // module driver status size
   sizeof(DRVVARS),   // sizeVars;           // module driver vars size
   sizeof(DRVHIST),   // sizeHist;           // module driver hist size
};

// Some module drivers might reserve a serial port for exclusive use.
ULONG ulReservedPort;

// IP Settings read from environment variables - IP address, subnet mask, default gateway, UDP Port.
IPPARS ippars;


// EE data

#ifndef _TARG_ARM
BYTE abDrvEECORE[EE_CFGDATA_SIZE] =
{
0x07, 0xd3, 0x01, 0x0a, 0x20, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, // crc size - calculated
            0x00, 0x00, // crc - calculated
                        LOBYTE(501),HIBYTE(501), // product type (Flip for little endian)
                                    0x00, 0x24,   // board type = 36
                                                  0x00, 0x04, // board rev = 4
                                                              0x07, 0x00, 0x00, 0x00, // hw present  (Flip for little endian)
                                                                                      0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x80, // RAM1 size = 16 MB
            0x00, 0x00, // RAM2 size
                        0x00, 0x40, // FLASH size = 8 MB
                                    0x00, 0x09, // FLASH type
                                                  0x00, 0x00, // RAM base
                                                              0x00, 0x00, // RAM2 base
                                                                          0x00, 0x00, // CPU type
                                                                                      0x00, 0x4b, 0x00, 0x00, // Osc Freq
            0x00, 0x0A, // PLL multiplier
                        0x02, 0xfa, 0xf0, 0x80, // memory bus speed
                                                  // device irq/cs table
                                                  0x01, 0x01, 0x03, // arcnet
                                                                    0x02, 0x02, 0x04, // can
                                                                                      0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

BYTE abDrvEEBASE[EE_CFGDATA_SIZE] =
{
0x07, 0xd3, 0x01, 0x0a, 0x20, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, // crc size - calculated
            0x00, 0x00, // crc - calculated
                        LOBYTE(501),HIBYTE(501), // product type (Flip for little endian)
                                    0x00, 0x36,   // board type = 37
                                                  0x00, 0x01, // board rev = 5
                                                              0x07, 0x00, 0x00, 0x00, // hw present  (Flip for little endian)
                                                                                      0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x08, // bo's
      0x0C, // ai's
            0x08, // ao's
                  0x0C, // bi's
                        0x06, // mx expanders supported
                              0x01, // first mx expander
                                    0x06, // CAN expanders supported
                                          0x01, // first CAN expander
                                                  // device irq/cs table
                                                  0x04, 0x05, 0x00, // 100BT
                                                                    0x05, 0x04, 0x00, // 10BT
                                                                                      0x03, 0x03, 0x00, // ST7
      0x06, 0x00, 0x06, // device read DI
                        0x07, 0x00, 0x07, // device read HOA status/write MUX
                                          0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#else
BYTE abDrvEECORE[EE_CFGDATA_SIZE];
BYTE abDrvEEBASE[EE_CFGDATA_SIZE];
#endif

BYTE  bHomeNetworkIndex = NG_IP_NETWORK_IDX;

// Local copy of IP addressing information.
static ULONG ulMyIPAddr;
static ULONG ulMySubnetMask;
static ULONG ulMyGatewayIPAddr;
static ULONG ulMyDNSServer1;
static ULONG ulMyDNSServer2;
static WORD  wMyUdpPort;
static WORD  wMySecUdpPort;

// IP
static BOOL fIPStarted;

// Integration point limit
static USHORT usIntegrationPointsAllowed;

static DRVPARS *pDRVPARS;
static DRVSTAT *pDRVSTAT;
static DRVVARS *pDRVVARS;
static DRVHIST *pDRVHIST;

#define NEWLINE "\r\n"
static CHAR *pchEthernetAddr;   // Ethernet MAC address.

BOOL fLoadDefaultNetNumbers;

BOOL bEthPortConfig;

// These structs eventually go away when the infrastructure is in place
// to drive these (and other) configuration options via the front end.
//ROUTER_PARAMS LocalRouterParams =
//   {
//         .wMyUdpPort             = 47808,
//         .acSerialNumber         = "",
//         .acEthAdapter           = DEFAULT_ETHERNET_NAME;
//   };
ROUTER_PARAMS LocalRouterParams;
// The initialization above was moved to root().

DEVICE_COLOR_CACHING_DATA *pDeviceColorCachingData;
DEVICE_COLOR_CACHING_DATA DeviceColorCachingData;
CACHEROUTE CachePars;

#ifdef BP_VENDOR_ID
#error BP_VENDOR_ID must not be defined!
#else
unsigned int BP_VENDOR_ID;   //now the vendor id will be set dynamically in the drv_fwex.c
char BP_VENDOR_STR[50];
#endif
char MODELNAMESTR[50];

static BYTE           bSensorFail = 0x00;                   // indicates communication failure of RS Sensor(s)
#define ADDR_RS_PRO 5
static RNET_LS2_INFO * apRnetDevice[MAX_RS + ADDR_RS_PRO];  // pointers to possible RSnet sensors;
static RNET_LS2_INFO * pRnetMaster;                         // pointer to default RSnet sensor, Pro if found;
static WORD wRsStatGfbInstance;                             // The first GFB instance that contains the RS microblock.
static WORD wRsfStatGfbInstance;                            // The first GFB instance that contains the RSF microblock.

// This pointer is for debug purposes only. The WebCTRL console can be used to set
// the last two bytes as ipdpars/ipdparspad to a known value to validate correct parameter
// space alignment for all pars above it. (Also remember to un-comment where
// the pointer gets set in the root task.)
//USHORT *pusIpdParsPadDebug;
//USHORT *pusIpdStatPadDebug;

// Module driver supports a user-specified timeout on BMS port to detect
// when the module is in "standalone" mode (master is not writing to him).
static COMM_STANDALONE_CTRL CommStandaloneCtrl;

static BOOL fNwNumsChanged = TRUE;     //Default value of this variable allows, some boot messages to be printed in first run
static WORD awSavedNwNums[NUM_NETWORK_NUMBERS_IN_USE];


/*------------------ F U N C T I O N S -----------------------------------*/

#define GPIO1_12 (int)((1*32)+12)
/**=== isFPGAinBypass ====================================================

   Read GPIO for FPGA bypass condition.  Not dependable for NGZN families.

   Name        Dir   Description

   Returns: True if GPIO low (bypass == low) otherwise false

   Cautions:

=========================================================================*/
static BOOL isFPGAinBypass(void)
{
   static BOOL fGPIOHasBeenRead = FALSE;
   static BOOL fIsFPGABypass = FALSE;

   if(!fGPIOHasBeenRead)
   {
      fIsFPGABypass = atoi(getenv("ALCENV_FPGA_IN_BYPASS"));
      fGPIOHasBeenRead = TRUE;
      if (fIsFPGABypass == TRUE) 
      {
         Boot(BOOT_DEBUG, (DBG_MSG_STARTUP | DBG_MSG_HW), "%s: Port S1 is available w/o FPGA\n", __FUNCTION__); 
      }
   }

   return fIsFPGABypass;
}

#define FPGA_MISSING (BYTE)(0xFE)
/**=== isPortS1HdwSupported ===============================================

   Validate PortS1 hardware is available on board

   Name        Dir   Description

   Returns: True if PortS1 hardware is on board otherwise false

   Cautions:

=========================================================================*/
static BOOL isPortS1HdwSupported(void)
{
   BOOL fPortS1HdwValid = FALSE;
   BOOL fValueNeedsBootMessage = TRUE;

   // process according to device family
   switch(GetDeviceFamily())
   {
      case DEV_FAMILY_ROUTER:
      case DEV_FAMILY_IC:
         if ( (GetFPGAType() != FPGA_MISSING) || isFPGAinBypass() )
         {
            fPortS1HdwValid = TRUE;
         }
         break; 
      case DEV_FAMILY_NGZN:
      case DEV_FAMILY_NGVAV:
         // The NGZN and NGVAV families have built in bypass when FPGA is missing
         fPortS1HdwValid = TRUE;
         break;
      default:
         Boot(BOOT_LOG_FATAL, (ULONG)0, (ULONG)0, "Invalid Device Family");
         break;
   }

   if (pDRVSTAT)
   {
      if (pDRVSTAT->fHWPortS1Supported != fPortS1HdwValid)
      {
         pDRVSTAT->fHWPortS1Supported = fPortS1HdwValid; 
         Boot(BOOT_DEBUG, (DBG_MSG_STARTUP|DBG_MSG_HW), "PortS1: pDRVSTAT->fHWPortS1Supported=%d\n", pDRVSTAT->fHWPortS1Supported);
      }
      else
      {
         fValueNeedsBootMessage = FALSE;
      }
   }

   if (fValueNeedsBootMessage)
   {
      Boot(BOOT_DEBUG, (DBG_MSG_STARTUP|DBG_MSG_HW), "%s: fPortS1HdwValid=%d\n", __FUNCTION__, fPortS1HdwValid); 
   }
   return fPortS1HdwValid;
}

/**=== isArcnetHdwSupported =============================================

   Validate Arcnet hardware is available on board

   Name        Dir   Description

   Returns: True if Arcnet hardware is on board otherwise false

   Cautions:

=========================================================================*/
static BOOL isArcnetHdwSupported(void)
{
   BOOL fArcnetHdwValid = FALSE;
   static BOOL fBootMessagePosted = FALSE;

   // process according to device family
   switch(GetDeviceFamily())
   {
      case DEV_FAMILY_ROUTER:
      case DEV_FAMILY_IC:
      case DEV_FAMILY_NGZN:
      case DEV_FAMILY_NGVAV:
         // The devices do not have ARCnet with an invalid or missing FPGA
         if(GetFPGAType() != FPGA_MISSING)
         {
            fArcnetHdwValid = TRUE;
         }
         break;
      default:
         Boot(BOOT_LOG_FATAL, (ULONG)0, (ULONG)0, "Invalid Device Family");
         break;
   }

   if (pDRVSTAT)
   {
      if (pDRVSTAT->fHWArcnetSupported != fArcnetHdwValid)
      {
         pDRVSTAT->fHWArcnetSupported = fArcnetHdwValid; 
         Boot(BOOT_DEBUG, (DBG_MSG_STARTUP|DBG_MSG_HW), "PortS1: pDRVSTAT->fHWArcnetSupported=%d\n", pDRVSTAT->fHWArcnetSupported);
      }
      // else
      // {
      //???    fBootMessagePosted = FALSE;
      // }
   }

   if (!fBootMessagePosted)
   {
      Boot(BOOT_DEBUG, (DBG_MSG_STARTUP|DBG_MSG_NETWORK), "%s: fArcnetHdwValid=%d\n", __FUNCTION__, fArcnetHdwValid);
      fBootMessagePosted = TRUE;
   }
   return fArcnetHdwValid;
}

/*==== DrvIsPortS1SupportedByProdType ===================================== 
 
   Checks if the board's product type normally supports Port S1 
 
   Returns: True if Product Type normally supports H/W Port S1 otherwise false

   Cautions:
      This should only be called via Local Access (data_functions.c)
 
=========================================================================*/
extern BOOL DrvIsPortS1SupportedByProdType(void)
{
   WORD wProductType = (WORD)Boot(BOOT_GET_PRODUCT_TYPE);
   BOOL fProdTypeSupportsPortS1 = FALSE;

   switch(wProductType)
   {
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_HIGH_TIER_ROUTER_CARRIER:
      case GEN5_HIGH_TIER_ROUTER_ALC:
      case GEN5_BASE_TIER_ROUTER_CARRIER:
      case GEN5_BASE_TIER_ROUTER_ALC:
      case GEN5_MECPU_ALC:
      case GEN5_MECPU_CARRIER:
      case GEN5_MECPU_OEM:
      case GEN5_MECPU_PTR:
      case GEN5_MECPU_OFHI:
      case GEN5_MECPU_OFHI_A:
      case GEN5_NON_ROUTER_ALC:
      case GEN5_TV_MPCXP1628:
      case GEN5_TV_MPCXP1628_NR:
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_OF1628:
      case GEN5_OF1628_NR:
      case GEN5_OF028_NR:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
      /* Next Gen Zone Controllers  */
      case GEN5_OF683XT_E2:
      case GEN5_OF683T_E2:
      case GEN5_OF683_ARC:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_TV_UC683_ARC:
      case GEN5_TV_UC683T:
      case GEN5_TV_FC:
      case GEN5_TV_UV:
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UCXP683T_V:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
      case GEN5_NGZNDEV:
      case GEN5_TV_SQ_PSM:
      /* Next Gen VAV Controllers  */
      case GEN5_TV_VVTZC_E2:
      case GEN5_TV_VVTBP_E2:
      case GEN5_TV_VAVB3_E2:
      case GEN5_SS_342A_E2:
      case GEN5_OF342_ARC:
      case GEN5_TV_VVTZC:
      case GEN5_TV_VVTBP:
      case GEN5_TV_VAVB3:
      case GEN5_SS_342A:
      case GEN5_NGVAVDEV:
         fProdTypeSupportsPortS1 = TRUE;
         break;

      /* Next Gen Zone Controllers  */
      case GEN5_OF253A_E2:
      case GEN5_OF561_E2:
      case GEN5_OF683_E2:
      case GEN5_TV_UC561:
      case GEN5_TV_UC253_V:
      /* Next Gen VAV Controllers  */
      case GEN5_OF141_E2:
      case GEN5_OF342_E2:
         fProdTypeSupportsPortS1 = FALSE;
         break;

      default:
         Boot(BOOT_LOG_FATAL, (ULONG)0, (ULONG)0, "Invalid Product Type");
         fProdTypeSupportsPortS1 = FALSE;
         break;
   }

   return (fProdTypeSupportsPortS1);
}

/**=== AssignBACnetDeviceInfo ============================================

   This function reads the value from EEPROM to determine what the model
   configuration is, then assigns the correct string name for the model.

   Name        Dir     Description
   pDevConst   in/out  pointer to the device constants structure
   pDevPars    in/out  pointer to the device parameters structure

   Returns: none

=========================================================================*/
extern void AssignBACnetDeviceInfo(void *pDevConst, void *pDevPars)
{
   if (pDevConst && pDevPars)
   {
      DEVICE_CONST *pLocalDevConst = (DEVICE_CONST *)pDevConst;
      DEVICE_PARS  *pLocalDevPars = (DEVICE_PARS *)pDevPars;
      
      //! Assign property product type from EEmprom
      pLocalDevConst->usEepromProductType = Boot(BOOT_GET_PRODUCT_TYPE);

       switch(pLocalDevConst->usEepromProductType)
       {
          case GEN5_HIGH_TIER_ROUTER_OEM:
          case GEN5_MECPU_OEM:
          case GEN5_LS_1628:
          case GEN5_SS_561TX:
          case GEN5_SS_683:
          case GEN5_SS_683TX:
          case GEN5_SS_683ETX:
          case GEN5_SS_342A_E2:
          case GEN5_SS_342A:
            OmInitAnsiString(&pLocalDevPars->strVendorName, "Automated Logic Corporation", DEFAULT_MAX_STR_LEN); // Vendor_Name
            pLocalDevPars->wVendorId = BP_VENDOR_ID_ALC_OEM;
            pszPortS1Name = OEM_PORTS1_NAME;
            pszPortS2Name = OEM_PORTS2_NAME;
            iPortNames = OEM_PORTS;
            break;
          case GEN5_VLINK_IO_1628U:
            OmInitAnsiString(&pLocalDevPars->strVendorName, "liebert", DEFAULT_MAX_STR_LEN); // Vendor_Name
            pLocalDevPars->wVendorId = BP_VENDOR_ID_LIEBERT;
            pszPortS1Name = OEM_PORTS1_NAME;
            pszPortS2Name = OEM_PORTS2_NAME;
            iPortNames = OEM_PORTS;
            break;

          case GEN5_HIGH_TIER_ROUTER_ALC:
          case GEN5_BASE_TIER_ROUTER_ALC:
          case GEN5_MECPU_ALC:
          case GEN5_NON_ROUTER_ALC:
          case GEN5_MECPU_OFHI:
          case GEN5_MECPU_OFHI_A:
          case GEN5_OF1628_NR:
          case GEN5_OF028_NR:
          case GEN5_OF1628:
          case GEN5_NGZNDEV:
          case GEN5_OF683XT_E2:
          case GEN5_OF683T_E2:
          case GEN5_OF561T_E2:
          case GEN5_OF253T_E2:
          case GEN5_OF253A_E2:
          case GEN5_OF683_ARC:
          case GEN5_OF561_E2:
          case GEN5_OF683_E2:
          case GEN5_OF_PSM:
          case GEN5_OF_OPT:
          case GEN5_OF141_E2:
          case GEN5_OF342_E2:
          case GEN5_OF342_ARC:
          case GEN5_NGVAVDEV:
            OmInitAnsiString(&pLocalDevPars->strVendorName, "Automated Logic Corporation", DEFAULT_MAX_STR_LEN); // Vendor_Name
            pLocalDevPars->wVendorId = BP_VENDOR_ID_ALC;
            pszPortS1Name = ALC_CAR_PORTS1_NAME;
            pszPortS2Name = ALC_CAR_PORTS2_NAME;
            iPortNames = ALC_CARRIER_PORTS;
            break;

          case GEN5_HIGH_TIER_ROUTER_CARRIER:
          case GEN5_BASE_TIER_ROUTER_CARRIER:
          case GEN5_MECPU_CARRIER:
          case GEN5_TV_MPCXP1628:
          case GEN5_TV_MPCXP1628_NR:
          case GEN5_TV_UCXP683T:
          case GEN5_TV_UCXP683T_V:
          case GEN5_TV_UC683T:
          case GEN5_TV_UC683_ARC:
          case GEN5_TV_FC:
          case GEN5_TV_UV:
          case GEN5_TV_UC561:
          case GEN5_TV_UC253_V:
          case GEN5_TV_PSM:
          case GEN5_TV_OPT:
          case GEN5_TV_SQ_PSM:
          case GEN5_TV_VVTZC_E2:
          case GEN5_TV_VVTBP_E2:
          case GEN5_TV_VAVB3_E2:
          case GEN5_TV_VVTZC:
          case GEN5_TV_VVTBP:
          case GEN5_TV_VAVB3:
            OmInitAnsiString(&pLocalDevPars->strVendorName, "Carrier Corporation", DEFAULT_MAX_STR_LEN); // Vendor_Name
            pLocalDevPars->wVendorId = BP_VENDOR_ID_CARRIER;
            pszPortS1Name = ALC_CAR_PORTS1_NAME;
            pszPortS2Name = ALC_CAR_PORTS2_NAME;
            iPortNames = ALC_CARRIER_PORTS;
            break;

          case GEN5_MECPU_PTR:
            OmInitAnsiString(&pLocalDevPars->strVendorName, "Production Test Router", DEFAULT_MAX_STR_LEN); // Vendor_Name
            pLocalDevPars->wVendorId  = BP_VENDOR_ID_ALC;
            pszPortS1Name = ALC_CAR_PORTS1_NAME;
            pszPortS2Name = ALC_CAR_PORTS2_NAME;
            iPortNames = ALC_CARRIER_PORTS;
            break;

          default:
            OmInitAnsiString(&pLocalDevPars->strVendorName, "Invalid", DEFAULT_MAX_STR_LEN); // Vendor_Name
            pLocalDevPars->wVendorId  = BP_VENDOR_ID_ALC;
            pszPortS1Name = ALC_CAR_PORTS1_NAME;
            pszPortS2Name = ALC_CAR_PORTS2_NAME;
            iPortNames = ALC_CARRIER_PORTS;
            break;
       }
       BP_VENDOR_ID = pLocalDevPars->wVendorId;
       memcpy(BP_VENDOR_STR, &(pLocalDevPars->strVendorName.abStr), pLocalDevPars->strVendorName.vss.usCurr);
       Boot(BOOT_DEBUG,(DBG_MSG_TRACE|DBG_MSG_STARTUP),"%s: BP_VENDOR_ID=%i, wVendorId=%i usEepromProductType=%d",
             __FUNCTION__, BP_VENDOR_ID, pLocalDevPars->wVendorId, pLocalDevConst->usEepromProductType);
   }
}

/*========================================================================
   MODULE DRIVER SPECIFIC HANDLERS
=========================================================================*/

/**=== DrvLoadDefaultPars ================================================

   This function loads default module driver parameters into the
   supplied parameter space.

   Name        Dir   Description
   p           In    Pointer to DRVPARS.

   Returns:  None.

   Cautions: None.

=========================================================================*/
extern void DrvLoadDefaultPars(void *p, USHORT usEepromProductType)
{
   CHAR szString[16];
   CHAR szNewString[16]={0};
   INT i,j=0;
   DRVPARS *pDrvPars = p;

   if (pDrvPars)
   {
      if (pDrvPars->ipnat.wNatGlobalUdpPort < 1024 || pDrvPars->ipnat.wNatGlobalUdpPort > 65535)
      {
         pDrvPars->ipnat.wNatGlobalUdpPort = DEFAULT_GLOBAL_UDP_PORT;
      }
      if (pnvSystemMemory)
      {
         if (ValidateBaudRate(nvhw.ulPortS2BaudRate) == FALSE)
         {
            nvhw.ulPortS2BaudRate = DRV_MSTP_BAUD_76800;
         }
         pDrvPars->ulPortBaud[PORTS2] = nvhw.ulPortS2BaudRate;
         if (ValidateBaudRate(nvhw.ulPortS1BaudRate) == FALSE)
         {
            nvhw.ulPortS1BaudRate = DRV_MSTP_BAUD_76800;
         }
         pDrvPars->ulPortBaud[PORTS1] = nvhw.ulPortS1BaudRate;
         if (CheckPortS1Address(ROTARY_PROTO_MSTP,nvhw.bPortS2MACAddr) == FALSE)  //@TODO: We should change the name CheckPortS1Address as it can be used for both port
         {
            nvhw.bPortS2MACAddr = 0;
         }
         ValidatePortS1NvMacSettings();
         
         if(nvhw.bPortS1Protocol > ROTARY_PROTO_MODBUS)
         {
            nvhw.bPortS1Protocol = ROTARY_PROTO_NONE;
         }
         
         if(nvhw.bPortS2Protocol > TPI_DLL_MODBUS)
         {
            nvhw.bPortS2Protocol = TPI_DLL_DISABLED;
         }
         
         pDrvPars->bPortMACAddr[PORTS1]=  nvhw.bPortS1MSTPMacAddr;
         pDrvPars->bPortMACAddr[PORTS2] = nvhw.bPortS2MACAddr;
         pDrvPars->bMstpAutobaud[PORTS1] = nvhw.bPortS1MstpAutobaud ;
         pDrvPars->bMstpAutobaud[PORTS2] = nvhw.bPortS2MstpAutobaud;
         
         /* Load Default Protocol on PortS1 based on Product Variant */
         nvhw.bPortS1Protocol = DrvGetDefaultProtocolOnPortS1(usEepromProductType);
         if(!IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
         {
            // For the boards doesn't have rotary switch to select protocol and mac address. 
            // We are loading last saved Mac address and Protocol on Port S1 from ENVIRONMENT Variable
            // This function should not be used for the boards Support rotary Switch
            UpdatePortS1File(NULL);
         }
      }
      else
      {
         pDrvPars->ulPortBaud[PORTS1]=DRV_MSTP_BAUD_76800;
         pDrvPars->ulPortBaud[PORTS2]=DRV_MSTP_BAUD_76800;
      }

      for(j=MIN_ACT_EXPANDER_SUPPORTED; j<=MAX_ACT_EXPANDER_SUPPORTED;j++)
      {
         memset(&pDrvPars->ACTnetPars[j-1].strDevDescription.abStr, 0, DEFAULT_MAX_STR_LEN);
         sprintf(szNewString,"Device_%d",j);
         OmInitAnsiString(&pDrvPars->ACTnetPars[j-1].strDevDescription, szNewString, DEFAULT_MAX_STR_LEN); // Object name
      }

      // initialize module generated alarms parameters
      for (i=0; i<MAX_DRIVER_ALARM_OBJS; i++)
      {
         pDrvPars->aDriverAlarmPars[i].id = MAKE_OBJID(OBJTYPE_BV, i+4001000);
         switch (i)
         {
            case MA_DEAD_MODULE: snprintf(szString, sizeof(szString), "Dead Module");
               break;

            case MA_FLOW_CONTROL_TROUBLE: snprintf(szString, sizeof(szString), "Flow Control");
               break;

            case MA_REHEAT_VALVE_TROUBLE: snprintf(szString, sizeof(szString), "Reheat Alarm");
               break;

            default:  snprintf(szString, sizeof(szString), "Unknown Alarm");
               break;
         }

         OmInitAnsiString(&pDrvPars->aDriverAlarmPars[i].strName, szString, DEFAULT_MAX_STR_LEN); // Object name
         OmInitAnsiString(&pDrvPars->aDriverAlarmPars[i].strDescription, szString, DEFAULT_MAX_STR_LEN); // Description

         pDrvPars->aDriverAlarmPars[i].bAlarmValue = TRUE; // this is in alarm when the state is TRUE;
         pDrvPars->aDriverAlarmPars[i].irp.ulTimeDelay = 10; // seconds of delay before event sent
         pDrvPars->aDriverAlarmPars[i].irp.ulNotificationClass = 1;
         pDrvPars->aDriverAlarmPars[i].irp.abEventEnable[1] = BACnetEventTransitionBits__to_offnormal; // we would want to know this
      }

      if (usEepromProductType == GEN5_HIGH_TIER_ROUTER_CARRIER ||
          usEepromProductType == GEN5_MECPU_OEM ||
          usEepromProductType == GEN5_MECPU_CARRIER ||
          usEepromProductType == GEN5_BASE_TIER_ROUTER_CARRIER)
      {
         // Flag network numbers to be set to defaults.
         fLoadDefaultNetNumbers = TRUE;
      }
   }
}

/**=== DrvResetIO ========================================================

   This function would reset I/O. This function is only here to satisfy
   the external reference made by ix86/shared/execinit.c. If this
   dependency is removed, this function should be deleted.

   Name        Dir   Description
   NA          NA    NA

   Returns:  None.

   Cautions: None.

=========================================================================*/
extern void DrvResetIO(void)
{
}

// ---- GFB handlers -------------------------------------------------------

/**=== DrvCpReset ========================================================

   This function is called my pi/shared/cp.c during the CP reset pass.
   This driver does nothing in this state. The return value is ignored
   by the caller.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  FALSE.

   Cautions: None.

=========================================================================*/
extern BOOL DrvCpReset(CPRUN *pcp)
{
   return FALSE;
}

/**=== DrvCpReinitBegin ==================================================

   This function resets the trend counters and calls the IpdCacheInit()
   function if it is included in the build.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  TRUE. (The return value is ignored by the caller, pi/shared/cp.c.)

   Cautions: None.

=========================================================================*/
extern BOOL DrvCpReinitBegin(CPRUN *pcp)
{
   if (pcp->pCpStat)
   {
      // Reset trend counters
      pcp->pCpStat->wNumTrendsRequested = 0;
      pcp->pCpStat->wNumTrendsActive    = 0;
      pcp->pCpStat->bEndianness = LITTLE_ENDIAN_BYTE_ORDER; // indicates little-endian byte order to System Touch
   }

   return TRUE;
}

/**=== DrvCpReinitEnd ====================================================

   This function is called my pi/shared/cp.c during the CP reinit pass.
   This driver does nothing in this state. The return value is ignored
   by the caller.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  TRUE.

   Cautions: None.

=========================================================================*/
BOOL DrvCpReinitEnd(CPRUN *pcp)
{
   return TRUE;
}

/**=== DrvCpExit =========================================================

   This function is called my pi/shared/cp.c at the end of each CP time
   slice.
   This driver does nothing in this state. The return value is ignored
   by the caller.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  FALSE.

   Cautions: None.

=========================================================================*/
extern BOOL DrvCpExit(CPRUN *pcp)
{
   return FALSE;
}

/**=== DrvCpEnter ========================================================

   This function is called my pi/shared/cp.c at the beginning of each
   CP time slice.
   This driver does nothing in this state. The return value is ignored
   by the caller.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  FALSE.

   Cautions: None.

=========================================================================*/
extern BOOL DrvCpEnter(CPRUN *pcp)
{
   return FALSE;
}

/**=== DrvCpBegin ========================================================

   This function calls the DrvUpdateStatus() function every five seconds.
   It also calls the ccnCpBegin() if the CARRIER_PHASE_2 code is included
   in the build.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  TRUE. (The return value is ignored by the caller, pi/shared/cp.c.)

   Cautions: None.

=========================================================================*/
extern BOOL DrvCpBegin(CPRUN *pcp)
{
   static SHORT sTimer, sTrendUpdateTimer;

   if (IS_MODULE_DRIVER(pcp))
   {
      // Do this once every 5 seconds  ...
      if (UtTimerPeriod(&sTimer, NETWORK_STATISTICS_UPDATE_RATE))
      {
         NetworkStatisticsUpdate(&pDRVSTAT->EthernetStatistics,      // pointer to Ethernet and IP statistics
                                 &pDRVSTAT->ARCnetStatistics,        // pointer to ARCnet statistics
                                 &pDRVSTAT->MSTP0Statistics,         // pointer to MSTP0 statistics
                                 &pDRVSTAT->MSTP1Statistics,         // pointer to MSTP1 statistics
                                 &pDRVSTAT->RoutedPacketsStatistic); // pointer to Router statistics

         IpAddressChangeover();
         IpChangeFromNetworkPortObject();
      }

      // Update driver trends every one second.
      if (UtTimerPeriod(&sTrendUpdateTimer, NETWORK_STATISTICS_DRIVER_TREND_UPDATE_RATE))
      {
         NetworkStatisticsDriverTrendUpdate(&pDRVSTAT->EthernetStatistics,      // pointer to Ethernet and IP statistics
                                            &pDRVSTAT->ARCnetStatistics,        // pointer to ARCnet statistics
                                            &pDRVSTAT->MSTP0Statistics,         // pointer to MSTP0 statistics
                                            &pDRVSTAT->MSTP1Statistics,         // pointer to MSTP1 statistics
                                            &pDRVSTAT->RoutedPacketsStatistic); // pointer to Router statistics
      }

      UpdateBACnetFirewallStatus(&((DRVPARS *)(pcp->pPars))->bfPars,
                                 &((DRVSTAT *)(pcp->pStat))->bfStat,
                                 &((DRVVARS *)(pcp->pVars))->bfVars);

   }

   return TRUE;
}

/**=== DrvCpEnd ==========================================================

   The IpdCacheUpdate() function is called, for non-driver CPs, if
   SUPPORTS_BACNET_OBJ_MAPPING is supported in the build.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  TRUE. (The return value is ignored by the caller, pi/shared/cp.c.)

   Cautions: None.

=========================================================================*/
extern BOOL DrvCpEnd(CPRUN *pcp)
{
   return TRUE;
}

// ---- I/O handlers -------------------------------------------------------

/**=== DrvZone ===========================================================

   This function is used by the zone MB.

   Name        Dir   Description
   pcp         NA    Not used.

   Returns:  FALSE;

   Cautions: None.

=========================================================================*/
BOOL DrvZone(CPRUN *pcp)
{
   return FALSE;
}

// ---- Driver handlers ------------------------------------------------------

/**=== DrvReset ==========================================================

   Called once at power up, or at module reset.   This function assumes
   that it is it's responsibility to create system resources and initialize
   everything that it is responsible for.  All static variables are assumed
   to be zero.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  TRUE. (The return value is ignored by the caller, pi/shared/cp.c.)

   Cautions: None

=========================================================================*/
extern BOOL DrvReset(CPRUN *pcp)
{
   int                  i;
   float          arTabData[13];
   short      *psTemp;
   WORD wProductType = (WORD)Boot(BOOT_GET_PRODUCT_TYPE);
   DRVSTAT              *pStat = (DRVSTAT*)(pcp->pStat);
   IPD_PROTOCOL_STATUS  *pProtocolStatus;
   BYTE     fFlowTubeStatus[2] = {0};

extern void RegisterFlowTubePtr(BYTE *pFlowRev);
   
   RegisterFlowTubePtr(pStat->fFlowTubesReversed);

   //Read T&B calibration parameters
   ReadFullTBCalibData(arTabData, sizeof(arTabData));
/*
   // We don't raise alarm here
   // At this point, we are not sure Airflow microblock is used in control program
   if(FALSE == ReadFullTBCalibData(arTabData, sizeof(arTabData)))
   {
      SetFlowControlError(FLOW_TBDATA_READ_FAIL, 0 );       // 0 is dummy value
   }
*/
   //Update T&B calibration parameters
   for(i=0; i<12; i++)
      pStat->arTabData[i] = arTabData[i];  // saved TAB constants

   //Read Flow input tubes status of onboard and zasf
   TBFile_ReadTubesStat(fFlowTubeStatus);

   pStat->fFlowTubesReversed[0] = fFlowTubeStatus[0];
   pStat->fFlowTubesReversed[1] = fFlowTubeStatus[1];

   psTemp = (SHORT *)(&arTabData[12]);
   pStat->asTabAutoZero[0] = *psTemp;
   psTemp++;
   pStat->asTabAutoZero[1] = *psTemp;
   
   // Service driver specific alarms.
   ModuleAlarmDriverService(CPM_PUPINIT, pcp);

   // set module driver color
   CpSetColor(pcp, CPC_RELINQUISH, 0);
   CpSetColor(pcp, CPC_WHITE, CPC_DEFAULT_PRIORITY);

   // Refresh network numbers.
   RefreshNetworkNumbers();

   // Set the protocol count for property page display logic.
#ifndef IPD_NULL
   // This includes available BACnet data links.
   if(IsFeatureSupported(ROUTING_SUPPORTED))
   {
         pStat->bIpdStatusCount = NUM_DATALINKS;
   }
   else
   {
      pStat->bIpdStatusCount = NUM_DATALINKS_NON_ROUTER;
   }
   if(IpdIsProductSupportMODBUS())
   {
      pStat->bIpdStatusCount += NUM_IPDS;
      if(!DrvIsHWPortS2Supported())
         pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
      if(!DrvIsHWPortS1Supported())
         pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
   }

   if(!IsFeatureSupported(MSTP_SUPPORT) || (!DrvIsHWPortS1Supported()))
         pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
   if(!IsFeatureSupported(ARCNET_SUPPORT) || !isArcnetHdwSupported())
         pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
#else
   // If the null ipd has been included, do not display
   // protocol status for any ipd's.
   pStat->bIpdStatusCount = NUM_DATALINKS;
#endif

   // Initialize IPD protocol status structure for BACnet.
   for (i = 0, pProtocolStatus = pStat->aIpdStatus;
        i < NUM_DATALINKS;
        i++, pProtocolStatus++)
   {
      memcpy(pProtocolStatus->achSignature, ExecAddLoadOffset ((void*)aszBACnetProtocolStatusStrings[BPSS_SIGNATURE]), DIM (pProtocolStatus->achSignature));
      memcpy(pProtocolStatus->achProtocolVersion, ExecAddLoadOffset ((void*)aszBACnetProtocolStatusStrings[BPSS_VERSION]), DIM (pProtocolStatus->achProtocolVersion));
      memcpy(pProtocolStatus->achImplementationVersion, ExecAddLoadOffset ((void*)aszBACnetProtocolStatusStrings[BPSS_DATALINK_0+i]), DIM (pProtocolStatus->achImplementationVersion));

      switch (i)
      {
         case 0: // IP Port
            pProtocolStatus->bProtocolPort = IPD_GIGE;
            break;

         case 1: // Port S1 for MSTP
            if (!DrvIsHWPortS1Supported() || !IsFeatureSupported(MSTP_SUPPORT))
            {
               --pProtocolStatus;
               continue;
            }
            else
            {
               pProtocolStatus->bProtocolPort = ((iPortNames == OEM_PORTS)? IPD_BAS :IPD_PORTS1);
            }
            break;

         case 4: // Port S1 for ARCnet
            if (!isArcnetHdwSupported() || !IsFeatureSupported(ARCNET_SUPPORT))
            {
               --pProtocolStatus;
               continue;
            }
            else
            {
               pProtocolStatus->bProtocolPort = ((iPortNames == OEM_PORTS)? IPD_BAS :IPD_PORTS1);
            }
            break;

         case 5:  //BACnet Service port
            pProtocolStatus->bProtocolPort = IPD_SERVICE_PORT;
            break;

         case 2: // Port S2 for MSTP
            //
            // For a non routing device there is no support of Port S2 -
            // reset the Protocol Status pointer and proceed to the next
            // port
            //
            if (!IsFeatureSupported(ROUTING_SUPPORTED))
            {
               --pProtocolStatus;
               continue;
            }
            else
            {
               pProtocolStatus->bProtocolPort = ((iPortNames == OEM_PORTS)? IPD_DEVICE :IPD_PORTS2);
            }
            break;

         case 3: // Ethernet Port
            pProtocolStatus->bProtocolPort = IPD_GIGE;
            break;

         default:
            break;
      }

      pProtocolStatus->bProtocolState = IPD_NOT_RUNNING;
   }

   NetworkStatisticsDriverTrendObjectInitialize((void *)pcp);
   return TRUE;
}

/**=== BpUpdateProtocolStatus ==============================================

   Called by DllMgmtTask() to update WebCTRL's protocol status page.
   DRVSTAT IPD_PROTOCOL_STATUS structure is updated with current driver
   selections.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.



   Returns:  None

   Cautions: None

=========================================================================*/
extern void BpUpdateProtocolStatus(CPRUN *pcp)
{
   BPIP_ENV *pEnv;

   if (pcp == NULL) return;

   int i;
   WORD wProductType = (WORD)Boot(BOOT_GET_PRODUCT_TYPE);

   DRVSTAT *pStat = (DRVSTAT*)(pcp->pStat);
   IPD_PROTOCOL_STATUS *pProtocolStatus;


   // Set the protocol count for property page display logic.
   // If the null ipd has been included, do not display
   // protocol status for any ipd's.
#ifndef IPD_NULL
   // This includes available BACnet data links.
   if(IsFeatureSupported(ROUTING_SUPPORTED))
   {
      pStat->bIpdStatusCount = NUM_DATALINKS;
   }
   else
   {
      pStat->bIpdStatusCount = NUM_DATALINKS_NON_ROUTER;
   }

   if(IpdIsProductSupportMODBUS())
   {
      pStat->bIpdStatusCount += NUM_IPDS;
      if(!DrvIsHWPortS2Supported())
         pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
      if(!DrvIsHWPortS1Supported())
         pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
   }

   if(!IsFeatureSupported(MSTP_SUPPORT) || (!DrvIsHWPortS1Supported()))
      pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
   if(!IsFeatureSupported(ARCNET_SUPPORT) || !isArcnetHdwSupported())
      pStat->bIpdStatusCount = pStat->bIpdStatusCount - 1;
#else
   // If the null ipd has been included, do not display
   // protocol status for any ipd's.
   pStat->bIpdStatusCount = NUM_DATALINKS;
#endif

   // Initialize IPD protocol status structure for BACnet.
   for (i = 0, pProtocolStatus = pStat->aIpdStatus;
        i < NUM_DATALINKS;
        i++, pProtocolStatus++)
   {
      pProtocolStatus->bProtocolState = IPD_NOT_RUNNING;
      switch (i)
      {
         case 0:
            // BACnet/IP
            // Display this protocol status only when IP dll is started, not disabled,
            // and the network number is assigned.
            if   (fIPStarted == TRUE)
            {
               pEnv = BpDllIpFindEnvironment(IP_PRIMARY_PORT);
               if ( pEnv && pEnv->fBpIpDisabled == FALSE )
               {
                  //
                  // If Network Number is set the state should be "Running" otherwise indicate that the port is not routable
                  //
                  pProtocolStatus->bProtocolState = ((NET_NUMBER(NG_IP_NETWORK_IDX)) ? IPD_RUNNING : IPD_RUNNING_NOT_ROUTABLE);
               }
            }
         break;

         case 1:
            // PORTS1 BACnet/MSTP
            if (!DrvIsHWPortS1Supported() || !IsFeatureSupported(MSTP_SUPPORT))
            {
               --pProtocolStatus;
               continue;
            }
            else
            {
               if (fDriverEnabled[PORTS1_MSTP_DRIVER] == TRUE)
               {
                   pProtocolStatus->bProtocolState = IPD_RUNNING;
               }
            }
         break;

         case 2:
            // PORTS2 BACnet/MSTP
            //
            // For a non routing device there is no support of Port S2 -
            // reset the Protocol Status pointer and proceed to the next
            // port
            //
            if (!IsFeatureSupported(ROUTING_SUPPORTED))
            {
               --pProtocolStatus;
               continue;
            }
            else
            {
               if (fDriverEnabled[PORTS2_MSTP_DRIVER] == TRUE)
               {
                   pProtocolStatus->bProtocolState = IPD_RUNNING;
               }
            }
         break;

         case 3:
            // BACnet/Ethernet
            if ( fBpEthernetDisabled == FALSE )
            {
               //
               // If Network Number is set the state should be "Running" otherwise indicate that the port is not routable
               //
               pProtocolStatus->bProtocolState = ((NET_NUMBER(NG_ETHERNET_NETWORK_IDX)) ? IPD_RUNNING : IPD_RUNNING_NOT_ROUTABLE);
            }
         break;

         case 4:
            //  PORTS1 BACnet/ARCNET
            if (!isArcnetHdwSupported() || !IsFeatureSupported(ARCNET_SUPPORT))
            {
               --pProtocolStatus;
               continue;
            }
            else
            {
               if (fArcnetOnPortS1Enabled == TRUE)
               {
                   pProtocolStatus->bProtocolState = IPD_RUNNING;
               }
            }
            break;
         case 5:
            // Bacnet Over Service port
            pEnv = BpDllServPortFindEnvironment(IP_SERV_PORT_NUM);
            if ( pEnv && pEnv->fBpIpDisabled == FALSE )
            {
               //
               // If Network Number is set the state should be "Running" otherwise indicate that the port is not routable
               //
               pProtocolStatus->bProtocolState = ((pEnv->bpaddrThisDll.net) ? IPD_RUNNING : IPD_RUNNING_NOT_ROUTABLE);
            }
            break;
         default:
            break;
      }
   }
}

/**=== DrvReinit =========================================================

   Called at reset time or every time the driver's parameters change.
   This function should not assume that status variables are zero, and
   should not re-create system resources.  Its main function is to check
   for changes that may have made to the drivers parameters.

   Name        Dir   Description
   pcp         In    Pointer to the CPRUN structure.

   Returns:  TRUE. (The return value is ignored by the caller, pi/shared/cp.c.)

   Cautions: None.

=========================================================================*/
extern BOOL DrvReinit(CPRUN *pcp)
{
   ULONG    ulHomeNetworkIDdll;
   BPADDR   HomeAddress;
   static BYTE fEnableDiag;
   static INT bTelnetFileCnt;
   USHORT usVal;

   ULONG tid;

   /*This flag set delay for GetACTnetInformation */
   fActnetConfigUpdate = TRUE;

   // Service driver specific alarms.
   ModuleAlarmDriverService(CPM_REINIT, pcp);

   // Set the peer caching status indication based on current parameters.
   pDRVSTAT->wCacheAllNetworksStatus = pDRVPARS->cachepars.wCacheAllNetworks;


   //
   // Set Max Real Expanders for IO Support
   //
   if (IsFeatureSupported(IO_MICROBLOCK_SUPPORT))
   {
      pDRVSTAT->wMaxRealExpanders[0] = IsFeatureSupported(XNET_SUPPORT)? MAX_XNET_EXPANDERS_SUPPORTED : 0;
      pDRVSTAT->wMaxRealExpanders[1] = MAX_IONET_EXPANDERS_SUPPORTED;
   }
   else
   {
      pDRVSTAT->wMaxRealExpanders[1] = pDRVSTAT->wMaxRealExpanders[0] = 0;
   }

   // Refresh network numbers.
   RefreshNetworkNumbers();

   //
   // IP network.
   // Set the IP Network Number
   BpDllIpChangeNetworkNumber(IP_PRIMARY_PORT, NET_NUMBER(NG_IP_NETWORK_IDX));

   //DE5127: Modstat not updating after changing 2ndary IP network number
   BpDllIpChangeNetworkNumber(IP_SECONDARY_PORT, NET_NUMBER(NG_SEC_IP_NETWORK_1_IDX));

   //
   // Re-initialize the MS/TP and/or ARCNET DLLs
   //
   DllReinitStation();

   //
   // Retrieve the Home Network DLL ID and prep for Home Network determination.
   //
   ulHomeNetworkIDdll = BpNetGetIDdll(pSyspars->awNet[NG_HOME_NETWORK_IDX]);

   //
   // If the Home Network DLL ID is valid (not 0) then call Net Change Local Network
   // to force home network to be desired network number. The first call (with 0 DLL ID)
   // sets the stage for the second call of Net Change Local Network to properly setup the
   // home network for the device.
   //
   if (ulHomeNetworkIDdll)
   {
      BpNetChangeLocalNetwork(0L, pSyspars->awNet[NG_HOME_NETWORK_IDX]);
      BpNetChangeLocalNetwork(ulHomeNetworkIDdll, pSyspars->awNet[NG_HOME_NETWORK_IDX]);
   }

#if 0
   int Status = access(FWDOWNLOAD,F_OK);
   if(Status)
   {
      // For first time the status will be 0 , and it will go to else and perform backup , after backup the file will be deleted .
      //Hence it will give error -1 , When this function is called internally unless there is firmware download .
	   fprintf(myfp, "\n access failed : %d\n",Status);fflush(myfp);
   }
   else
   {
	   fprintf(myfp, "\nMarker file found, Performing Archiving.. \n");fflush(myfp);
	   performDatabaseArchive();
       /*
	   fprintf(myfp, "\n About to launch performDatabaseArchive task\n");fflush(myfp);
	   if(ExecTaskCreate(&tid, "AUTOARCH", performDatabaseArchive, AUTOARCH_PRIORITY, AUTOARCH_STACK_SIZE, 0, NULL) == 0)
	   {
		   fprintf(myfp, "\n performDatabaseArchive task created successfully \n");fflush(myfp);
	   }
	   else{
		   fprintf(myfp, "\n Cannot launch performDatabaseArchive task\n");fflush(myfp);
	   }*/
	  /*
	  ExecReleaseSem(pcp->sidBusy); // releasing lock before backup activity;

	  fprintf(myfp, "\n semaphore with id = %lu is released;\n",pcp->sidBusy);fflush(myfp);

      performDatabaseArchive();

         // lock shouldn't fail here
         // get lock after backup activity is completed;
         if (ExecGetSemTimeout(pcp->sidBusy, MSEC_TO_CLOCKS(2000)))
         {
            // lock accessed
        	 fprintf(myfp, "\n semaphore access gained again thread_id =%p\n",(void*) pthread_self());fflush(myfp);

         }
         else
         {
            // this call does not return
            Boot(BOOT_FATAL, (time_t)0, "Module driver access denied on regaining access.");
         }
         */

	   //ExecTaskCreate(&tid, "ROOT", root, ROOT_PRIORITY, ROOT_STACK_SIZE, 0, NULL);
	   //ExecTaskCreate(&tid, "ARCH", ArchiveTask, ARCH_PRIORITY, CP_STACK_SIZE, 0, NULL);
   }
#endif


   ArchiveReinit(); // For archiver parameter updates.

   // Initialize the IPD interface.
   IpdReinit(pcp);

   // From ToDo TD-CL436, item 3 of the "Firmware Tasks", when receiving UnconfirmedPrivateTransfer
   // vendor ID=24 and Service Number=3 for "I-Am-Your-Color-Cache" message the c-Vu should always ignore
   // the part that sets the clock. (Note, this must be done after IPD reinit because the CCN IPD also
   // sets this but the final word should always be timesync-disabled.)
   ColorDisableTimeSynch();

   // Initialize the Daylight Savings tables for the RTC
   InitDSTTable();

   usVal = (((DRVPARS *)(pcp->pPars))->bNumTelnetSessions);
   if(usVal != bTelnetFileCnt)
   {
      bTelnetFileCnt =usVal;
      DrvTelnetLogCnt(bTelnetFileCnt);
      //If old value  and new value are same don't call the function
      ValidateCapturedFiles();
   }

   // Detect Enable Diag parameter changes.
   if(!fEnableDiag && ((DRVPARS *)(pcp->pPars))->fEnableDiag)
   {
     // Start telnet diagnostic tasks.
     fEnableDiag = TRUE;
     TelnetServerStart(0);
   }
   else if(fEnableDiag && !((DRVPARS *)(pcp->pPars))->fEnableDiag)
   {
      TelnetSetTaskShutdown();
      fEnableDiag = FALSE;
      // Turning this feature OFF requires a reset.
      //Boot(BOOT_RESET, sysstat.time, "DIAG off - resources freed.");
   }

   // Update Whitelist in BACnet Firewall
   ReinitializeBACnetFirewall( &((DRVPARS *)(pcp->pPars))->fConfirmBacnetFirewallSettings,
                               &((DRVPARS *)(pcp->pPars))->bfPars,
                               &((DRVSTAT *)(pcp->pStat))->bfStat,
                               &((DRVVARS *)(pcp->pVars))->bfVars,
                               &ippars);

   DrvReinitCommStandaloneConfig();

   //Function identify actnet config change and update the changes to ioserver
   if(IsFeatureSupported(ACTNET_SUPPORT))
   {
      ActnetAddressConfig();
   }

   UpdateVendorInfoOnReinit();

   return TRUE;
}

// ---- BACnet handlers ------------------------------------------------------

/**=== RefreshNetworkNumbers =============================================
   Called by functions like MyLaunchDlls, DrvReset, and DrvReinit,
   to refresh the network numbers considering any automatic network
   numbering scheme.

   Name        Dir   Description

   Returns:
      void
      Updates network number status members in DRVSTAT.

   Cautions:
      Should be called after pDRVSTAT is initialized.

=========================================================================*/
extern void RefreshNetworkNumbers(void)
{
   BPADDR      addrNv;
   BYTE        bNetworkIndexNv;
   int         idx1;
   int         idx2;
   int         idxWinner;
   WORD        wNetwork;
   WORD        wNewNetworkNumber;
   BOOL        fSaveNVRAM = FALSE;

   if (pSysstat && pSyspars && pSyshist)
   {
      //
      // If the awNet array (Network Numbers) were set via a UI such as WebCtrl/iVu then
      // we need to determine what that UI wants the home network to be prior to doing any
      // Auto Generation of network number so that we have the proper operation profile
      // type
      //
      for (idx1 = 0; idx1 < (NUM_NETWORK_NUMBERS_IN_USE-1); idx1++)
      {
         if (pSyspars->awNet[idx1] && (pSyspars->awNet[idx1] == pSyspars->awNet[NG_HOME_NETWORK_IDX]))
         {
            break;
         }
      }


      // For non-routers clear out unused port's network numbers and
      // set home port network number for auto-generation
      if (IsFeatureSupported(ROUTING_SUPPORTED) == FALSE)
      {
         // Loop through all networks in use
         for(idx2 = 0; (idx2 < NUM_NETWORK_NUMBERS_IN_USE-1); idx2++)
         {
            if(idx2 == bHomeNetworkIndex)
            {
               // This is the home network so set network number to Auto Generate if
               // there currently is no network number and the home network number is
               // unknown
               if (pSyspars->awNet[idx2] == 0)
               {
                  if (pSyspars->awNet[NG_HOME_NETWORK_IDX])
                  {
                     pSyspars->awNet[idx2] = pSyspars->awNet[NG_HOME_NETWORK_IDX];
                  }
                  else
                  {
                     pSyspars->awNet[idx2] = AUTO_NETWORK_NUMBER;
                  }
               }
            }
            else
            {
               // Not the home network so set network number to zero
               pSyspars->awNet[idx2] = 0;
            }
         }
      }

      if ( (idx1 != (NUM_NETWORK_NUMBERS_IN_USE-1)) && (idx1 != bHomeNetworkIndex))
      {
          DrvSetHomeNetworkIndex(idx1);
      }

      // if running serial home router operation correct network numbers as required
      if (bFwexProfileType == SERIAL_HOME_ROUTER_TYPE)
      {
         // if home network is Port S1, it cannot be 0 (disabled)
         if (bHomeNetworkIndex == NG_PORTS1_NETWORK_IDX && pSyspars->awNet[NG_PORTS1_NETWORK_IDX] == 0)
         {
            // automatically change to autogenerated
            pSyspars->awNet[NG_PORTS1_NETWORK_IDX] = AUTO_NETWORK_NUMBER;
         }
         // if home network is Port S2, it cannot be 0 (disabled)
         if (bHomeNetworkIndex == NG_PORTS2_NETWORK_IDX && pSyspars->awNet[NG_PORTS2_NETWORK_IDX] == 0)
         {
            // automatically change to autogenerated
            pSyspars->awNet[NG_PORTS2_NETWORK_IDX] = AUTO_NETWORK_NUMBER;
         }
      }

      wNewNetworkNumber  = AutoNetGetNetwork(BP_VENDOR_ID, NG_IP_NETWORK_IDX);

      if(pSysstat->awNet[NG_IP_NETWORK_IDX] != wNewNetworkNumber)
      {
         pSysstat->awNet[NG_IP_NETWORK_IDX] = wNewNetworkNumber;
         fNwNumsChanged = TRUE;
      }

      wNewNetworkNumber = AutoNetGetNetwork(BP_VENDOR_ID, NG_PORTS1_NETWORK_IDX);

      // DE3584: During the investigation of OPI-BAY8RCQ (DE3584), found that during LGR
      // downloading file to downstream (ofbbc or tv-mpcxp) router by ARCnet, fwex couldn't
      // handle a case that its network number changed during the download and the the
      // download failed.
      // The network number change due to not saving the number was fixed in DE3641.
      // But for the case that the network number changed externally such as by Local Access,
      // still needs the code to check for the change and to update the destination network
      // number, so that the download with segmented messages can be completed.      
      if(wNewNetworkNumber != pSysstat->awNet[NG_PORTS1_NETWORK_IDX])
      {
         BpTSMUpdateDestinationNetworks(pSysstat->awNet[NG_PORTS1_NETWORK_IDX], wNewNetworkNumber);
         pSysstat->awNet[NG_PORTS1_NETWORK_IDX] = wNewNetworkNumber;
      }

      pSysstat->awNet[NG_ETHERNET_NETWORK_IDX] = AutoNetGetNetwork(BP_VENDOR_ID, NG_ETHERNET_NETWORK_IDX);
      pSysstat->awNet[NG_PORTS2_NETWORK_IDX]   = AutoNetGetNetwork(BP_VENDOR_ID, NG_PORTS2_NETWORK_IDX);
      pSysstat->awNet[NG_SEC_IP_NETWORK_1_IDX] = pSyspars->awNet[NG_SEC_IP_NETWORK_1_IDX];

      // Now zero out duplicate network numbers.
      for (idx1 = 0; idx1 < NUM_NETWORK_NUMBERS_IN_USE-1; idx1++)
      {
         // Network number to compare.
         wNetwork = pSysstat->awNet[idx1];

         // Compare against all other networks and declare a winner.
         for ( idx2 = idx1 + 1, idxWinner = idx1;
               idx2 < NUM_NETWORK_NUMBERS_IN_USE;
               idx2++ )
         {
            // Look for duplicate network numbers.
            if (pSysstat->awNet[idx2] == wNetwork)
            {
               // Determine which network gets to keep the network number.
               if (pSyspars->awNet[idx2] == AUTO_NETWORK_NUMBER)
               {
                  // Automatic loser.
                  pSysstat->awNet[idx2] = 0;
               }
               else if (pSyspars->awNet[idxWinner] == AUTO_NETWORK_NUMBER)
               {
                  // New winner!
                  pSysstat->awNet[idxWinner] = 0;
                  idxWinner = idx2;
               }
               else
               {
                  // Both are assigned to be the same network number.
                  // idx2 loses by priority.
                  pSysstat->awNet[idx2] = 0;
               }
            }
         }
      }

      //
      // Save PORTS2 Port Number if it differs from currently stored value.
      //
      if (nvhw.awSavedNetworkNumber[NG_PORTS2_NETWORK_IDX] != pSyspars->awNet[NG_PORTS2_NETWORK_IDX])
      {
         nvhw.awSavedNetworkNumber[NG_PORTS2_NETWORK_IDX] = pSyspars->awNet[NG_PORTS2_NETWORK_IDX];
         fSaveNVRAM = TRUE;
      }

      //
      // Save Secondary IP Port Number if it differs from currently stored value.
      //
      if (nvhw.awSavedNetworkNumber[NG_SEC_IP_NETWORK_1_IDX] != pSyspars->awNet[NG_SEC_IP_NETWORK_1_IDX])
      {
         nvhw.awSavedNetworkNumber[NG_SEC_IP_NETWORK_1_IDX] = pSyspars->awNet[NG_SEC_IP_NETWORK_1_IDX];
         fSaveNVRAM = TRUE;
      }

      //US20136: Display an error message in modstat when it is not possible to autogenerate network numbers for serial BACnet ports
      if (awSavedNwNums[NG_IP_NETWORK_IDX] != pSyspars->awNet[NG_IP_NETWORK_IDX])
      {
         awSavedNwNums[NG_IP_NETWORK_IDX] = pSyspars->awNet[NG_IP_NETWORK_IDX];
         fNwNumsChanged = TRUE;
      }
      if (awSavedNwNums[NG_PORTS1_NETWORK_IDX] != pSyspars->awNet[NG_PORTS1_NETWORK_IDX])
      {
         awSavedNwNums[NG_PORTS1_NETWORK_IDX] = pSyspars->awNet[NG_PORTS1_NETWORK_IDX];
         fNwNumsChanged = TRUE;
      }
      if (awSavedNwNums[NG_PORTS2_NETWORK_IDX] != pSyspars->awNet[NG_PORTS2_NETWORK_IDX])
      {
         awSavedNwNums[NG_PORTS2_NETWORK_IDX] = pSyspars->awNet[NG_PORTS2_NETWORK_IDX];
         fNwNumsChanged = TRUE;
      }

      if(fNwNumsChanged && (pSysstat->awNet[NG_IP_NETWORK_IDX] > MAX_IP_NET_AUTO_NET))
      {
         if( (pSyspars->awNet[NG_PORTS1_NETWORK_IDX] == AUTO_NETWORK_NUMBER) && (pSysstat->awNet[NG_PORTS1_NETWORK_IDX] == 0))
         {
            Boot(BOOT_WARNING, sysstat.time, "Port S1 autogenerate network# failed");
         }

         if( (pSyspars->awNet[NG_PORTS2_NETWORK_IDX] == AUTO_NETWORK_NUMBER) && (pSysstat->awNet[NG_PORTS2_NETWORK_IDX] == 0))
         {
            Boot(BOOT_WARNING, sysstat.time, "Port S2 autogenerate network# failed");
         }
         fNwNumsChanged = FALSE;
      }

      //
      // DE3641: Cannot download via Rnet when using Field Assistant
      // When field assistant issues the CPT22 Firmware Upgrade clear memory,
      // the IP network number is set to 0 which prevents field assistant from
      // being able to communicate to the device again, causing the download
      // to fail.
      // Save the last known home address in non-volatile memory for recovery
      // on next restart to allow field assistant download to complete
      //
      // Save Home Network info if it differs from currently stored value.
      //
      if ((nvhw.bSavedHomeNetworkIndex != bHomeNetworkIndex) ||
            (nvhw.wSavedHomeNetworkStatValue != pSysstat->awNet[bHomeNetworkIndex]))
      {
         nvhw.bSavedHomeNetworkIndex = bHomeNetworkIndex;
         nvhw.wSavedHomeNetworkParsValue = pSyspars->awNet[bHomeNetworkIndex];
         nvhw.wSavedHomeNetworkStatValue = pSysstat->awNet[bHomeNetworkIndex];
         nvhw.wSavedHomeNetworkHistValue = pSyshist->awNet[bHomeNetworkIndex];
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK|DBG_MSG_TRACE),"%s: idx = %d pars = %d stat = %d hist = %d",
               __FUNCTION__, nvhw.bSavedHomeNetworkIndex, nvhw.wSavedHomeNetworkParsValue, nvhw.wSavedHomeNetworkStatValue, nvhw.wSavedHomeNetworkHistValue);
         fSaveNVRAM = TRUE;

      }

      if (fSaveNVRAM)
      {
         //
         // Save nvhw in nvram on disk
         //
         Boot(BOOT_TRACE,"%s: Calling NVRamSave()",__FUNCTION__);
         NVRamSave();
      }


      pSyspars->awNet[NG_HOME_NETWORK_IDX] = pSysstat->awNet[bHomeNetworkIndex];

      if (pdevPars)
      {
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK|DBG_MSG_TRACE),"%s: ID=0x%08X (%ld) name=%s scheme=%d base=%ld",__FUNCTION__,
              (unsigned int)pdevPars->id, OBJID_INST(pdevPars->id), pdevPars->strName.abStr,
               pdevPars->bAutoIdScheme, pdevPars->ulAutoIdBase);
      }

      Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK|DBG_MSG_TRACE),"%s: IP=%d/%d IP2=%d/%d S1=%d/%d ETH=%d/%d S2=%d/%d",__FUNCTION__,
           pSysstat->awNet[NG_IP_NETWORK_IDX], pSyspars->awNet[NG_IP_NETWORK_IDX],
           pSysstat->awNet[NG_SEC_IP_NETWORK_1_IDX], pSyspars->awNet[NG_SEC_IP_NETWORK_1_IDX],
           pSysstat->awNet[NG_PORTS1_NETWORK_IDX], pSyspars->awNet[NG_PORTS1_NETWORK_IDX],
           pSysstat->awNet[NG_ETHERNET_NETWORK_IDX], pSyspars->awNet[NG_ETHERNET_NETWORK_IDX],
           pSysstat->awNet[NG_PORTS2_NETWORK_IDX], pSyspars->awNet[NG_PORTS2_NETWORK_IDX]);

      Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK|DBG_MSG_TRACE),"%s: bFwexProfileType=%d bHomeNetworkIndex=%d HOME=%d/%d",__FUNCTION__,
            bFwexProfileType, bHomeNetworkIndex, pSysstat->awNet[NG_HOME_NETWORK_IDX], pSyspars->awNet[NG_HOME_NETWORK_IDX]);
   }
}

/**=== BpLaunchDlls ======================================================

   This function is only called by the BACnetStart() function in
   ix86/bp/mynode.c and should probably be removed.

   Name        Dir   Description
   NA          NA    NA

   Returns:  None.

   Cautions: None.

=========================================================================*/
extern void BpLaunchDlls(void)
{
}

/**=== MyLaunchDlls ======================================================

   This function is called by the root() function to start the BACnet DLLs
   and the DLL Manager.

   Name        Dir   Description
   NA          NA    NA

   Returns:  None.

   Cautions: None.

=========================================================================*/
static void MyLaunchDlls(void)
{
   ULONG tid;

   //
   // Ensure IP network number is updated/valid prior to starting IP DLL
   //
   RefreshNetworkNumbers();

   //
   // This device is a router, activate IP data link.
   //
   //
   // Launch IP data link.
   //
   BpDllIpStart(IP_PRIMARY_PORT, NET_NUMBER(NG_IP_NETWORK_IDX), ulMyIPAddr, ulMySubnetMask, ulMyGatewayIPAddr, wMyUdpPort);
   fIPStarted = TRUE;

   //
   // Create the downstream dll manager task.
   //
   DllMgmtTaskStart();

   // Reset the BMS BACnet standalone timer at launch time.
   UtTimerSet(&CommStandaloneCtrl.sCommStandaloneTimer, ONE_MINUTE_IN_SECONDS);
}

/**=== ModuleAlarmDriverService ==========================================

   This function services driver specific alarms.

   Name        Dir   Description
   bMode       In    The CPM executation mode.
   pcp         In    Pointer to the CPRUN structure.

   Returns:  None.

   Cautions: None.

=========================================================================*/
void ModuleAlarmDriverService(BYTE bMode, CPRUN *pcp)
{
   UINT  i;
   BOOL  fGfbAlarm;
   WORD  wTextPosition = 0;

   if (!pDRVPARS || !MAX_DRIVER_ALARM_OBJS) return;       // Not initialized yet.

   // Build the dead module alarm if necessary.
   ColorBuildDeadAlarm(&pDRVVARS->aDriverAlarmVars[MA_DEAD_MODULE],
                       &pDRVSTAT->aDriverAlarmStat[MA_DEAD_MODULE]);

   // Flow Control Trouble - begin with the character set 0 -- ansi
   pDRVVARS->aDriverAlarmVars[MA_FLOW_CONTROL_TROUBLE].aMessage[wTextPosition++] = 0;
   pDRVVARS->aDriverAlarmVars[MA_FLOW_CONTROL_TROUBLE].aMessage[wTextPosition] = 0; // null string if not used

   pDRVVARS->aDriverAlarmVars[MA_REHEAT_VALVE_TROUBLE].aMessage[wTextPosition++] = 0;
   pDRVVARS->aDriverAlarmVars[MA_REHEAT_VALVE_TROUBLE].aMessage[wTextPosition] = 0; // null string if not used

   // Check Flow / Reheat valve alarm
   CheckAlarms();

   for (i=0; i<MAX_DRIVER_ALARM_OBJS; i++)
   {
      fGfbAlarm = FALSE;         // No GFB alarm for this driver.
      // Resolve pointers, process this alarm object.
      ModuleAlarm(&pDRVPARS->aDriverAlarmPars[i], &pDRVSTAT->aDriverAlarmStat[i],
                  &pDRVVARS->aDriverAlarmVars[i], &pDRVHIST->aDriverAlarmHist[i],
                  fGfbAlarm, bMode, pcp);
   }
   return;
}

/**==== CheckAlarms =======================================================

   Check for trouble with devices that could lead to inability to
   properly control air flow.

   Name  Dir   Description
   pcp   in    CP pointer that shouldb be pointing to driver.

   Returns:

   Cautions:

=========================================================================*/
void CheckAlarms(void)
{
   MODULE_ALARM_STAT *pMAStat;
   BYTE bLoop;
   BOOL fSpecificEventError;

   // Flow control device alarm and Reheat valve Device alarm.
   for(bLoop=MA_FLOW_CONTROL_TROUBLE; bLoop < MAX_DRIVER_ALARM_OBJS; bLoop++)
   {
      pMAStat = &(pDRVSTAT->aDriverAlarmStat[bLoop]); // bLoop 1 for Flow Control Alarm, and 2 for Reheat Valve alarm.
      if(bLoop == MA_FLOW_CONTROL_TROUBLE)
      {
         fSpecificEventError = (fFlowDeviceAlarmState.usFlowDeviceError != 0x00) ? TRUE : FALSE;
//         Boot(BOOT_WARNING,0,"CheckAlarms: Flow fSpecificEventError %d", fFlowDeviceAlarmState.usFlowDeviceError);
      }
      else if(bLoop == MA_REHEAT_VALVE_TROUBLE)
      {
         fSpecificEventError = (fReheatDeviceAlarmState.ubValveDeviceError != 0x00) ? TRUE : FALSE;
//         Boot(BOOT_WARNING,0,"CheckAlarms: Reheat fSpecificEventError %d", fReheatDeviceAlarmState.ubValveDeviceError);
      }
      pMAStat->bPresentValue = (BYTE)fSpecificEventError; // set or clear the present value
   }
}

/**=== root ==============================================================

   This function is automatically entered after system startup
   and is run by the ROOT task.

   This is the first function to get control after OS startup.

   Name        Dir   Description
   pvTaskPars  In    Not used.

   Returns:  None.

   Cautions: None.

=========================================================================*/
extern void root(void *pvTaskPars)
{
   ULONG tid;
   ULONG ulStartupFlags;     // see boot.h for bit encoding
   BOOL fSafe = FALSE;
   INT  systemRet=-1;

   fprintf(myfp, "\n ROOT task thread_id =%p\n",(void*) pthread_self());fflush(myfp);


   // initialize the boot syslog function
   InitBootSyslog(syslog);

   // get startup flags and consecutive system error counter
   // from as set by command line arguments
   ulStartupFlags = Boot(BOOT_GET_STARTUP);


   systemRet =system("killall telnetd"); //killall returns 0 if any matched string got killed
   if(systemRet == 0)
   {
      //Boot(BOOT_INFO,0,"%s: Successfully Killed default telnetd service.",__FUNCTION__);
   }
   else
   {
      //If failed try again
      ExecDelayMSec(1000L);
      systemRet =system("killall telnetd"); //killall returns 0 if any matched string got killed
      /*if(systemRet == 0)
         Boot(BOOT_INFO,0,"2 %s: Successfully Killed default telnetd service.",__FUNCTION__);*/
      /*else
         Boot(BOOT_WARNING, sysstat.time,"%s: No Telnet service is running/ Failed killing it. ",__FUNCTION__);*/
   }

   // These structs eventually go away when the infrastructure is in place
   // to drive these (and other) configuration options via the front end.
   //
   // This is the wrong location for this initialization. It should be moved
   // closer to its first access. Doing it this early in system startup
   // should make it safe and expedient.
   LocalRouterParams.wMyUdpPort = 47808;
   strcpy(LocalRouterParams.acSerialNumber, "");
   strcpy(LocalRouterParams.acEthAdapter, DEFAULT_ETHERNET_NAME);

   // Create the clock task.
   ExecTaskCreate(&tid, "CLCK", ClockTask, CLOCK_PRIORITY, CLOCK_STACK_SIZE, 0, NULL);

   // Create the watchdog task. Do not added to list of tasks terminated by ExecDeleteAllThread()
   // to allow watchdog to continue to run during ArchiveExitNonBlocking() at shutdown to prevent
   // watchdog timeout when archiving large databases
   _ExecTaskCreate(&tid, "WDTT", WatchdogTask, WDT_PRIORITY, MAXTASK_STACK_SIZE, 0, NULL, FALSE);
   // After starting WatchdogTask, tickle watchdog
   WatchdogTickle();

   // Let the LED task have time to read the dip switches
   ExecDelay(2);

   // Get driver startup options.
   DecodeDriverOptions();

   // Get values stored in EEPROM and poke them into the global IPPARS structure.
   FetchIpPars_100BT(&ippars);

   /* Based on  UdpPortFile BACnet primary/scondary ports will be added in Firewall script */
   UpdateUdpPortFile(NULL);

   /* Restarting firewall, as Telnet port will be enabled
    * old restart was only enabled when Modbus TCP/IP is enabled
    */
   RestartFirewall();

   // Set the BACnet model name handler.
   pfnDrvGetBpModelName = DrvGetBpModelName;

   InitStore(FALSE);

  /**-----------------------------------------------*
   **    Returns a pointer to the module driver     **
   **    CPRUN, or NULL if no module driver found.  **
   **    Also sets the NV database size and         **
   **    syspars.                                   **
   **-----------------------------------------------*/
   pcpModuleDriver = SysReset(ulStartupFlags);

   if (pcpModuleDriver)
   {
      // Assign global pointers, for convenience
      pDRVPARS = (DRVPARS *)pcpModuleDriver->pPars;
      pDRVSTAT = (DRVSTAT *)pcpModuleDriver->pStat;
      pDRVVARS = (DRVVARS *)pcpModuleDriver->pVars;
      pDRVHIST = (DRVHIST *)pcpModuleDriver->pHist;

      // DE5772: Autogenerate BACnet Device Instance causes catastrophic failure.
      // Below section of code is moved from SysReset() function.
      // Below section of code must be called after initialization of pDRVPARS.
      // don't attempt to do the following if running in the gtest environment
#ifndef _UNIT_TEST
      // Force a reinit pass now.
      SysReinit(pcpModuleDriver);

      // Reset all local IO - all DOs off, all AOs = 0 volts.
      DrvResetIO();
#endif
   }

   // process reset cause from internal nvram and command line
   ulStartupFlags = ResetControlReadResetCause(ulStartupFlags);

   // as FWEX app has time with itself now, providing watchdog messages
   WatchdogModstatMsg(ulStartupFlags);

   if (pcpModuleDriver)
   {
      // Allow for network configuration overrides from WebCTRL's script(s).
      DrvSetLinuxEnvironmentForNetwork();

      DeviceColorCachingData.pDevParsID = &pdevPars->id;
      DeviceColorCachingData.pDevVarsID = &pdevVars->id;
      DeviceColorCachingData.pulSysParsGroupNumber = &pSyspars->ulGroupNumber;
      DeviceColorCachingData.pulSysStatMyCachingRouterID = &pSysstat->ulMyCachingRouterID;
      DeviceColorCachingData.pwCacheAllNetworksStatus = &pDRVSTAT->wCacheAllNetworksStatus;
      DeviceColorCachingData.pfnApplicationColorCaching = (void *)AppColorCaching;
      pDeviceColorCachingData =  &DeviceColorCachingData;
      // initialize color caching global pointer
      ColorInitializePars(&pDRVPARS->cachepars);

      // Global pointers used by the flash archiver
      pArchivePars = &pDRVPARS->ArchiverPars;
      pArchiveStat = &pDRVSTAT->ArchiverStat;

      // Global pointer for ACTnet status
      pDrvACTnetStat = &pDRVSTAT->aACTnetInfo;      //Initialize the pointer for the use of other modules

      // Set pointers to custom translation tables.
      // This activates these scaling options.
      MbScaleInitializeCTT(pDRVPARS->ctt);
   }

   if (IsFeatureSupported(BP_TIME_MASTER))
   {
      BACnetTimeMasterData.pusTimeSynchIntervalMin = &pDRVPARS->TimeMasterPars.usTimeSynchIntervalMin;
      BACnetTimeMasterData.pusTimeSynchIntervalOffsetMin = &pDRVPARS->TimeMasterPars.usTimeSynchIntervalOffsetMin;
      BACnetTimeMasterData.pfTimeSynchAlignIntervals = &pDRVPARS->TimeMasterPars.fTimeSynchAlignIntervals;
      BACnetTimeMasterData.plistTimeRecipients = &pdevPars->listTimeRecipients;
      BACnetTimeMasterData.plistUTCTimeRecipients = &pdevPars->listUTCTimeRecipients;

      // Launch the time master task.
      BpTimeMasterLaunch(&pDRVPARS->TimeMasterPars, &BACnetTimeMasterData);
   }

   // Activate the TCP/IP network interface
   IpInit();

   int number_of_datalinks = (IsFeatureSupported(ROUTING_SUPPORTED)) ? NUM_DATALINKS : NUM_DATALINKS_NON_ROUTER;

   if (!DrvIsHWPortS1Supported() || !IsFeatureSupported(MSTP_SUPPORT))
   {
       number_of_datalinks--;
   }

   if (!isArcnetHdwSupported() || !IsFeatureSupported(ARCNET_SUPPORT))
   {
       number_of_datalinks--;
   }

   IpdReset(pcpModuleDriver,
            &((DRVPARS*)(pcpModuleDriver->pPars))->abIPD,
            &((DRVSTAT*)(pcpModuleDriver->pStat))->abIPD,
            &((DRVHIST*)(pcpModuleDriver->pHist))->abIPD,
            &((DRVSTAT*)(pcpModuleDriver->pStat))->aIpdStatus[ number_of_datalinks ]);

   //
   // Launch the data link layers and the DLL Management Task
   //
   MyLaunchDlls();

   //
   // Create the task that control the Network and System status LEDs
   //
   StatusLEDsTaskStart(&(pDRVPARS->fLocatorLed));

   // Launch Local Network Configuration Task
   LocalNetworkTaskStart(&(pDRVPARS->LocalNwPars), &(pDRVSTAT->LocalNwStat));
   // Launch App-Device Management Task
   DeviceMngmtTaskStart(NULL, NULL);
   
   // Launch Sntp Time Update Task
   SntpTimeUpdateTaskStart(&(pDRVPARS->SntpPars), &(pDRVSTAT->SntpStat));

   // Launch the Data Server Task
   InitDataServer();
   InitAppDataServer();

   // Launch the "fake" download task. (Not so "fake" any more and needs to be renamed.)
   ExecTaskCreate(&tid, "FDLT", FdlFwexUpgradeTask, IPD_PRIORITY, IPD_STACK_SIZE, 0, NULL);

   // Launch the file archive task.
   ExecTaskCreate(&tid, "ARCH", ArchiveTask, ARCH_PRIORITY, CP_STACK_SIZE, 0, NULL);

   // Enter "safe mode" if there are frequent fatal errors on startup
   // (the upper 16 bits of the startup flags is the frequent fatal
   // (error counter).
   // If there have been too many consecutive errors, we never start
   // the rest of the system.  The show stops here.
   //
   // Examine consecutive error counter - example: if counter == 2, there
   // have been 3 recent errors.
   // If we have reached MAX_CONSECUTIVE_ERRORS, no more tasks are started.
   // If less than MAX_CONSECUTIVE_ERRORS, launch remaining tasks.
   //
   if ((ulStartupFlags >> 16) >= MAX_FREQ_ERRORS)
   {
      fSafe = TRUE;
      ModuleSetStatus(MODULE_HALTED);  // module driver halted
      Boot(BOOT_WARNING, sysstat.time, "Halted due to frequent system errors.");
   }

   // Enter "safe mode" if the module is undergoing a format invoked
   // by either pressing the format button or sending the appropriate
   // software command.
   if (sysstat.fFormatButton ||
      (ulStartupFlags & (STARTUP_BOOT_REPLACEMENT | STARTUP_EXEC_REPLACEMENT)))
   {
      fSafe = TRUE;
   }

   // Enter "safe mode" if the database cannot be found.
   if (!pNV)
   {
      fSafe = TRUE;
   }

   // "Safe mode" does not start up I/O or GFB related tasks.
   if (!fSafe)
   {

      // Check to see if we are restarting after an abnormal reset and if so,
      // delay before starting the rest of the system.
      AutoRestartDelay(ulStartupFlags);

      if(IsFeatureSupported(RSM_SUPPORT) == TRUE) //RSM Support only for OFBBC Variant
      {
         RtsStart(&(pDRVPARS->rtspars), &(pDRVSTAT->rtsstat));
      }
      /*---------------------------------------------------------------------*/
      /* Create and Start the MBclient task.                                 */
      /*---------------------------------------------------------------------*/
      MbClientTaskCreate(pdevConst->usEepromProductType);

      /*---------------------------------------------------------------------*/
      /* Create and Start the CP execution task(s) - maybe more than one.    */
      /*---------------------------------------------------------------------*/
      Boot(BOOT_TRACE,"%s: pre-CpStart.",__FUNCTION__);
      IOInterfaceStartup();
      CpStart(pcpModuleDriver);
      Boot(BOOT_TRACE,"%s: post-CpStart.",__FUNCTION__);

      // Revive 10Hz task
      /*---------------------------------------------------------------------*/
      /* Create and Start the Hw10Hz task.                                   */
      /*---------------------------------------------------------------------*/
      ExecTaskCreate(&idHw10Hz, "10HZ", Hw10HzTask, HW10HZ_PRIORITY, HW10HZ_STACK_SIZE, 0, NULL);


   }
   else
   {
      // Maintain the watchdog timer from here since the GFB task is not running

      // Use the GFB task priority.
      ExecChangePriority(NULL, CP_PRIORITY);

      for (;;)
      {
         WatchdogTickle();
         ExecDelayMSec(1000L);
      }
   }

   /*------------------------------------------------------------------------------------**
   ** Suspend this task (root) - this will have the effect of simultaneously unblocking  **
   ** all tasks just started above, except that the Clock task is already running.       **
   **------------------------------------------------------------------------------------*/
   ExecTaskSuspend(0L);
}

/**=== ConvertEthernetMACToString =====================================================

   This function converts a 6-byte array representing an Ethernet MAC address
    to a string.

   This was basically written to make unit testing easier.

   Name              Dir   Description
   pchEthMACArray    in    The array holding the Ethernet MAC address
   pchString         i/o   Pointer to the string buffer to be loaded


   Returns: number of chars put into the string.

   Caution: This function checks for a NULL string pointer, however
     it does NOT know the allocated string length or how much room is
     left in the string. So it's the caller's responsibility to be sure
     the string has at least 18 characters worth of space.

=========================================================================*/
extern int ConvertEthernetMACToString(CHAR* pchEthMacArray, CHAR* pchString)
{
   CHAR* psz = pchString;
   int iCharsConsumed = 0;

   if (pchEthMacArray && psz)
   {
      // NULL terminate the char array so it can be used as a string
      *psz = 0;

      iCharsConsumed = sprintf(psz, "%02X-%02X-%02X-%02X-%02X-%02X",
                                 (BYTE)pchEthMacArray[0],
                                 (BYTE)pchEthMacArray[1],
                                 (BYTE)pchEthMacArray[2],
                                 (BYTE)pchEthMacArray[3],
                                 (BYTE)pchEthMacArray[4],
                                 (BYTE)pchEthMacArray[5]);
   }

   return iCharsConsumed;
}


/**=== GetEthernetMACAddress =====================================================

   This function loads a supplied string pointer with the Ethernet MAC address
    of the Ethernet interface being used by the BACnet stack.

   Name        Dir   Description
   pchString   i/o   Pointer to the string buffer to be loaded


   Returns: number of chars put into the string.

   Caution: This function checks for a NULL string pointer, however
     it does NOT know the allocated string length or how much room is
     left in the string. So it's the caller's responsibility to be sure
     the string has at least 18 characters worth of space.

=========================================================================*/
extern int GetEthernetMACAddress(CHAR* pchString)
{
   CHAR* psz = pchString;
   int iCharsConsumed = 0;

   iCharsConsumed = ConvertEthernetMACToString(pchEthernetAddr, pchString);

   return iCharsConsumed;
}

#define NET_ADD_SERIAL_PORTS_TO_MODSTAT_NAME_SIZE 9
/**=== NetAddSerialPortsToModstat ========================================

   This function adds Serial Port information to the Modstat page.
   This function was written to be called by NetAddModstat.

   Name        Dir     Description
   ppszDest    In/Out  Pointer to the string buffer for the screen data.
   maxsize     In/Out  The maximum size the screen buffer can accept.
                       Updated on return.

   Returns:  None.

   Cautions: None.

=========================================================================*/
static void NetAddSerialPortsToModstat(CHAR **ppszDest, size_t *maxsize)
{
   BOOL fHasPortS1, fHasPortS2;
   char szProtocol[NET_ADD_SERIAL_PORTS_TO_MODSTAT_NAME_SIZE];
   int iPortS2Addr;

   fHasPortS1 = DrvIsHWPortS1Supported();
   fHasPortS2 = DrvIsHWPortS2Supported();

   if (fHasPortS1 || fHasPortS2)
   {
      PRINT_TO_BUFFER(*ppszDest, *maxsize, NEWLINE "Serial Ports:" NEWLINE); 
   }

   if (fHasPortS1)
   {
      PRINT_TO_BUFFER(*ppszDest, *maxsize, "  %s: ", DrvGetPortS1Name()); 
      GetProtocolOnPortS1(szProtocol, NET_ADD_SERIAL_PORTS_TO_MODSTAT_NAME_SIZE);
      PRINT_TO_BUFFER(*ppszDest, *maxsize, "Protocol = %s", szProtocol);
      if (pDRVSTAT)
      {
         if (pDRVSTAT->bPortDLLType[PORTS1] != DLL_DISABLED) 
         {
            ULONG ulModbusVal = 0;

            if (pDRVSTAT->bPortDLLType[PORTS1] == DLL_MODBUS)
            {
               IpdGetModbusSrlPars(MSRL1_MASTER_SLAVE, &ulModbusVal);
               if (ulModbusVal == MODBUS_IS_MASTER)
               {
                  // Modbus slaves are address 1-255. Technically, address 0
                  // is reserved for broadcast messages, Masters don't have
                  // an address.
                  PRINT_TO_BUFFER(*ppszDest, *maxsize, " Master");
               }
               else
               {
                  ULONG ulVal = 0;
                  // Modules with rotary switches set the S1 modbus slave address with hardware.
                  if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
                  {
                      ulVal = IpdGetSlaveId(MODBUS_SRL1_SGNTR);
                  }
                  else
                  {
                     IpdGetModbusSrlPars(MSRL1_SLAVE_ADDRESS,&ulVal);
                  }
                  PRINT_TO_BUFFER(*ppszDest, *maxsize, ", Address = %ld", ulVal); 
               }
            }
            else
            {
                if ( (BP_VENDOR_ID == BP_VENDOR_ID_CARRIER) &&
                     (bFwexProfileType == IP_HOME_ROUTER_TYPE))
                {
                   PRINT_TO_BUFFER(*ppszDest, *maxsize, ", Address = %d", pDRVSTAT->bPortMACAddr[PORTS1]);
                }
                else
                {
               PRINT_TO_BUFFER(*ppszDest, *maxsize, ", Address = %d", GetCurrentAddressValueOnPortS1()); 
            }
         }
      }
      }
      PRINT_TO_BUFFER(*ppszDest, *maxsize, NEWLINE);
   }

   if (fHasPortS2)
   {
      PRINT_TO_BUFFER(*ppszDest, *maxsize, "  %s: ", DrvGetPortS2Name()); 
      GetProtocolOnPortS2(szProtocol, NET_ADD_SERIAL_PORTS_TO_MODSTAT_NAME_SIZE);
      PRINT_TO_BUFFER(*ppszDest, *maxsize, "Protocol = %s", szProtocol);
      if (pDRVSTAT)
      {
         if (pDRVSTAT->bPortDLLType[PORTS2] != DLL_DISABLED)
         {
            iPortS2Addr = GetCurrentAddressValueOnPortS2();
            if (iPortS2Addr != -1)
            {
               PRINT_TO_BUFFER(*ppszDest, *maxsize, ", Address = %d", iPortS2Addr); 
            }
            else
            {
               // The address -1 indicates Modbus is in Master mode.
               PRINT_TO_BUFFER(*ppszDest, *maxsize, " Master"); 
            }
         }
      }
      PRINT_TO_BUFFER(*ppszDest, *maxsize, NEWLINE);
   }
}

/**=== NetAddModstat =====================================================

   This function gets installed into the AddModstatText hook and is
   therefor called automatically during every Modstat report.
   It always returns TRUE, which indicates that we HAVE added text
   to the Modstat screen.
   We use this hook to add Ethernert and UDP/IP information to
   the Modstat page.

   Name        Dir     Description
   pszDest     In/Out  Pointer to the string buffer for the screen data.
   maxsize     In      The maximum size the screen buffer can accept.

   Returns:  TRUE on success, FALSE on failure.

   Cautions: None.

=========================================================================*/
static BOOL NetAddModstat(CHAR *pszDest, size_t maxsize)
{
   ETHERNET_STATISTICS        *pEthernetStatistics;
   ARCNET_STATISTICS          *pARCnetStatistics;
   MSTP_STATISTICS            *pMSTPStatistics;
   ROUTED_PACKETS_STATISTICS  *pRouterStatistics;
   CHAR *psz = pszDest;

   union
   {
      ULONG ul;
      BYTE ab[4];
   } ipaddr;

   LONG sizeused = (LONG)maxsize;

   PRINT_TO_BUFFER(psz, sizeused, "Network Information:" NEWLINE);

   if (pchEthernetAddr)
   {
      PRINT_TO_BUFFER(psz, sizeused, "  Ethernet MAC address  = %02X-%02X-%02X-%02X-%02X-%02X" NEWLINE,
                     (BYTE)pchEthernetAddr[0],
                     (BYTE)pchEthernetAddr[1],
                     (BYTE)pchEthernetAddr[2],
                     (BYTE)pchEthernetAddr[3],
                     (BYTE)pchEthernetAddr[4],
                     (BYTE)pchEthernetAddr[5]);
   }

   // display the current IP
   ipaddr.ul = ulMyIPAddr;
   PRINT_TO_BUFFER(psz, sizeused, "  Current IP Address    = %u.%u.%u.%u" NEWLINE,
                  ipaddr.ab[0],
                  ipaddr.ab[1],
                  ipaddr.ab[2],
                  ipaddr.ab[3]);

   ipaddr.ul = ulMySubnetMask;
   PRINT_TO_BUFFER(psz, sizeused, "  Current Subnet Mask   = %u.%u.%u.%u" NEWLINE,
                  ipaddr.ab[0],
                  ipaddr.ab[1],
                  ipaddr.ab[2],
                  ipaddr.ab[3]);

   ipaddr.ul = ulMyGatewayIPAddr;
   PRINT_TO_BUFFER(psz, sizeused, "  Current Gateway Addr  = %u.%u.%u.%u" NEWLINE,
                  ipaddr.ab[0],
                  ipaddr.ab[1],
                  ipaddr.ab[2],
                  ipaddr.ab[3]);

   ipaddr.ul = ulMyDNSServer1;
   PRINT_TO_BUFFER(psz, sizeused, "  Current DNS Server 1  = %u.%u.%u.%u" NEWLINE,
                   ipaddr.ab[0],
                   ipaddr.ab[1],
                   ipaddr.ab[2],
                   ipaddr.ab[3]);
   
   ipaddr.ul = ulMyDNSServer2;
   PRINT_TO_BUFFER(psz, sizeused, "  Current DNS Server 2  = %u.%u.%u.%u" NEWLINE,
                   ipaddr.ab[0],
                   ipaddr.ab[1],
                   ipaddr.ab[2],
                   ipaddr.ab[3]);
   
   // display the assigned IP
   if (pDRVPARS)
   {
      ipaddr.ul = ippars.ulIPAddress;
      PRINT_TO_BUFFER(psz, sizeused, "  Assigned IP Address   = %u.%u.%u.%u" NEWLINE,
                     ipaddr.ab[0],
                     ipaddr.ab[1],
                     ipaddr.ab[2],
                     ipaddr.ab[3]);

      ipaddr.ul = ippars.ulSubnetMask;
      PRINT_TO_BUFFER(psz, sizeused, "  Assigned Subnet Mask  = %u.%u.%u.%u" NEWLINE,
                     ipaddr.ab[0],
                     ipaddr.ab[1],
                     ipaddr.ab[2],
                     ipaddr.ab[3]);

      ipaddr.ul = ippars.ulGatewayIPAddress;
      PRINT_TO_BUFFER(psz, sizeused, "  Assigned Gateway Addr = %u.%u.%u.%u" NEWLINE,
                     ipaddr.ab[0],
                     ipaddr.ab[1],
                     ipaddr.ab[2],
                     ipaddr.ab[3]);
      
      ipaddr.ul = ippars.ulDNSServer1;
      PRINT_TO_BUFFER(psz, sizeused, "  Assigned DNS Server 1 = %u.%u.%u.%u" NEWLINE,
                      ipaddr.ab[0],
                      ipaddr.ab[1],
                      ipaddr.ab[2],
                      ipaddr.ab[3]);
      
      ipaddr.ul = ippars.ulDNSServer2;
      PRINT_TO_BUFFER(psz, sizeused, "  Assigned DNS Server 2 = %u.%u.%u.%u" NEWLINE,
                      ipaddr.ab[0],
                      ipaddr.ab[1],
                      ipaddr.ab[2],
                      ipaddr.ab[3]);
   }

   //
   // Include Serial Port information.
   //
   NetAddSerialPortsToModstat(&psz, (size_t *)&sizeused);

   //
   // Include Statistics for Routed Packets (if a router)
   //
   if (pSyspars->awNet[NG_HOME_NETWORK_IDX] && IsFeatureSupported(ROUTING_SUPPORTED))
   {
      //     ----> Routed Packets Statistics  <----
      pRouterStatistics = (ROUTED_PACKETS_STATISTICS *)GetRouterStatisticsStructPtr();

      if (pRouterStatistics)
      {
         PRINT_TO_BUFFER(psz, sizeused, NEWLINE "Router Statistics" NEWLINE);
         PRINT_TO_BUFFER(psz, sizeused, "  Dropped Packets                                 %lu" NEWLINE, pRouterStatistics->ulRouterDroppedPacketsCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Route Not Found                                 %lu" NEWLINE, pRouterStatistics->ulRouteNotFoundCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Route Unreachable                               %lu" NEWLINE, pRouterStatistics->ulRouteUnreachableCount);
         PRINT_TO_BUFFER(psz, sizeused, NEWLINE);
         PRINT_TO_BUFFER(psz, sizeused, "  Network Activity                        Incoming                Outgoing"       NEWLINE);
         PRINT_TO_BUFFER(psz, sizeused, "                                      Unicast  Broadcast      Unicast  Broadcast" NEWLINE);

         if (pSysstat->awNet[NG_IP_NETWORK_IDX])
         {
            PRINT_TO_BUFFER(psz, sizeused, "   IP Network:           %5u     %10lu %10lu   %10lu %10lu" NEWLINE,
                           pSysstat->awNet[NG_IP_NETWORK_IDX],
                           pRouterStatistics->IPPort.ulIncomingUnicast,
                           pRouterStatistics->IPPort.ulIncomingBroadcast,
                           pRouterStatistics->IPPort.ulOutgoingUnicast,
                           pRouterStatistics->IPPort.ulOutgoingBroadcast);
         }

         if (pSysstat->awNet[NG_SEC_IP_NETWORK_1_IDX])
         {
            PRINT_TO_BUFFER(psz, sizeused,  "   Secondary IP Network: %5u     %10lu %10lu   %10lu %10lu" NEWLINE,
                           pSysstat->awNet[NG_SEC_IP_NETWORK_1_IDX],
                           pRouterStatistics->SecondaryIPPort.ulIncomingUnicast,
                           pRouterStatistics->SecondaryIPPort.ulIncomingBroadcast,
                           pRouterStatistics->SecondaryIPPort.ulOutgoingUnicast,
                           pRouterStatistics->SecondaryIPPort.ulOutgoingBroadcast);
         }

         if (pSysstat->awNet[NG_ETHERNET_NETWORK_IDX])
         {
            PRINT_TO_BUFFER(psz, sizeused, "   Ethernet Network:     %5u     %10lu %10lu   %10lu %10lu" NEWLINE,
                           pSysstat->awNet[NG_ETHERNET_NETWORK_IDX],
                           pRouterStatistics->EthernetPort.ulIncomingUnicast,
                           pRouterStatistics->EthernetPort.ulIncomingBroadcast,
                           pRouterStatistics->EthernetPort.ulOutgoingUnicast,
                           pRouterStatistics->EthernetPort.ulOutgoingBroadcast);
         }

         if (pSysstat->awNet[NG_PORTS1_NETWORK_IDX] && isPortS1HdwSupported())
         {
            PRINT_TO_BUFFER(psz, sizeused, "   %s Network:      %*u     %10lu %10lu   %10lu %10lu" NEWLINE,
                           DrvGetPortS1Name(),
                           (int)(12-strlen(DrvGetPortS1Name())),
                           pSysstat->awNet[NG_PORTS1_NETWORK_IDX],
                           pRouterStatistics->PortS1Port.ulIncomingUnicast,
                           pRouterStatistics->PortS1Port.ulIncomingBroadcast,
                           pRouterStatistics->PortS1Port.ulOutgoingUnicast,
                           pRouterStatistics->PortS1Port.ulOutgoingBroadcast);
         }

         if (pSysstat->awNet[NG_PORTS2_NETWORK_IDX])
         {
            PRINT_TO_BUFFER(psz, sizeused, "   %s Network:      %*u     %10lu %10lu   %10lu %10lu" NEWLINE,
                           DrvGetPortS2Name(),
                           (int)(12-strlen(DrvGetPortS2Name())),
                           pSysstat->awNet[NG_PORTS2_NETWORK_IDX],
                           pRouterStatistics->PortS2Port.ulIncomingUnicast,
                           pRouterStatistics->PortS2Port.ulIncomingBroadcast,
                           pRouterStatistics->PortS2Port.ulOutgoingUnicast,
                           pRouterStatistics->PortS2Port.ulOutgoingBroadcast);
         }

         PRINT_TO_BUFFER(psz, sizeused, NEWLINE);
         PRINT_TO_BUFFER(psz, sizeused, "  Router Sourced Packets" NEWLINE);
         PRINT_TO_BUFFER(psz, sizeused, "       Unicast  Broadcast"   NEWLINE);
         PRINT_TO_BUFFER(psz, sizeused, "    %10lu %10lu" NEWLINE,
                        pRouterStatistics->ulRouterSourcedUnicastCount,
                        pRouterStatistics->ulRouterSourcedBroadcastCount);
      }
   }

   //
   // If Configured, include Statistics for GigE port
   //
   if ((pSysstat->awNet[NG_IP_NETWORK_IDX]) || (pSysstat->awNet[NG_ETHERNET_NETWORK_IDX]))
   {
      pEthernetStatistics = (ETHERNET_STATISTICS *)GetEthernetStatisticsStructPtr();

      if (pEthernetStatistics)
      {
         if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
         {
          PRINT_TO_BUFFER(psz, sizeused, NEWLINE "Gig-E Port Ethernet Statistics" NEWLINE);
         }
         else
         {
          PRINT_TO_BUFFER(psz, sizeused, NEWLINE "Eth0/Eth1 Port Ethernet Statistics" NEWLINE);//As Gig-E port speed is not supported in NGZN
         }

         PRINT_TO_BUFFER(psz, sizeused, "  Ethernet Rx packets                             %lu" NEWLINE, pEthernetStatistics->ulEthernetRxPacketCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Ethernet Tx packets                             %lu" NEWLINE, pEthernetStatistics->ulEthernetTxPacketCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Receive Errors (total)                          %lu" NEWLINE, pEthernetStatistics->ulEthernetRxErrorCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Transmit Errors (total)                         %lu" NEWLINE, pEthernetStatistics->ulEthernetTxErrorCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Dropped Packets                                 %lu" NEWLINE, pEthernetStatistics->ulEtherenetDroppedPacketCount);

         if (pSyspars->awNet[NG_IP_NETWORK_IDX])
         {
            if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
            {
              PRINT_TO_BUFFER(psz, sizeused,  NEWLINE "Gig-E Port BACnet/IP Statistics" NEWLINE);
            }
            else
            {
              PRINT_TO_BUFFER(psz, sizeused,  NEWLINE "Eth0/Eth1 Port BACnet/IP Statistics" NEWLINE); //As Gig-E port speed is not supported in NGZN
            }

            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/IP Rx Unicast Packets                    %lu" NEWLINE, pEthernetStatistics->ulBpIpRxUnicastPacketCount);
            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/IP Tx Unicast Packets                    %lu" NEWLINE, pEthernetStatistics->ulBpIpTxUnicastPacketCount);
            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/IP Rx Broadcast Packets                  %lu" NEWLINE, pEthernetStatistics->ulBpIpRxBroadcastPacketCount);
            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/IP Tx Broadcast Packets                  %lu" NEWLINE, pEthernetStatistics->ulBpIpTxBroadcastPacketCount);
            PRINT_TO_BUFFER(psz, sizeused,  "  Whitelist Rejections                            %lu" NEWLINE, pEthernetStatistics->ulWhiteListRejectionCount);
         }


         if (pSyspars->awNet[NG_ETHERNET_NETWORK_IDX])
         {
            if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
            {
             PRINT_TO_BUFFER(psz, sizeused,  NEWLINE "Gig-E Port BACnet/Ethernet Statistics" NEWLINE); //As Gig-E port speed is not supported in NGZN
            }
            else
            {
               PRINT_TO_BUFFER(psz, sizeused,  NEWLINE "Eth0/Eth1 Port BACnet/Ethernet Statistics" NEWLINE);
            }

            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/Ethernet Tx Unicast Packets              %lu" NEWLINE, pEthernetStatistics->ulBpEthernetRxUnicastPacketCount);
            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/Ethernet Rx Unicast Packets              %lu" NEWLINE, pEthernetStatistics->ulBpEthernetTxUnicastPacketCount);
            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/Ethernet Tx Broadcast Packets            %lu" NEWLINE, pEthernetStatistics->ulBpEthernetRxBroadcastPacketCount);
            PRINT_TO_BUFFER(psz, sizeused,  "  BACnet/Ethernet Rx Broadcast Packets            %lu" NEWLINE, pEthernetStatistics->ulBpEthernetTxBroadcastPacketCount);
         }
      }
   }

   //
   // If Configured, include Statistics for PORTS1 - determine if ARCnet or MSTP
   //
   if (pSysstat->awNet[NG_PORTS1_NETWORK_IDX])
   {
      //     ----> ARCnet Statistics  <----
      if (pDRVSTAT->bPortDLLType[PORTS1] == DLL_ARCNET)
      {
         pARCnetStatistics = (ARCNET_STATISTICS *)GetARCnetStatisticsStructPtr();

         if (pARCnetStatistics)
         {
            PRINT_TO_BUFFER(psz, sizeused, NEWLINE "%s ARCnet Statistics " NEWLINE, DrvGetPortS1Name());
            PRINT_TO_BUFFER(psz, sizeused, "  Node Reconfiguration                            %lu" NEWLINE, pARCnetStatistics->ulARCnetNodeReconfigurationCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Bus Reconfiguration                             %lu" NEWLINE, pARCnetStatistics->ulARCnetBusReconfigurationCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Excessive NACK                                  %lu" NEWLINE, pARCnetStatistics->ulARCnetExcessiveNACKCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Dropped Packets                                 %lu" NEWLINE, pARCnetStatistics->ulARCnetKernelDriverErrorCount);
            PRINT_TO_BUFFER(psz, sizeused, "  BACnet/ARCnet Rx Packets                        %lu" NEWLINE, pARCnetStatistics->ulARCnetRxPacketCount);
            PRINT_TO_BUFFER(psz, sizeused, "  BACnet/ARCnet Tx Packets                        %lu" NEWLINE, pARCnetStatistics->ulARCnetTxPacketCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Average Latency Value (mSec)                    %lu" NEWLINE, pARCnetStatistics->ulARCnetAverageLatency);
            PRINT_TO_BUFFER(psz, sizeused, "  Maximum Latency Value (mSec)                    %lu" NEWLINE, pARCnetStatistics->ulARCnetMaximumLatency);
         }
      }

      //     ----> MSTP Statistics  <----
      if (pDRVSTAT->bPortDLLType[PORTS1] == DLL_MSTP)
      {
         pMSTPStatistics = (MSTP_STATISTICS *)GetMSTPStatisticsStructPtr(PORTS1);

         if (pMSTPStatistics)
         {
            PRINT_TO_BUFFER(psz, sizeused, NEWLINE "%s MSTP Statistics" NEWLINE, DrvGetPortS1Name());
            PRINT_TO_BUFFER(psz, sizeused, "  UART Errors                                     %lu" NEWLINE, pMSTPStatistics->ulMSTPUARTErrorCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Invalid Frames                                  %lu" NEWLINE, pMSTPStatistics->ulMSTPInvalidFramesCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Dropped Packets                                 %lu" NEWLINE, pMSTPStatistics->ulMSTPKernelDriverErrorsCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Dropped Tokens                                  %lu" NEWLINE, pMSTPStatistics->ulMSTPDroppedTokensCount);
            PRINT_TO_BUFFER(psz, sizeused, "  No Responses                                    %lu" NEWLINE, pMSTPStatistics->ulMSTPNoResponsesCount);
            PRINT_TO_BUFFER(psz, sizeused, "  BACnet/MSTP Rx Packets                          %lu" NEWLINE, pMSTPStatistics->ulMSTPRxPacketCount);
            PRINT_TO_BUFFER(psz, sizeused, "  BACnet/MSTP Tx Packets                          %lu" NEWLINE, pMSTPStatistics->ulMSTPTxPacketCount);
            PRINT_TO_BUFFER(psz, sizeused, "  Average Latency Value (mSec)                    %lu" NEWLINE, pMSTPStatistics->ulMSTPAverageLatency);
            PRINT_TO_BUFFER(psz, sizeused, "  Maximum Latency Value (mSec)                    %lu" NEWLINE, pMSTPStatistics->ulMSTPMaximumLatency);
         }
      }
   }

   //
   // If Configured, include Statistics for PORTS2
   //
   if (pSysstat->awNet[NG_PORTS2_NETWORK_IDX])
   {
      //     ----> MSTP Statistics  <----
      pMSTPStatistics = (MSTP_STATISTICS *)GetMSTPStatisticsStructPtr(PORTS2);

      if (pMSTPStatistics)
      {
         PRINT_TO_BUFFER(psz, sizeused, NEWLINE "%s MSTP Statistics " NEWLINE, DrvGetPortS2Name());
         PRINT_TO_BUFFER(psz, sizeused, "  UART Errors                                     %lu" NEWLINE, pMSTPStatistics->ulMSTPUARTErrorCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Invalid Frames                                  %lu" NEWLINE, pMSTPStatistics->ulMSTPInvalidFramesCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Dropped Packets                                 %lu" NEWLINE, pMSTPStatistics->ulMSTPKernelDriverErrorsCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Dropped Tokens                                  %lu" NEWLINE, pMSTPStatistics->ulMSTPDroppedTokensCount);
         PRINT_TO_BUFFER(psz, sizeused, "  No Responses                                    %lu" NEWLINE, pMSTPStatistics->ulMSTPNoResponsesCount);
         PRINT_TO_BUFFER(psz, sizeused, "  BACnet/MSTP Rx Packets                          %lu" NEWLINE, pMSTPStatistics->ulMSTPRxPacketCount);
         PRINT_TO_BUFFER(psz, sizeused, "  BACnet/MSTP Tx Packets                          %lu" NEWLINE, pMSTPStatistics->ulMSTPTxPacketCount);
         PRINT_TO_BUFFER(psz, sizeused, "  Average Latency Value (mSec)                    %lu" NEWLINE, pMSTPStatistics->ulMSTPAverageLatency);
         PRINT_TO_BUFFER(psz, sizeused, "  Maximum Latency Value (mSec)                    %lu" NEWLINE, pMSTPStatistics->ulMSTPMaximumLatency);
      }

   }

   if (sizeused < 0)
   {
      return FALSE;
   }
   else if(pfnNetAddModstatChained)
   {
      return pfnNetAddModstatChained(psz,(size_t)sizeused);
   }
   else
   {
      return TRUE;
   }

}

/**=== IpInit ============================================================

   Initialize the TCP/IP network interface.

   Name        Dir   Description
   pcp         In    Module driver's CPRUN.

   Returns:  None.

   Cautions: This function contains makes calls specific to the RTOS.

=========================================================================*/
static void IpInit()
{
   IPPARS *pIpPars;

   // Make sure we have a valid Ethernet address
   pchEthernetAddr = GetEthernetAddr_100BT();   // sets global pointer to Ethernet MAC address
   if (!pchEthernetAddr)
   {
      Boot(BOOT_WARNING, time (NULL), "Ethernet MAC address invalid.");
   }

   pIpPars = &ippars;
   // ulNodeBindingIpAddress is used for the CCN/IP binding under Linux.
   ulMyIPAddr = EvaluateIPAddr (pIpPars, (BYTE)sysstat.uLGnetAddr);
   ulMySubnetMask    = EvaluateSubnetMask(pIpPars);
   ulMyGatewayIPAddr = EvaluateGatewayAddr(pIpPars);
   ulMyDNSServer1    = EvaluateDNSServerAddr(IP_CUSTOM_DNS_SERVER1, pIpPars);
   ulMyDNSServer2    = EvaluateDNSServerAddr(IP_CUSTOM_DNS_SERVER2, pIpPars);
   wMyUdpPort        = EvaluateUdpPort(IP_CUSTOM_UDP_PORT, pIpPars);
   wMySecUdpPort     = EvaluateUdpPort(IP_CUSTOM_SEC_UDP_PORT, pIpPars);

   if (pDRVSTAT)
   {
      // Copy some values to status for display purposes.
      pDRVSTAT->ipstat.ulIPAddress        = ulMyIPAddr;
      pDRVSTAT->ipstat.ulSubnetMask       = ulMySubnetMask;
      pDRVSTAT->ipstat.ulGatewayIPAddress = ulMyGatewayIPAddr;
      pDRVSTAT->ipstat.ulDNSServer1       = ulMyDNSServer1;
      pDRVSTAT->ipstat.ulDNSServer2       = ulMyDNSServer2;
      pDRVSTAT->ipstat.wUdpPort           = htons(wMyUdpPort);
      pDRVSTAT->ipstat.wSecUdpPort[IP_SEC_UDP_PORT_IDX]     = htons(wMySecUdpPort);
      if (pchEthernetAddr)
         memcpy(pDRVSTAT->abMACAddr, pchEthernetAddr, 6);
      else
         memset(pDRVSTAT->abMACAddr, 0, 6);  // show MAC address as all zeros
   }

   BpNetSetStatisticsPointer();

   NetworkStatisticsInitialize();

   // Activate the Modstat hook so that network information is displayed
   // on the Modstat page.
   pfnNetAddModstatChained=pfnAddModstatText;
   pfnAddModstatText = NetAddModstat;
}

/**=== IpAddressChangeover ===============================================

   This function is called periodically from the driver cp to check
   for an IP address change.  The strategy is to poll the current
   gateway IP address with ARP requests until it is nonresponsive for
   the specified timeout.  When that timeout is reached, we assume it
   has been nonresponsive because the new IP configuration is in
   effect and it is time to take on the new settings as specified
   in the parameters.  Also, the old settings are copied to
   the parameters when changing over to the new settings so that
   it is possible to automatically revert to them using the same
   mechanism.

   Name        Dir   Description

   Returns:
      void

   Cautions:

=========================================================================*/
static void IpAddressChangeover(void)
{
   // Force read of bms_mode before use.
   GetBMSMode(NULL, 0);

   // The module must be configured for custom address,
   // the changeover must be authorized,
   // and the change must be to a valid (nonzero) IP address
   if ( (bms_mode == BMSM_USER_DEFINED_STATIC) &&
        pDRVPARS->ipnext.fEnableChangeover &&
        pDRVPARS->ipnext.ippars.ulIPAddress )
   {
        // Periodically poll the next gateway using the next IP address to determine its connectivity.
        ArpTestRequest(
       (char *)&(LocalRouterParams.acEthAdapter),
          ulMyIPAddr,
          pDRVPARS->ipnext.ippars.ulIPAddress,
          pDRVPARS->ipnext.ippars.ulGatewayIPAddress);

  // If the next gateway has responded to our polls, perform the changeover.
        if ( ArpTestActive() ||

        // If a timeout has been specified, force the changeover to occur when
        // the next gateway does not respond to the polls for the timeout.
         ( pDRVPARS->ipnext.usInactivityTimeout &&
             (ArpTestInactivity() >= pDRVPARS->ipnext.usInactivityTimeout) ) )
        {
            // Do the changeover.
            // Write the new configuration into EEPROM.
            StoreIpPars_100BT(&pDRVPARS->ipnext.ippars);
            // Disable further changeover activity.
            pDRVPARS->ipnext.fEnableChangeover = FALSE;
            // Reset to use the new settings.
            Boot(BOOT_RESET, sysstat.time, "IP reconfiguration");
        }
      }
   else
   {
      // Indicate to user that the attempt cannot be made by
      // clearing the changeover checkbox on the property page.
      pDRVPARS->ipnext.fEnableChangeover = FALSE;

      // Reset the arp test in preparation for use at a later time.
      ArpTestReset();
   }
}

/**=== IpAddressChangeover ===============================================

   This function is called periodically from the driver cp to check
   for an IP configuration change by the network port object.
   The network port object is used by equipment touch.

   Name        Dir   Description
   NA          NA    NA

   Returns:
      void

   Cautions:

=========================================================================*/
static void IpChangeFromNetworkPortObject(void)
{
   OBJINST *pObj;
   NETWORK_PORT_VARS *pVars;
   BACnetObjectIdentifier idObject = MAKE_OBJID(OBJTYPE_NETWORK_PORT, 1);
   INT iCount;

   pObj = OmFindObj(idObject);

   if (pObj)
   {
      pVars = (NETWORK_PORT_VARS*)pObj->apPtr[OBJPTR_VARS];

      // Determine if a module reset is required.
      if (pVars->fResetModule)
      {
         // Endianness of udp ports.
         ippars.wUdpPort = htons(ippars.wUdpPort);
         for (iCount=0; iCount<MAX_IP_SECONDARY_PORTS; iCount++)
         {
            ippars.wSecUdpPort[iCount] = htons(ippars.wSecUdpPort[iCount]);
         }

         // Write the new configuration into EEPROM.
         StoreIpPars_100BT(&ippars);
         pVars->fResetModule = FALSE;
         Boot(BOOT_RESET, sysstat.time, "IP parameters changed.");
      }
   }
}

/**=== DecodeDriverOptions ===============================================

   Initializes options.

   Name        Dir   Description
   NA          NA    NA

   Returns:  None.

   Cautions: None.

=========================================================================*/
static void DecodeDriverOptions()
{
   const char *pEnvString;
   size_t length;

   pEnvString = getenv("EXECB_BACNET_IP_ADAPTER");

   if (pEnvString)
   {
      memset(LocalRouterParams.acEthAdapter,0x00,sizeof(LocalRouterParams.acEthAdapter));

      length = strlen(pEnvString);

      if (length > sizeof(LocalRouterParams.acEthAdapter) - 1)
      {
         length = sizeof(LocalRouterParams.acEthAdapter) - 1;
      }

      memcpy(LocalRouterParams.acEthAdapter,pEnvString,length);

      if (LocalRouterParams.acEthAdapter[length-1] == '\r')
      {
         LocalRouterParams.acEthAdapter[length-1] = 0x00;
      }

      Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK),"%s: EXECB_BACNET_IP_ADAPTER = %s", __FUNCTION__, LocalRouterParams.acEthAdapter);
   }

   // by default, disable PORTS2 MSTP and enable PORTS2 UART (DllMgmtTask will swap if needed)
   fDriverEnabled[PORTS2_MSTP_DRIVER] = FALSE;
   fDriverEnabled[PORTS2_UART_DRIVER] = TRUE;

   ulReservedPort = UART_SCC1;

}

/*========================================================================
IPD Port interface
=========================================================================*/

/**=== IpdPortUART =======================================================

   This function is called to return which serial channel is used for
   an IPD Port.  Care should be taken to insure that the number of
   indexed elements represents the value defined for NUM_IPD_PORTS.

   Name        Dir   Description
   ulIpdPort   In    One-based index of IPD ports.

   Returns:  Serial channel number associated with IPD port.

   Cautions: Zero will always be returned on the iVu Express product.

=========================================================================*/
extern ULONG IpdPortUART(ULONG ulIpdPort)
{
   ASSERT(ulIpdPort && (ulIpdPort <= NUM_IPD_PORTS));

   switch(ulIpdPort)
   {
      case UART_SCC1: // Port S1
         return UART_SCC1;

      case UART_SCC2: // Port S2
         return UART_SCC2;

      case 3: // Port S3
      // TODO: TBD
      default: // Invalid
         return 0L;
   }
}

/**=== IpdPortAvailable ==================================================

   This function is called to discern whether an IPD Port is
   available or if has been reserved for use by some other process.

   Name        Dir   Description
   ulIpdPort   In    One-based index of IPD ports.

   Returns:  TRUE if the IPD port is available, FALSE otherwise.

   Cautions: FALSE is always be returned on the iVu Express product.

=========================================================================*/
extern BOOL IpdPortAvailable(ULONG ulIpdPort)
{

   switch (ulIpdPort)
   {
      case UART_SCC1:
         if(DllMgmtIsDriverLoaded(PORTS1_UART_DRIVER) && DllMgmtIsProtocolEnabled(PORTS1_UART_DRIVER) )
            return (TRUE);
         else
            return (FALSE);
         break;

      case UART_SCC2:
         if(DllMgmtIsDriverLoaded(PORTS2_UART_DRIVER) && DllMgmtIsProtocolEnabled(PORTS2_UART_DRIVER) )
            return (TRUE);
         else
            return (FALSE);
         break;

      default:
         return (FALSE);
         break;
   }

   return FALSE;
}

/**=== IpdGetPortSwitchState =============================================

   This function will return the state of the associated DIP switch
   or parameter setting which corresponds to the serial channel
   it is using.
   Note that not all serial channels have an associated DIP switch.

   Name        Dir   Description
   ulIpdPort   In    One-based index of IPD ports.

   Returns: 1  - DIP switch ON
            0  - DIP switch OFF
            -1 - Indeterminate

   Cautions: -1 is always be returned on the iVu Express product.

=========================================================================*/
extern INT IpdGetPortSwitchState(ULONG ulIpdPort)
{
   return -1;
}

/**=== IpdGetIPAddress ===================================================

   This function is called to retrieve the IP address of this module.

   Name        Dir     Description
   pIpPars     In/Out  Pointer to a place to dump the information.

   Returns:  None.

   Cautions: None.

=========================================================================*/
extern void IpdGetIPAddress(IPPARS *pIpPars)
{
   ASSERT(pDRVSTAT);

   *pIpPars = pDRVSTAT->ipstat;
}

/*=== IpdIsProductSupportPMT =====================================================

   Called by the IPD code and Local Access to check for PMT support

   Returns: TRUE or FALSE

=========================================================================*/
extern BYTE IpdIsProductSupportPMT(void)
{
   switch ((WORD)Boot(BOOT_GET_PRODUCT_TYPE))
   {
      /* OEM Variant */
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_MECPU_OEM:
      case GEN5_LS_1628:
      /* ALC Variant */
      case GEN5_OF683XT_E2:
      case GEN5_OF683T_E2:
      case GEN5_NGZNDEV:
      case GEN5_OF561_E2:
      case GEN5_OF683_E2:
      case GEN5_TV_UC561:
      case GEN5_TV_UC253_V:
      case GEN5_OF683_ARC:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_OF253A_E2:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_OF141_E2:
      case GEN5_OF342_E2:
      case GEN5_OF342_ARC:
      case GEN5_NGVAVDEV:
      /* CARIER Variant */
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UCXP683T_V:
      case GEN5_TV_UC683T:
      case GEN5_TV_UC683_ARC:
      case GEN5_TV_FC:
      case GEN5_TV_UV:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
      case GEN5_TV_SQ_PSM:
      case GEN5_TV_VVTZC_E2:
      case GEN5_TV_VVTBP_E2:
      case GEN5_TV_VAVB3_E2:
      case GEN5_TV_VVTZC:
      case GEN5_TV_VVTBP:
      case GEN5_TV_VAVB3:
      /* OEM Variant */
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
      case GEN5_SS_342A_E2:
      case GEN5_SS_342A:
         return (TRUE);
   }

   return (FALSE);
}

/*=== IpdIsProductSupportMODBUS =====================================================

   Called by the IPD code and Local Access to check for MODBUS support

   Returns: TRUE, if product type supports modbus IPD

=========================================================================*/
extern BYTE IpdIsProductSupportMODBUS(void)
{
   switch ((WORD)Boot(BOOT_GET_PRODUCT_TYPE))
   {
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_HIGH_TIER_ROUTER_ALC:
      case GEN5_HIGH_TIER_ROUTER_CARRIER:
      case GEN5_MECPU_ALC:
      case GEN5_MECPU_CARRIER:
      case GEN5_MECPU_OEM:
      case GEN5_MECPU_OFHI:
      case GEN5_MECPU_OFHI_A:
      case GEN5_NON_ROUTER_ALC:
      case GEN5_TV_MPCXP1628:
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_OF1628:
      case GEN5_OF1628_NR:
      case GEN5_OF028_NR:
      case GEN5_TV_MPCXP1628_NR:
      case GEN5_NGZNDEV:
      case GEN5_OF683XT_E2:
      case GEN5_OF683T_E2:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UCXP683T_V:
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
      case GEN5_TV_UC683T:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
      case GEN5_TV_SQ_PSM:
      case GEN5_SS_342A_E2:
      case GEN5_SS_342A:
      case GEN5_NGVAVDEV:
            return (TRUE);
   }

   return (FALSE);
}

/**=== DrvSetLinuxEnvironmentForNetwork ==================================

   This function gets the network parameters from the Linux environment,
   if available, for the BACnet/IP network number, the MS/TP network
   number, and/or the MS/TP BAUD.

   Name        Dir   Description
   NA          NA    NA

   Returns:  NA.

   Cautions: This function needs to be called before any the the
             environment overrides would be used by the system for
             initialization.

=========================================================================*/
static void DrvSetLinuxEnvironmentForNetwork(void)
{
   const char *pEnvString;
   ULONG ulMstpBaud;
   WORD wProductType;
   int iRotarySwitchAddress = 0;
   BYTE fGoodProductType = 0;

   wProductType = (WORD)Boot(BOOT_GET_PRODUCT_TYPE);
   Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_TRACE),"%s: wProductType = %d.",__FUNCTION__, wProductType);

   // we want to know if the product type is OK
   switch(wProductType)
   {
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_HIGH_TIER_ROUTER_CARRIER:
      case GEN5_HIGH_TIER_ROUTER_ALC:
      case GEN5_BASE_TIER_ROUTER_CARRIER:
      case GEN5_BASE_TIER_ROUTER_ALC:
      case GEN5_MECPU_ALC:
      case GEN5_MECPU_CARRIER:
      case GEN5_MECPU_OEM:
      case GEN5_MECPU_PTR:
      case GEN5_MECPU_OFHI:
      case GEN5_MECPU_OFHI_A:
      case GEN5_NON_ROUTER_ALC:
      case GEN5_TV_MPCXP1628:
      case GEN5_TV_MPCXP1628_NR:
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_OF1628:
      case GEN5_OF1628_NR:
      case GEN5_OF028_NR:
      case GEN5_OF683XT_E2:
      case GEN5_OF683T_E2:
      case GEN5_NGZNDEV:
      case GEN5_OF683_ARC:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_OF253A_E2:
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UCXP683T_V:
      case GEN5_TV_UC683T:
      case GEN5_TV_UC683_ARC:
      case GEN5_TV_FC:
      case GEN5_TV_UV:
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
      case GEN5_OF561_E2:
      case GEN5_OF683_E2:
      case GEN5_TV_UC561:
      case GEN5_TV_UC253_V:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
      case GEN5_TV_SQ_PSM:
      case GEN5_OF141_E2:
      case GEN5_OF342_E2:
      case GEN5_TV_VVTZC_E2:
      case GEN5_TV_VVTBP_E2:
      case GEN5_TV_VAVB3_E2:
      case GEN5_SS_342A_E2:
      case GEN5_OF342_ARC:
      case GEN5_TV_VVTZC:
      case GEN5_TV_VVTBP:
      case GEN5_TV_VAVB3:
      case GEN5_SS_342A:
      case GEN5_NGVAVDEV:
         fGoodProductType = 1;
         break;

      default:
         Boot(BOOT_LOG_FATAL, (ULONG)0, (ULONG)0, "Invalid Product Type");
         break;
   }

   if (DrvIsHWPortS1Supported() && !IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      // For the boards doesn't have rotary switch to select protocol and mac address.
      // We are loading last saved Mac address and Protocol on Port S1 from ENVIRONMENT Variable
      // This function should not be used for the boards Support rotary Switch
      DrvLoadPortS1Config();
   }
   if (DrvIsHWPortS2Supported())
   {
      DrvLoadPortS2Config();
   }
   // Needed to read rotary switch address at boot to perform automatic device (DrvAutogenerateDeviceIdentity),
   // automatic network (AutoNetGetNetwork), and BpSendConnectedNetworksReport.
   iRotarySwitchAddress = GetCurrentMstpArcnetAddressValueOnPortS1();

   // This Code block IP address more then 128 , Need to discuss and remove this, It seems it is for MSTP Protocol Only.
   if ((iRotarySwitchAddress >= ROTARY_MSTP_MIN) && (iRotarySwitchAddress <= ROTARY_MSTP_MAX))
   {
      sysstat.uLGnetAddr = (USHORT)iRotarySwitchAddress;
      Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: Rotary switch address set to %d",__FUNCTION__, iRotarySwitchAddress);
   }
   else
   {
      // Default to 1.
      sysstat.uLGnetAddr = 1;
      if (iRotarySwitchAddress < 0)
      {
         Boot(BOOT_WARNING,0,"%s: Unable to determine rotary switch address, using default value (%d)", __FUNCTION__,sysstat.uLGnetAddr);
      }
      else
      {
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: Rotary switch address out of range (%d), using default value (%d)", __FUNCTION__,iRotarySwitchAddress,sysstat.uLGnetAddr);
      }
   }

   ValidatePortS1NvMacSettings();
   // if Home Network Index does not equal Port S1 or Port S2, set PORTS1 MAC address to 0 but retain sysstat.uLGnetAddr
   // as it is used by DrvAutogenerateDeviceIdentity, AutoNetGetNetwork and BpSendConnectedNetworksReport
   if ( (bHomeNetworkIndex != NG_PORTS1_NETWORK_IDX) &&
        (bHomeNetworkIndex != NG_PORTS2_NETWORK_IDX))
   {
      bFwexProfileType = IP_HOME_ROUTER_TYPE;
      // After determining operating profile, check whether the MAC address of Port S1 needs to be
      // based on configuration or address rotary due to Carrier with IP/Ethernet or serial home network
      DrvChkReinitPortS1MacCarrier();
   }
   else
   {
      bFwexProfileType = SERIAL_HOME_ROUTER_TYPE;
      DrvSetMacAddrOnPortS1(sysstat.uLGnetAddr);
   }

   Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: PORTS1 MAC=%d sysstat.uLGnetAddr=%d",__FUNCTION__, pDRVSTAT->bPortMACAddr[PORTS1],sysstat.uLGnetAddr);

   // If the product type is no good disable routing by setting the network numbers to 0 and
   // not loading the environment variables.
   // Webctrl can still comm to the module if the address is known
   if (fGoodProductType)
   {
      pEnvString = getenv("EXECB_BACNET_IP_NETWORK");
      if (pEnvString)
      {
         pSyspars->awNet[NG_IP_NETWORK_IDX] = atoi(pEnvString);
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK),"%s: EXECB_BACNET_IP_NETWORK set to %d",__FUNCTION__, pSyspars->awNet[NG_IP_NETWORK_IDX]);
      }

      pEnvString = getenv("EXECB_BACNET_CMNET_NETWORK");
      if (pEnvString)
      {
         pSyspars->awNet[NG_PORTS1_NETWORK_IDX] = atoi(pEnvString);
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: EXECB_BACNET_CMNET_NETWORK set to %d",__FUNCTION__, pSyspars->awNet[NG_PORTS1_NETWORK_IDX]);
      }

      pEnvString = getenv("EXECB_BACNET_CMNET_BAUD");
      if (pEnvString)
      {
         ulMstpBaud = (ULONG)atol(pEnvString);
         switch (ulMstpBaud)
         {
            case DRV_MSTP_BAUD_9600:
            case DRV_MSTP_BAUD_19200:
            case DRV_MSTP_BAUD_38400:
            case DRV_MSTP_BAUD_57600:
            case DRV_MSTP_BAUD_76800:
            case DRV_MSTP_BAUD_115200:
               pDRVPARS->ulPortBaud[PORTS1] = ulMstpBaud;
               Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: EXECB_BACNET_CMNET_BAUD set to %ld",__FUNCTION__, pDRVPARS->ulPortBaud[PORTS1]);
               break;
            /*For IPDs, status will be updated in IpdReset function*/
            default:
               pDRVPARS->ulPortBaud[PORTS1] = DRV_MSTP_BAUD_76800;
               Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: EXECB_BACNET_CMNET_BAUD=%ld is invalid for MS/TP; value dropped!",__FUNCTION__, pDRVPARS->ulPortBaud[PORTS1]);
               break;
         }
      }

      pEnvString = getenv("EXECB_BACNET_TPI_NETWORK");
      if (pEnvString)
      {
         pSyspars->awNet[NG_PORTS2_NETWORK_IDX] = pSysstat->awNet[NG_PORTS2_NETWORK_IDX] = atoi(pEnvString);
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: EXECB_BACNET_TPI_NETWORK = %d",__FUNCTION__, pSyspars->awNet[NG_PORTS2_NETWORK_IDX]);
      }

      pEnvString = getenv("EXECB_BACNET_TPI_ADDRESS");
      if (pEnvString)
      {
         pDRVSTAT->bPortMACAddr[PORTS1] = pDRVPARS->bPortMACAddr[PORTS2]= atoi(pEnvString);
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: EXECB_BACNET_TPI_ADDRESS = %d",__FUNCTION__, pDRVSTAT->bPortMACAddr[PORTS1]);
      }

      pEnvString = getenv("EXECB_BACNET_TPI_BAUD");
      if (pEnvString)
      {
         ulMstpBaud = (ULONG)atol(pEnvString);
         switch (ulMstpBaud)
         {
            case DRV_MSTP_BAUD_9600:
            case DRV_MSTP_BAUD_19200:
            case DRV_MSTP_BAUD_38400:
            case DRV_MSTP_BAUD_57600:
            case DRV_MSTP_BAUD_76800:
            case DRV_MSTP_BAUD_115200:
               Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: EXECB_BACNET_TPI_BAUD set to %ld",__FUNCTION__,pDRVPARS->ulPortBaud[PORTS2]);
               pdevStat->aulPortBaud[PORTS2] = pDRVPARS->ulPortBaud[PORTS2] = ulMstpBaud;
               break;
            default:
               pdevStat->aulPortBaud[PORTS2] = pDRVPARS->ulPortBaud[PORTS2] = DRV_MSTP_BAUD_76800;
               Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: EXECB_BACNET_TPI_BAUD=%ld is invalid for MS/TP; value dropped!",__FUNCTION__, pDRVPARS->ulPortBaud[PORTS2]);
               break;
         }
      }

      pEnvString = getenv("EXECB_BACNET_ETHERNET_NETWORK");
      if (pEnvString)
      {
         pSyspars->awNet[NG_ETHERNET_NETWORK_IDX] = pSysstat->awNet[NG_ETHERNET_NETWORK_IDX] = atoi(pEnvString);
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK),"%s: EXECB_BACNET_ETHERNET_NETWORK = %d",__FUNCTION__, pSyspars->awNet[NG_ETHERNET_NETWORK_IDX]);
      }

      // if ACTNet provided by FPGA, but not available
      pEnvString = getenv("ALCENV_DISABLE_ACTNET");
      if (pEnvString)
      {
          pDRVSTAT->fHWActnetSupported = !atoi(pEnvString);
          if (atoi(pEnvString)) {
              Boot(BOOT_INFO, sysstat.time, "Act Net Disabled");
              //syslog(LOG_INFO, "%s: Act Net Disabled = %d", __FUNCTION__, pDRVSTAT->fHWActnetSupported);
          }
      }
   }
   else
   {
      // our product type is no good so zero out the network numbers and go to a default speed
      // of 76800 for the 485 ports
      pSyspars->awNet[NG_IP_NETWORK_IDX] = 0;
      pSyspars->awNet[NG_PORTS1_NETWORK_IDX] = 0;
      pDRVPARS->ulPortBaud[PORTS1] = DRV_MSTP_BAUD_76800;
      pSyspars->awNet[NG_PORTS2_NETWORK_IDX] = pSysstat->awNet[NG_PORTS2_NETWORK_IDX] = 0;
      pdevStat->aulPortBaud[PORTS2] = pDRVPARS->ulPortBaud[PORTS2] = DRV_MSTP_BAUD_76800;
      pSyspars->awNet[NG_ETHERNET_NETWORK_IDX] = pSysstat->awNet[NG_ETHERNET_NETWORK_IDX] = 0;
   }

}

/**=== DrvSetUniqueEepromSerialNumber ====================================

   Sets the serial number in the fake EEPROM to the ethernet MAC address.

   Name        Dir     Description
   NA          In/Out  NA

   Returns:  None.

   Cautions: Be careful where this is called.

=========================================================================*/
extern void DrvSetUniqueEepromSerialNumber(void)
{
   CHAR *pchMACAddr;   // Ethernet MAC address.
   char achMacString[(6 * 2) + 1];
   int iStringSize;

   // Make sure we have a valid Ethernet address
   pchMACAddr = GetEthernetAddr_100BT();   // sets global pointer to Ethernet MAC address

   if (pchMACAddr)
   {
      memset(achMacString, 0, sizeof(achMacString));

      iStringSize = snprintf(achMacString, sizeof(achMacString), "%02X%02X%02X%02X%02X%02X",
                            (BYTE)pchMACAddr[0],
                            (BYTE)pchMACAddr[1],
                            (BYTE)pchMACAddr[2],
                            (BYTE)pchMACAddr[3],
                            (BYTE)pchMACAddr[4],
                            (BYTE)pchMACAddr[5]  );

      if (iStringSize == (6 * 2))
      {
         memcpy(&(abDrvEECORE[EE_SERIAL_NUMBER]), achMacString, sizeof(achMacString));
         memcpy(&(abDrvEEBASE[EE_SERIAL_NUMBER]), achMacString, sizeof(achMacString));
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_DRIVER),"Pseudo base and code EEPROM serial number set to: %s", achMacString);
      }
      else // This should NOT happen!
      {
         // No need to do anything to the serial number because it is hardcoded to a single ASCII space character.
         // (If the preinitialized image has its serial # change, this code may need updating.)
         Boot(BOOT_WARNING, time (NULL), "EEPROM serial number corrupt!");
         Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_DRIVER),"Ethernet MAC string corrupt: EEPROM serial number not updated! (This should not happen!!!)");
      }
   }
   else
   {
      // No need to do anything to the serial number because it is hardcoded to a single ASCII space character.
      Boot(BOOT_WARNING, time (NULL), "EEPROM serial number not updated!");
      Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_DRIVER),"Ethernet MAC invalid: EEPROM serial number not updated!");
   }
}

/**==== DrvGetBpModelName ===============================================

   This function is used to set the BACnet model_name property in the
   device object.

   Name              Dir   Description
   pszDest           Out   The string buffer to put the name in
   maxlength         In    The max length of the buffer

   Returns:

   Cautions: The function pointer pfnDrvGetBpModelName must be
             set to this function before InitModuleDriverConst() in
             mynode.c is called. (InitModuleDriverConst() is currently
             called in the SysReset() call-stack.)

=========================================================================*/
void DrvGetBpModelName(CHAR *pszDest, size_t maxlength)
{
   // WebCTRL is expecting the BACnet model name to uniquely
   // identify the actual module type.
   switch(Boot(BOOT_GET_PRODUCT_TYPE))
   {
      case GEN5_HIGH_TIER_ROUTER_OEM:
         strncpy(pszDest, "GW-1000", maxlength);
         break;

      case GEN5_HIGH_TIER_ROUTER_CARRIER:
         strncpy(pszDest, "XT-LB", maxlength);
         break;

      case GEN5_HIGH_TIER_ROUTER_ALC:
         strncpy(pszDest, "G5CE", maxlength);
         break;

      case GEN5_BASE_TIER_ROUTER_CARRIER:
         strncpy(pszDest, "XT-RB", maxlength);
         break;

      case GEN5_BASE_TIER_ROUTER_ALC:
         strncpy(pszDest, "G5RE", maxlength);
         break;

      case GEN5_MECPU_ALC:
         strncpy(pszDest, "OFBBC", maxlength);
         break;

      case GEN5_NON_ROUTER_ALC:
         strncpy(pszDest, "OFBBC-NR", maxlength);
         break;

      case GEN5_MECPU_CARRIER:
         strncpy(pszDest, "TV-MPCXP", maxlength);
         break;

      case GEN5_MECPU_OEM:
         strncpy(pszDest, "LS-CPU", maxlength);
         break;

      case GEN5_MECPU_PTR:
         strncpy(pszDest, "G5-MEcpu-PTR", maxlength);
         break;

      case GEN5_MECPU_OFHI:
         strncpy(pszDest, "OFHI", maxlength);
         break;

      case GEN5_MECPU_OFHI_A:
          strncpy(pszDest, "OFHI-A", maxlength);
          break;

      case GEN5_OF1628_NR:
         strncpy(pszDest, "OF1628-NR", maxlength);
         break;

      case GEN5_OF028_NR:
         strncpy(pszDest, "OF028-NR", maxlength);
         break;

      case GEN5_LS_1628:
         strncpy(pszDest, "LS-1628", maxlength);
         break;

      case GEN5_VLINK_IO_1628U:
         strncpy(pszDest, "Vlink-IO-1628u", maxlength);
         break;

      case GEN5_TV_MPCXP1628:
         strncpy(pszDest, "TV-MPCXP1628", maxlength);
         break;

      case GEN5_OF1628:
         strncpy(pszDest, "OF1628", maxlength);
         break;

      case GEN5_TV_MPCXP1628_NR:
         strncpy(pszDest, "TV-MPCXP1628-NR", maxlength);
         break;

      case GEN5_OF683XT_E2:
         strncpy(pszDest, "OF683XT-E2", maxlength);
         break;

      case GEN5_OF683T_E2:
         strncpy(pszDest, "OF683T-E2", maxlength);
         break;

      case GEN5_OF683_ARC:
         strncpy(pszDest, "OF683-ARC", maxlength);
         break;

      case GEN5_OF561T_E2:
         strncpy(pszDest, "OF561T-E2", maxlength);
         break;

      case GEN5_OF253T_E2:
         strncpy(pszDest, "OF253T-E2", maxlength);
         break;

      case GEN5_OF253A_E2:
         strncpy(pszDest, "OF253A-E2", maxlength);
         break;

      case GEN5_TV_UC683T:
         strncpy(pszDest, "TV-UC683T", maxlength);
         break;

      case GEN5_TV_UC683_ARC:
         strncpy(pszDest, "TV-UC683-ARC", maxlength);
         break;

      case GEN5_TV_FC:
         strncpy(pszDest, "TV-FC", maxlength);
         break;

      case GEN5_TV_UV:
         strncpy(pszDest, "TV-UV", maxlength);
         break;

      case GEN5_TV_UCXP683T:
         strncpy(pszDest, "TV-UCXP683T", maxlength);
         break;

      case GEN5_TV_UCXP683T_V:
         strncpy(pszDest, "TV-UCXP683T-V", maxlength);
         break;

      case GEN5_SS_561TX:
         strncpy(pszDest, "SS-561TX", maxlength);
         break;

      case GEN5_SS_683:
         strncpy(pszDest, "SS-683", maxlength);
         break;

      case GEN5_SS_683TX:
         strncpy(pszDest, "SS-683TX", maxlength);
         break;

      case GEN5_SS_683ETX:
         strncpy(pszDest, "SS-683ETX", maxlength);
         break;

      case GEN5_NGZNDEV:
         strncpy(pszDest, "NGZNDEV", maxlength);
         break;

      case GEN5_OF561_E2:
         strncpy(pszDest, "OF561-E2", maxlength);
         break;

      case GEN5_OF683_E2:
         strncpy(pszDest, "OF683-E2", maxlength);
         break;

     case GEN5_TV_UC561:
         strncpy(pszDest, "TV-UC561", maxlength);
         break;

     case GEN5_TV_UC253_V:
         strncpy(pszDest, "TV-UC253-V", maxlength);
         break;

      case GEN5_OF_PSM:
          strncpy(pszDest, "OF-PSM", maxlength);
          break;

      case GEN5_OF_OPT:
          strncpy(pszDest, "OF-OPT", maxlength);
          break;

      case GEN5_TV_PSM:
          strncpy(pszDest, "TV-PSM", maxlength);
          break;

      case GEN5_TV_OPT:
          strncpy(pszDest, "TV-OPT", maxlength);
          break;

      case GEN5_TV_SQ_PSM:
         strncpy(pszDest, "TV-SQ-PSM", maxlength);
         break;

      case GEN5_OF141_E2:
         strncpy(pszDest, "OF141-E2", maxlength);
         break;

      case GEN5_OF342_E2:
         strncpy(pszDest, "OF342-E2", maxlength);
         break;

      case GEN5_TV_VVTZC_E2:
         strncpy(pszDest, "TV-VVTZC-E2", maxlength);
         break;

      case GEN5_TV_VVTBP_E2:
         strncpy(pszDest, "TV-VVTBP-E2", maxlength);
         break;

      case GEN5_TV_VAVB3_E2:
         strncpy(pszDest, "TV-VAVB3-E2", maxlength);
         break;

      case GEN5_SS_342A_E2:
         strncpy(pszDest, "SS-342A-E2", maxlength);
         break;

      case GEN5_OF342_ARC:
         strncpy(pszDest, "OF342-ARC", maxlength);
         break;

      case GEN5_TV_VVTZC:
         strncpy(pszDest, "TV-VVTZC", maxlength);
         break;

      case GEN5_TV_VVTBP:
         strncpy(pszDest, "TV-VVTBP", maxlength);
         break;

      case GEN5_TV_VAVB3:
         strncpy(pszDest, "TV-VAVB3", maxlength);
         break;

      case GEN5_SS_342A:
         strncpy(pszDest, "SS-342A", maxlength);
         break;

      case GEN5_NGVAVDEV:
          strncpy(pszDest, "NGVAVDEV", maxlength);
          break;

      default:
         strncpy(pszDest, "Invalid Product Type", maxlength);
         break;
   }
}
/*==== ApplicationShutdown ==============================================

   Delete or shutdown the resource or process they are running in the
   application.

   Name             Dir   Description

   Returns:
     none.

   Cautions:
       This function is called from ApplicationSysExitHandler().
=========================================================================*/
extern void ApplicationShutdown(void)
{
   // DE5860: Shutdown cp-engine before io-client interface(socket) shutdown,
   // Since this interface using by cp-engine. Same case for Rnet interface
   // also.
   CpEngineShutdown();
   /* Local Network Configuration */
   LocalNetworkTaskEnd();
   /* App-Device Management */
   DeviceMngmtTaskEnd();
   /* Sntp Time Update Task */
   SntpTimeUpdateTaskEnd();

   // shut down the MS/TP and/or ARCNET DLLs
   DllShutdown();

   StatusLEDsReleaseControl();

   IOInterfaceShutdown();

   // unload the PORTS1, and/or PORTS2, and IOnet network device drivers, if loaded
   UnLoadNetworkDrivers(TRUE, 0);

   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      // un-export the GPIOs associated with the rotary switches
      RotaryShutdown();
   }

   // Shutdown the Data Server Task
   ShutdownDataServer();
   ShutdownAppDataServer();

   // Remove event instance buffers
   DeleteEventInstanceBufferPool();
   
   // Delete session timer of Background Download
   DeleteBGDLSessionTimer();

   fprintf(myfp, "\n---------------------FWEX EXIT ---------------------------\n"); fflush(myfp);
   //fclose(myfp);



}
/*==== DeletePacketBufferPool ============================================

   Delete the resource or process they are running in the
   application.

   Name             Dir   Description

   Returns:
     none.

   Cautions:
       This function is called from ApplicationSysExitHandler().
=========================================================================*/
extern void DeletePacketBufferPool(void)
{
   // delete the packet buffer(s)
   if (idBpPacketBufferPool)
   {
      ExecBuffersDelete(idBpPacketBufferPool);
      idBpPacketBufferPool = 0;
   }
}
/*==== DrvGetBaudRateOnPortS1 ==============================================

   Set PORTS2 Baud Rate.

   Name             Dir   Description

   Returns:
     Baud rate

=========================================================================*/
extern ULONG DrvGetBaudRateOnPortS1()
{
   ULONG ulPortS1BaudRate = 0;
   if (pDRVPARS)
   {
      ulPortS1BaudRate = pDRVPARS->ulPortBaud[PORTS1];
   }
   return ulPortS1BaudRate;
}
/*==== DrvGetBaudRateOnPortS2 ==============================================

   Set PORTS2 Baud Rate.

   Name             Dir   Description
   ulPortS2BaudRate    out    PORTS2 Baud Rate

   Returns:
     None

=========================================================================*/
extern ULONG DrvGetBaudRateOnPortS2()
{
   ULONG ulPortS2BaudRate = 0;
   if (pDRVPARS)
   {
      ulPortS2BaudRate = pDRVPARS->ulPortBaud[PORTS2];
   }
   return ulPortS2BaudRate;
}


/*==== SetUdpPortPars ==============================================

   Set UDP port number property in pars structure.

   Name             Dir   Description

   Returns:
     None

=========================================================================*/
extern void SetUdpPortPars(BYTE bPort, WORD wValue)
{
   if (pDRVPARS)
   {
      switch (bPort)
      {
      case IP_CUSTOM_UDP_PORT:
         pDRVPARS->ipnext.ippars.wUdpPort = wValue;
         break;

      case IP_CUSTOM_SEC_UDP_PORT:
         pDRVPARS->ipnext.ippars.wSecUdpPort[IP_SEC_UDP_PORT_IDX] = wValue;
         break;

      default:
         break;
      }
   }
}

/*==== DrvSetBaudRateOnPortS2 ==============================================

   Set PORTS2 Baud Rate.

   Name             Dir   Description
   ulPortS2BaudRate    In    PORTS2 Baud Rate

   Returns:
     None

=========================================================================*/
extern void DrvSetBaudRateOnPortS2(ULONG ulPortS2BaudRate)
{
   if (pDRVPARS)
   {
      if (ValidateBaudRate(ulPortS2BaudRate) == FALSE)
      {
         ulPortS2BaudRate = DRV_MSTP_BAUD_76800;
      }
      pDRVPARS->ulPortBaud[PORTS2] = ulPortS2BaudRate;
      SavePortHWConfig();
   }
}

/*==== DrvSetMstpBaudOnPortS1 =========================================

   Set MSTP baud rate for use on the PORTS1.

   Name             Dir   Description
   ulBaudRate    In    PORTS2 Baud Rate

   Returns:
     None

   Caution: It is assumed that the baud rate to be written to DRV pars
            is valid (e.g. values submitted via Local Access interface).

=========================================================================*/
extern void DrvSetMstpBaudOnPortS1(ULONG ulBaudRate)
{
   if (pDRVPARS)
   {
      if (ValidateBaudRate(ulBaudRate) == FALSE)
      {
         ulBaudRate = DRV_MSTP_BAUD_76800;
      }
      pDRVPARS->ulPortBaud[PORTS1] = ulBaudRate;
      SavePortHWConfig();
   }
}

/*==== DrvSetMacAddrOnPortS1 ==============================================

   Set PORTS1 MAC address / Station ID.

   Name             Dir   Description
   bPortS1Addr      In    PORTS1 MAC address / Station ID

   Returns:
     None

   Note:
      bPortS1Addr is not used used if Carrier with IP/Ethernet home network
=========================================================================*/
extern void DrvSetMacAddrOnPortS1(BYTE bPortS1Addr)
{
   BYTE bAddr = bPortS1Addr;
   BOOL fRotarySupport = IsFeatureSupported(ROTARY_SWITCH_SUPPORT);
   if (pDRVSTAT)
   {
      // if Carrier device functioning with an IP/Ethernet home network (non-serial)
      // Or the next gen zone controller which doesn't have rotary switch support
      // the Port S1 MAC address is settable from Local Access and WebCTRL
      if((BP_VENDOR_ID == BP_VENDOR_ID_CARRIER && bFwexProfileType == IP_HOME_ROUTER_TYPE && fRotarySupport == TRUE) || fRotarySupport == FALSE)
      {
         // Determine which MAC address to use based on protocol selected
         int iProtocol = GetProtocolValueOnPortS1();
         switch (iProtocol)
         {
            case ROTARY_PROTO_ARCNET:
               bAddr = nvhw.bPortS1ARCNETMacAddr;
               if (pSyspars)
               {
                  pSyspars->bPortMACAddrS1ARCnet = bAddr;
                  pDRVSTAT->bPortMACAddr[PORTS1] = bAddr;
               }
               break;
            case ROTARY_PROTO_MSTP:
               bAddr = nvhw.bPortS1MSTPMacAddr;
               if (pDRVPARS)
               {
                  pDRVPARS->bPortMACAddr[PORTS1] = bAddr;
                  pDRVSTAT->bPortMACAddr[PORTS1] = bAddr;
               }
               break;
            case ROTARY_PROTO_MODBUS:
            default:
               break;
         }
         // Check and save the new mac address if it is not match with nvram
         ChkSavePortS1MacToNv();

      }
      else
      {
         pDRVSTAT->bPortMACAddr[PORTS1] = bAddr;
      }
      Boot(BOOT_DEBUG,(DBG_MSG_TRACE),"%s: pDRVSTAT->bPortMACAddr[PORTS1]=%i",
            __FUNCTION__, pDRVSTAT->bPortMACAddr[PORTS1]);
   }
}
/*==== DrvGetMacAddrOnPortS1 ==============================================

   Get PORTS1 MAC address / Station ID.

   Name             Dir   Description
   NA               In/Out  NA

   Returns:
     PORTS1 MAC address / Station ID.

=========================================================================*/
extern BYTE DrvGetMacAddrOnPortS1(void)
{
   if (pDRVSTAT)
   {
      return pDRVSTAT->bPortMACAddr[PORTS1];
   }
   else
   {
      return 0;
   }
}


/*==== DrvGetDllTypeOnPortS1 ======================================

   Get the BACnet DLL type assigned to PORTS1.

   Name             Dir   Description


   Returns:
     The BACnet DLL type selected (via DIP switch) on PORTS1
     if the module driver has been downloaded.  Otherwise, DLL_DISABLED
     will be indicated.

=========================================================================*/
extern BYTE DrvGetDllTypeOnPortS1(void)
{
   if (pDRVSTAT)
   {
      return pDRVSTAT->bPortDLLType[PORTS1];
   }
   else
   {
      return DLL_DISABLED;
   }
}

/*==== DrvGetDllTypeOnPortS2 ======================================

   Get the BACnet DLL type assigned to PORTS2.

   Name             Dir   Description


   Returns:
     The BACnet DLL type selected (via DIP switch or Local UI) on PORTS2
     if the module driver has been downloaded.  Otherwise, DLL_DISABLED
     will be indicated.

=========================================================================*/
extern BYTE DrvGetDllTypeOnPortS2(void)
{
   if (pDRVSTAT)
   {
      return pDRVSTAT->bPortDLLType[PORTS2];
   }
   else
   {
      return DLL_DISABLED;
   }
}
/*==== DrvSetDLLTypeOnPortS1 ==============================================

   Set Driver Status parameter to indicate which DLL Type the PORTS1
   port is configured as.

   Name             Dir   Description
   bPortS1DLLType    In    DLL Type of PORTS1 port when configured

   Returns:
     none.

=========================================================================*/
extern void DrvSetDLLTypeOnPortS1(BYTE bPortS1DLLType)
{
   if (pDRVSTAT)
   {
      pDRVSTAT->bPortDLLType[PORTS1] = bPortS1DLLType;
   }
}
/*==== DrvSetDLLTypeOnPortS2 ==============================================

   Set Driver Status parameter to indicate which DLL Type the PORTS1
   port is configured as.

   Name             Dir   Description
   bPortS2DLLType    In    DLL Type of PORTS1 port when configured

   Returns:
     none.

=========================================================================*/
extern void DrvSetDLLTypeOnPortS2(BYTE bPortS2DLLType)
{
   if (pDRVSTAT)
   {
      pDRVSTAT->bPortDLLType[PORTS2] = bPortS2DLLType;
   }
}

/*==== DrvSetHomeNetworkIndex =========================================

   Set Home Network Index and operating profile then update other processes.

   Name             Dir   Description

   Returns:
     None
=========================================================================*/
extern void DrvSetHomeNetworkIndex(BYTE bRequestedHomeNetworkIndex)
{

   if (bRequestedHomeNetworkIndex < NG_HOME_NETWORK_IDX)
   {
      switch (bRequestedHomeNetworkIndex)
      {
         case NG_IP_NETWORK_IDX:
         case NG_ETHERNET_NETWORK_IDX:
         default:
            bFwexProfileType = IP_HOME_ROUTER_TYPE;
            break;

         case NG_PORTS1_NETWORK_IDX:
         case NG_PORTS2_NETWORK_IDX:
            bFwexProfileType = SERIAL_HOME_ROUTER_TYPE;
            break;
      }
      // After changing the home network, check whether the MAC address of Port S1 needs to
      // be changed due to Carrier with IP/Ethernet or serial home network
      DrvChkReinitPortS1MacCarrier();

      //
      // Perform housekeeping on Home Network Index
      //
      bHomeNetworkIndex = bRequestedHomeNetworkIndex;
      nvhw.bSavedHomeNetworkIndex = bHomeNetworkIndex;
      //
      // DE3641: Cannot download via Rnet when using Field Assistant
      // When field assistant issues the CPT22 Firmware Upgrade clear memory,
      // the IP network number is set to 0 which prevents field assistant from
      // being able to communicate to the device again, causing the download
      // to fail.
      // Save the last known home address in non-volatile memory for recovery
      // on next restart to allow field assistant download to complete
      //
      nvhw.wSavedHomeNetworkParsValue = pSyspars->awNet[bHomeNetworkIndex];
      nvhw.wSavedHomeNetworkStatValue = pSysstat->awNet[bHomeNetworkIndex];
      nvhw.wSavedHomeNetworkHistValue = pSyshist->awNet[bHomeNetworkIndex];
      Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_NETWORK|DBG_MSG_TRACE),"%s: idx = %d pars = %d stat = %d hist = %d",
            __FUNCTION__, nvhw.bSavedHomeNetworkIndex, nvhw.wSavedHomeNetworkParsValue, nvhw.wSavedHomeNetworkStatValue, nvhw.wSavedHomeNetworkHistValue);
      NVRamSave();

      //
      // Alert relevant processes to potential operating profile change
      //
      DllMgmtSetProfile(bFwexProfileType, bHomeNetworkIndex);
      AutoNetSetProfile(bFwexProfileType, bHomeNetworkIndex);
      AutoIdSetProfile(bFwexProfileType,  bHomeNetworkIndex);
   }
}

/*==== DrvGetHomeNetwork =========================================

   Get the home network number.

   Name             Dir   Description

   Returns:
     None
=========================================================================*/
extern WORD DrvGetHomeNetwork(void)
{
   return pSysstat->awNet[bHomeNetworkIndex];
}
/*==== GetPortDataPointers ==============================================
   Return pointers to certain status and parameter values to indicate facilitate
   DLL configuration and control

   Name                  Dir          Description
   pPortBaudRates        out          pointer to Baud Rate array parameter in Driver Pars
   pPortMACAddr_PARS     out          pointer to MSTP MAC Address parameter in Driver Pars
   pPortMACAddr_STAT     out          pointer to MSTP MAC Address parameter in Driver Stat
   pPortDLLType          out          pointer to DLL Type status parameter in Driver Stat

   Returns:
     none.

=========================================================================*/
extern void GetPortDataPointers(ULONG **pPortBaudRates, BYTE **pPortMACAddr_PARS, BYTE **pPortMACAddr_STAT, BYTE **pPortDLLType)
{
   if ((pDRVPARS) && (pDRVSTAT))
   {
      if (pPortBaudRates) *pPortBaudRates = &pDRVPARS->ulPortBaud[0];
      if (pPortMACAddr_PARS) *pPortMACAddr_PARS = &pDRVPARS->bPortMACAddr[0];
      if (pPortMACAddr_STAT) *pPortMACAddr_STAT = &pDRVSTAT->bPortMACAddr[0];
      if (pPortDLLType) *pPortDLLType = &pDRVSTAT->bPortDLLType[0];

   }
   else
   {
      if (pPortBaudRates) *pPortBaudRates = NULL;
      if (pPortMACAddr_PARS) *pPortMACAddr_PARS = NULL;
      if (pPortMACAddr_STAT) *pPortMACAddr_STAT = NULL;
      if (pPortDLLType) *pPortDLLType = NULL;
   }
}

/*==== DrvSetMacAddrOnPort2 ==============================================

   Set PORTS2 MAC address / Station ID.

   Name             Dir   Description
   bPortS2Addr       In    PORTS2 MAC address / Station ID

   Returns:
     None

=========================================================================*/
extern void DrvSetMacAddrOnPort2(BYTE bPortS2Addr)
{
   if (CheckPortS1Address(ROTARY_PROTO_MSTP,bPortS2Addr) == FALSE)
   {
      bPortS2Addr = 0;
   }
   if (pDRVPARS)
   {
      pDRVPARS->bPortMACAddr[PORTS2] = bPortS2Addr;
   }
   if (pDRVSTAT)
   {
      pDRVSTAT->bPortMACAddr[PORTS2] = bPortS2Addr;
   }
   SavePortHWConfig();
}
/*==== DrvGetMacAddrOnPortS2 ==============================================

   Get PORTS1 MAC address / Station ID.

   Name             Dir   Description
   NA               In/Out  NA

   Returns:
     PORTS1 MAC address / Station ID.

=========================================================================*/
extern BYTE DrvGetMacAddrOnPortS2(void)
{
   if (pDRVSTAT)
   {
      return pDRVSTAT->bPortMACAddr[PORTS2];
   }
   else
   {
      return 0;
   }
}

/*==== DrvHasPortS1ConfigChanged =======================================

   Monitor DRVPARS for a PORTS1 port configuration change
   If config has changed, return TRUE.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     TRUE/FALSE

=========================================================================*/
extern BOOL DrvHasPortS1ConfigChanged(void)
{
   // Saved PORTS1 MSTP baud rate
   static ULONG ulLastPortS1Baud = 0xFFFFFFFE; //initialize to invalid value to force evaluation
   static BYTE bLastMaxMaster = 0xFF;
   static USHORT usMaxInfoFrames = 0xFFFF;
   static BYTE bLastMacAddr = 0xFF;
   static BYTE bMSTPLastAutobaud;
   BOOL fRetValue=FALSE;
   BOOL fSaveConfig=FALSE;
   BOOL fRotarySupport = IsFeatureSupported(ROTARY_SWITCH_SUPPORT);

   if (pDRVPARS)
   {
      if (pDRVPARS->ulPortBaud[PORTS1] != ulLastPortS1Baud)
      {
         switch (pDRVPARS->ulPortBaud[PORTS1])
         {
            case DRV_MSTP_BAUD_9600:
            case DRV_MSTP_BAUD_19200:
            case DRV_MSTP_BAUD_38400:
            case DRV_MSTP_BAUD_57600:
            case DRV_MSTP_BAUD_76800:
            case DRV_MSTP_BAUD_115200:
               if (ulLastPortS1Baud != 0xFFFFFFFE)
               {
                  Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: %s Baud changed from %lu to %lu",
                     __FUNCTION__,DrvGetPortS1Name(),ulLastPortS1Baud,pDRVPARS->ulPortBaud[PORTS1]);
                  fSaveConfig=TRUE;
               }
                pdevStat->aulPortBaud[PORTS1] = ulLastPortS1Baud = pDRVPARS->ulPortBaud[PORTS1];
                fRetValue=TRUE;
               break;
            default:
               if (ulLastPortS1Baud != 0xFFFFFFFE)
               {
               // if invalid baud rate, ignore and use 76800 baud
                  Boot(BOOT_WARNING,0,"%s baud error (%ld) using %d",DrvGetPortS1Name(),pDRVPARS->ulPortBaud[PORTS1],DRV_MSTP_BAUD_76800);
               }
               // check whether already at 76800 (if not using 76800, return TRUE)
               fRetValue=(pdevStat->aulPortBaud[PORTS1] != DRV_MSTP_BAUD_76800);
               pdevStat->aulPortBaud[PORTS1] = ulLastPortS1Baud = pDRVPARS->ulPortBaud[PORTS1] = DRV_MSTP_BAUD_76800;
               break;
         }
      }
      if (bLastMaxMaster != pdevPars->abMaxMaster[PORTS1])
      {
         if (bLastMaxMaster != 0xFF)
         {
            Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: %s max master changed from %d to %d",__FUNCTION__,DrvGetPortS1Name(),bLastMaxMaster,pdevPars->abMaxMaster[PORTS1]);
         }
         bLastMaxMaster = pdevPars->abMaxMaster[PORTS1];
         fRetValue=TRUE;
      }
      if (usMaxInfoFrames != pdevPars->ausMaxInfoFrames[PORTS1])
      {
         if (usMaxInfoFrames != 0xFFFF)
         {
            Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: %s max info frames changed from %d to %d",__FUNCTION__,DrvGetPortS1Name(),usMaxInfoFrames,pdevPars->ausMaxInfoFrames[PORTS1]);
         }
         usMaxInfoFrames = pdevPars->ausMaxInfoFrames[PORTS1];
         fRetValue=TRUE;
      }
      // if Carrier device functioning with an IP/Ethernet home network (non-serial)
      // Or the next gen zone controller which doesn't have rotary switch support
      // the Port S1 MAC address is settable from Local Access and WebCTRL
      if((BP_VENDOR_ID == BP_VENDOR_ID_CARRIER && bFwexProfileType == IP_HOME_ROUTER_TYPE && fRotarySupport == TRUE) || fRotarySupport == FALSE)
      {
         // Determine which MAC address to use based on protocol selected
         int iProtocol = GetProtocolValueOnPortS1();
         switch (iProtocol)
         {
            case ROTARY_PROTO_ARCNET:
               if (bLastMacAddr != pSyspars->bPortMACAddrS1ARCnet)
               {
                  pDRVSTAT->bPortMACAddr[PORTS1] = pSyspars->bPortMACAddrS1ARCnet;
               }
               break;
            case ROTARY_PROTO_MSTP:
               if (bLastMacAddr != pDRVPARS->bPortMACAddr[PORTS1])
               {
                  pDRVSTAT->bPortMACAddr[PORTS1] = pDRVPARS->bPortMACAddr[PORTS1];
               }
               break;
            case ROTARY_PROTO_MODBUS:
            default:
               break;
         }
         if (bLastMacAddr != pDRVSTAT->bPortMACAddr[PORTS1])
         {
            if (bLastMacAddr != 0xFF)
            {
               Boot(BOOT_DEBUG,(DBG_MSG_TRACE|DBG_MSG_STARTUP|DBG_MSG_MSTP|DBG_MSG_ARC),"%s: %s MAC Addr changed from %d to %d",__FUNCTION__,DrvGetPortS1Name(),bLastMacAddr,pDRVSTAT->bPortMACAddr[PORTS1]);
            }
            bLastMacAddr = pDRVSTAT->bPortMACAddr[PORTS1];
            ChkSavePortS1MacToNv();
            fRetValue=TRUE;
            if(!IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
            {
               // For the boards doesn't have rotary switch to select protocol and mac address.
               // We are loading last saved Mac address and Protocol on Port S1 from ENVIRONMENT Variable
               // This function should not be used for the boards Support rotary Switch
               UpdatePortS1File(NULL);
            }
         }
      }

      //Identify MSTP Autobaud changes if any
      if(bMSTPLastAutobaud != pDRVPARS->bMstpAutobaud[PORTS1])
      {
         fRetValue = TRUE;
         fSaveConfig=TRUE;
         bMSTPLastAutobaud = pDRVPARS->bMstpAutobaud[PORTS1];
         BpDllSetMstpAutobaudFlags(PORTS1,bMSTPLastAutobaud);
         DrvUpdateAutobaudStats((ULONG)PORTS1,"N/A", 0);
      }

      if( pdevStat->fPortS1AutoSync)
      {
         BpDllSetMstpAutobaudSyncFlags(PORTS1,pdevStat->fPortS1AutoSync);
         pdevStat->fPortS1AutoSync = 0;
      }
   }
   if (fSaveConfig==TRUE)
   {
      SavePortHWConfig();
   }
   return(fRetValue);
}
/*==== DrvHasPortS2ConfigChanged =======================================

   Monitor DRVPARS for a PORTS2 port configuration change
   If config has changed, return TRUE.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     TRUE/FALSE

=========================================================================*/
extern BOOL DrvHasPortS2ConfigChanged(void)
{
   // Saved PORTS2 MSTP baud rate
   static ULONG ulLastPortS2Baud = 0xFFFFFFFE; //initialize to invalid value to force evaluation
   static BYTE bLastMaxMaster = 0xFF;
   static USHORT usMaxInfoFrames = 0xFFFF;
   static BYTE bLastMacAddr = 0xFF;
   static BYTE bMSTPLastAutobaud;
   BOOL fRetValue=FALSE;
   BOOL fSaveConfig=FALSE;

   if (pDRVPARS)
   {
      if (pDRVPARS->ulPortBaud[PORTS2] != ulLastPortS2Baud)
      {
         switch (pDRVPARS->ulPortBaud[PORTS2])
         {
            case DRV_MSTP_BAUD_9600:
            case DRV_MSTP_BAUD_19200:
            case DRV_MSTP_BAUD_38400:
            case DRV_MSTP_BAUD_57600:
            case DRV_MSTP_BAUD_76800:
            case DRV_MSTP_BAUD_115200:
               if (ulLastPortS2Baud != 0xFFFFFFFE)
               {
                  Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: %s Baud changed from %lu to %lu",__FUNCTION__,DrvGetPortS2Name(),ulLastPortS2Baud,pDRVPARS->ulPortBaud[PORTS2]);
                  fSaveConfig=TRUE;
               }
               pdevStat->aulPortBaud[PORTS2] = ulLastPortS2Baud = pDRVPARS->ulPortBaud[PORTS2];
               fRetValue=TRUE;
               break;
            default:
               if (ulLastPortS2Baud != 0xFFFFFFFE)
               {
               // if invalid baud rate, ignore and use 76800 baud
                  Boot(BOOT_WARNING,0,"%s baud error (%ld) using %d",DrvGetPortS2Name(),pDRVPARS->ulPortBaud[PORTS2],DRV_MSTP_BAUD_76800);
               }
               // check whether already at 76800 (if not using 76800, return TRUE)
               fRetValue=(pdevStat->aulPortBaud[PORTS2] != DRV_MSTP_BAUD_76800);
               pdevStat->aulPortBaud[PORTS2] = ulLastPortS2Baud = pDRVPARS->ulPortBaud[PORTS2] = DRV_MSTP_BAUD_76800;
               break;
         }
      }
      if (bLastMaxMaster != pdevPars->abMaxMaster[PORTS2])
      {
         if (bLastMaxMaster != 0xFF)
         {
            Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: %s max master from %d to %d",__FUNCTION__,DrvGetPortS2Name(),bLastMaxMaster,pdevPars->abMaxMaster[PORTS2]);
         }
         bLastMaxMaster = pdevPars->abMaxMaster[PORTS2];
         fRetValue=TRUE;
      }
      if (usMaxInfoFrames != pdevPars->ausMaxInfoFrames[PORTS2])
      {
         if (usMaxInfoFrames != 0xFFFF)
         {
            Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: %s max info frames changed from %d to %d",__FUNCTION__,DrvGetPortS2Name(),usMaxInfoFrames,pdevPars->ausMaxInfoFrames[PORTS2]);
         }
         usMaxInfoFrames = pdevPars->ausMaxInfoFrames[PORTS2];
         fRetValue=TRUE;
      }
      if (pDRVSTAT)
      {
         if (bLastMacAddr != pDRVPARS->bPortMACAddr[PORTS2])
         {
            if (bLastMacAddr != 0xFF)
            {
               Boot(BOOT_DEBUG,(DBG_MSG_STARTUP|DBG_MSG_MSTP),"%s: %s MAC Addr changed from %d to %d",__FUNCTION__,DrvGetPortS2Name(),bLastMacAddr,pDRVPARS->bPortMACAddr[PORTS2]);
               fSaveConfig=TRUE;
            }
            bLastMacAddr = pDRVSTAT->bPortMACAddr[PORTS2] = pDRVPARS->bPortMACAddr[PORTS2];
            fRetValue=TRUE;
         }
      }

      //Identify MSTP Autobaud changes if any
      if(bMSTPLastAutobaud != pDRVPARS->bMstpAutobaud[PORTS2])
      {
         fRetValue = TRUE;
         fSaveConfig=TRUE;
         bMSTPLastAutobaud = pDRVPARS->bMstpAutobaud[PORTS2];
         BpDllSetMstpAutobaudFlags(PORTS2,bMSTPLastAutobaud);
         DrvUpdateAutobaudStats((ULONG)PORTS2,"N/A",0);
      }

      if( pdevStat->fPortS2AutoSync)
      {
         BpDllSetMstpAutobaudSyncFlags(PORTS2,pdevStat->fPortS2AutoSync);
         pdevStat->fPortS2AutoSync = 0;
      }
   }
   if (fSaveConfig==TRUE)
   {
      SavePortHWConfig();
   }
   return(fRetValue);
}

/*==== DrvGetEndOfNetworkSwitch =======================================

   Retrieves the status of the End Of Network Switches

   Name                     Dir   Description
   iPortIndex               in    The port index of PORTS1 or PORTS2

   Returns:
     The status of the End of Network Switches

=========================================================================*/
extern BOOL DrvGetEndOfNetworkSwitch(int iPortIndex)
{
   BOOL fRetVal = FALSE;
   if (pDRVSTAT && ((iPortIndex == PORTS1) || (iPortIndex == PORTS2)))
   {
      fRetVal =  pDRVSTAT->fPortEndOfNetwork[iPortIndex];
   }
   return fRetVal;
}
/**=== DrvGetPortS1Name ================================================

   Get the name Port S1/BAS

   Name        Dir   Description
      none

   Returns:
      Port S1/BAS name

   Cautions:

=========================================================================*/
extern char *DrvGetPortS1Name(void)
{
   return pszPortS1Name;
}
/**=== DrvGetPortS2Name ================================================

   Get the name Port S2/Device

   Name        Dir   Description
      none

   Returns:
      Port S1/BAS name

   Cautions:

=========================================================================*/
extern char *DrvGetPortS2Name(void)
{
   return pszPortS2Name;
}

/*==== UpdateDevStat =======================================

   Refresh Device Status Fields

   Name                     Dir   Description
   N/A                      N/A   none

   Returns:
     None

=========================================================================*/
extern void UpdateDevStat(void)
{
   if ((pdevStat) && (pSysstat))
   {
      pdevStat->awPortNet[PORTS1] = pSysstat->awNet[NG_PORTS1_NETWORK_IDX];
      pdevStat->awPortNet[PORTS2] = pSysstat->awNet[NG_PORTS2_NETWORK_IDX];
   }

   // Update the End of Network Switches
   if (pDRVSTAT)
   {
      int iSwitchPosition = 0;

      // Update the PORTS1 End of Network Switch
      iSwitchPosition = GetPortS1TerminationValue();
      switch (iSwitchPosition)
      {
      case 0: pDRVSTAT->fPortEndOfNetwork[PORTS1] = FALSE; break;  // Switch is off
      case 1: pDRVSTAT->fPortEndOfNetwork[PORTS1] = TRUE; break;  // Switch in on
      default: pDRVSTAT->fPortEndOfNetwork[PORTS1] = FALSE; break; // Error occured while trying to read the switch
      }
      // Update the MSTP (PORTS2) End of Network Switch
      iSwitchPosition = GetPortS2TerminationValue();
      switch (iSwitchPosition)
      {
      case 0: pDRVSTAT->fPortEndOfNetwork[PORTS2] = FALSE; break;  // Switch is off
      case 1: pDRVSTAT->fPortEndOfNetwork[PORTS2] = TRUE; break;  // Switch in on
      default: pDRVSTAT->fPortEndOfNetwork[PORTS2] = FALSE; break; // Error occured while trying to read the switch
      }
   }
}


/*==== DrvGetBACnetFirewallEnable ========================================

   The Local Access interface calls this function to retrieve the current
   value of the BACnet Firewall Enable parameter.  If the module driver
   has not been downloaded, the function returns FALSE.

   Name             Dir   Description
   N/A              N/A   N/A

   Returns:  The current boolean value of the BACnet Firewall Enable
             parameter, fEnableBacnetFirewall, if the module driver
             has been downloaded. Otherwise, the function returns FALSE.

   Cautions: None.

=========================================================================*/
extern BOOL DrvGetBACnetFirewallEnable(void)
{
   if (pDRVPARS)
   {
       return pDRVPARS->bfPars.fEnableBacnetFirewall;
   }
   else
   {
       return FALSE;
   }
}

/*==== DrvDisableBACnetFireWall ==========================================

   The Local Access interface calls this function to disable BACnet Firewall
   protection by setting the driver parameter, fEnableBACnetFirewall, to
   FALSE.  If it is already FALSE, nothing more needs to happen.

   Name             Dir   Description
   N/A              N/A   N/A

   Returns:  A boolean flag indicating whether the parameter was actually
             changed to FALSE.

   Cautions: None.

=========================================================================*/
extern BOOL DrvDisableBACnetFirewall(void)
{
   BOOL fParChanged = FALSE;

   if (pDRVPARS)
   {
      if (pDRVPARS->bfPars.fEnableBacnetFirewall)
      {
         pDRVPARS->bfPars.fEnableBacnetFirewall = FALSE;
         fParChanged = TRUE;
      }
   }

   return fParChanged;
}

/**=== DrvGetIpInfo ======================================================
   Driver specific function call to retrieve IP configuration settings.

   Name        Dir   Description
   bIpType     In    Identifier specifying the type of information
                     to return.

   Returns:
      A ULONG representation of the desired information,
      or 0 if not found.

   Cautions:

=========================================================================*/
extern ULONG DrvGetIpInfo(BYTE bIpType)
{
   ULONG ulValue = 0;
   switch (bIpType)
   {
      case IP_CURRENT_ADDR:
         ulValue = ulMyIPAddr;
         break;
      case IP_CURRENT_SUBNET_MASK:
         ulValue = ulMySubnetMask;
         break;
      case IP_CURRENT_GATEWAY:
         ulValue = ulMyGatewayIPAddr;
         break;
      case IP_CUSTOM_ADDR:
         ulValue = ippars.ulIPAddress;
         break;
      case IP_CUSTOM_SUBNET_MASK:
         ulValue = ippars.ulSubnetMask;
         break;
      case IP_CUSTOM_GATEWAY:
         ulValue = ippars.ulGatewayIPAddress;
         break;
      case IP_CURRENT_UDP_PORT:
         ulValue = (ULONG)wMyUdpPort;
         break;
      case IP_CUSTOM_UDP_PORT:
         ulValue = (ULONG)ippars.wUdpPort;
         break;
      case IP_CURRENT_SEC_UDP_PORT:
         ulValue = (ULONG)wMySecUdpPort;
         break;
      case IP_CUSTOM_SEC_UDP_PORT:
         ulValue = (ULONG)ippars.wSecUdpPort[IP_SEC_UDP_PORT_IDX];
         break;
      default:
         break;
   }
   return ulValue;
}

/**=== DrvSetUdp =====================================================
   Driver specific function called to affect modification to BACnetIP network
   UDP Port value.

   Name        Dir   Description
   bDLLType    In    Data link type

   Returns:
      IP Port number status associated with the BACnet IP Network.

   Cautions:

=========================================================================*/
extern void DrvSetUdp(WORD wPortNumber)
{
   ippars.wUdpPort = wPortNumber;   // Update UDP Port number

   // THIS ALSO REQUIRES A RESET OF THE MODULE!!!!!
   // The restart is required because IP and UDP configuration are
   // stored externally and handled by the operating system. The
   // restart is handled by IpChangeFromNetworkPortObject().

   return;
}

/**=== DrvGetNetwork =====================================================
   Driver specific function called from the bacview router screen code
   which returns the network number status associated with the
   specified data link type.
   Note that for some drivers, the network number returned by this
   function may differ from the parameter setting retrieved by
   a DrvFindNetwork() call (above).

   Name        Dir   Description
   bDLLType    In    Data link type

   Returns:
      Network number status associated with the specified data link type.

   Cautions:

=========================================================================*/
extern WORD DrvGetNetwork(BYTE bDLLType)
{
   // Return present network number based on BACnet DLL type.
   switch (bDLLType)
   {
      case DLL_MSTP:
         return NET_NUMBER(NG_PORTS2_NETWORK_IDX);
         break;

      case DLL_ARCNET: // NOTE: MSTP on S1 borrows the DLL_ARCNET identity
         return NET_NUMBER(NG_PORTS1_NETWORK_IDX);
         break;

      case DLL_IP:
         return NET_NUMBER(NG_IP_NETWORK_IDX);
         break;

      case DLL_ETHERNET:
         return NET_NUMBER(NG_ETHERNET_NETWORK_IDX);
         break;

      default:
         return 0;
         break;
   }
}



/**=== DrvSetIP =========================================================
   Name        Dir   Description
   bIpType     In    IP data type being written (Address, Subnet Mask, Default Gateway)
   bIndex      In    Byte offset into data type
   vValue      In    data value to be written into IP data type

   Returns:
      Network number status associated with the specified data link type.

   Cautions:

=========================================================================*/
extern void DrvSetIP(BYTE bIpType, BYTE *pbValue)
{
   BYTE bIndex;
   BYTE *pbIpSnippet = NULL;

   switch (bIpType)
   {
      case IP_CUSTOM_ADDR:
         pbIpSnippet = (BYTE *)&ippars.ulIPAddress;
         break;
      case IP_CUSTOM_SUBNET_MASK:
         pbIpSnippet = (BYTE *)&ippars.ulSubnetMask;
         break;
      case IP_CUSTOM_GATEWAY:
         pbIpSnippet = (BYTE *)&ippars.ulGatewayIPAddress;
         break;
      default:
         break;
   }

   if (pbIpSnippet)
   {
      for (bIndex = 0; bIndex < 4; bIndex++, pbIpSnippet++, pbValue++)
      {
         *pbIpSnippet = *pbValue;
      }
   }

   // THIS ALSO REQUIRES A RESET OF THE MODULE!!!!!
   // The restart is required because IP and UDP configuration are
   // stored externally and handled by the operating system. The
   // restart is handled by IpChangeFromNetworkPortObject().

   return;
}

/*==== SetNetworkPortObjectScreenSupport =================================
   Sets the bit field(s) in the NETWORK PORT DEVICE COM DATA structure that
   indicate which screen data types are supported by this device.

   Name               Dir      Description
   aDeviceComData     In       CPRUN pointer for the module driver

 =========================================================================*/
extern void SetNetworkPortObjectScreenSupport(void *pDeviceComData)
{
   //
   // For Non-Router products, Router screen is not allowed to be enabled and byte order Endianness is set to little-endian.
   //
   switch(Boot(BOOT_GET_PRODUCT_TYPE))
   {
      case GEN5_HIGH_TIER_ROUTER_OEM: //GW-1000
      case GEN5_HIGH_TIER_ROUTER_CARRIER: //XT-LB
      case GEN5_HIGH_TIER_ROUTER_ALC: //G5CE
      case GEN5_BASE_TIER_ROUTER_CARRIER: //XT-RB
      case GEN5_BASE_TIER_ROUTER_ALC: //G5RE
      case GEN5_MECPU_ALC: //OFBBC
      case GEN5_MECPU_CARRIER: //TV-MPCXP
      case GEN5_MECPU_OEM: //LS-CPU
      case GEN5_MECPU_PTR: //G5-MEcpu-PTR
      case GEN5_MECPU_OFHI:
      case GEN5_MECPU_OFHI_A:
      case GEN5_OF1628:
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_TV_MPCXP1628:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
        ((NETWORK_PORT_DEVICE_COM_DATA *)pDeviceComData)->bScreenSupported |= (TIME_MASTER_SUPPORTED + ROUTER_SCREEN_SUPPORTED + IP_SCREEN_SUPPORTED + LITTLE_ENDIAN_DATA);
         break;
      case GEN5_NON_ROUTER_ALC:   //OFBBC_NR
      case GEN5_OF028_NR:
      case GEN5_TV_MPCXP1628_NR:
      case GEN5_OF1628_NR:
      case GEN5_OF683XT_E2:
      case GEN5_OF683T_E2:
      case GEN5_OF683_ARC:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_OF253A_E2:
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UCXP683T_V:
      case GEN5_TV_UC683T:
      case GEN5_TV_UC683_ARC:
      case GEN5_TV_FC:
      case GEN5_TV_UV:
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
      case GEN5_NGZNDEV:
      case GEN5_OF561_E2:
      case GEN5_OF683_E2:
      case GEN5_TV_UC561:
      case GEN5_TV_UC253_V:
      case GEN5_TV_SQ_PSM:
      case GEN5_OF141_E2:
      case GEN5_OF342_E2:
      case GEN5_TV_VVTZC_E2:
      case GEN5_TV_VVTBP_E2:
      case GEN5_TV_VAVB3_E2:
      case GEN5_SS_342A_E2:
      case GEN5_OF342_ARC:
      case GEN5_TV_VVTZC:
      case GEN5_TV_VVTBP:
      case GEN5_TV_VAVB3:
      case GEN5_SS_342A:
      case GEN5_NGVAVDEV:
      default:
         ((NETWORK_PORT_DEVICE_COM_DATA *)pDeviceComData)->bScreenSupported |= (TIME_MASTER_SUPPORTED + IP_SCREEN_SUPPORTED + LITTLE_ENDIAN_DATA);
         break;
   }
}

/**=== DrvFindNetwork ====================================================
   Driver specific function called from the bacview router screen code
   which returns the location of the network number parameter
   associated with the specified data link type.

   Name        Dir   Description
   bDLLType    In    Data link type

   Returns:
      Pointer to the network number parameter associated with the
      specified data link type.

   Cautions:

=========================================================================*/
extern WORD *DrvFindNetwork( BYTE bDLLType)
{
   // Return ptr to awNet[ ] array based on BACnet DLL type.
   switch (bDLLType)
   {
      case DLL_IP:
         return &pSyspars->awNet[NG_IP_NETWORK_IDX];
         break;

      case DLL_MSTP:
         return &pSyspars->awNet[NG_PORTS2_NETWORK_IDX];
         break;

      case DLL_ARCNET: // NOTE: MSTP on S1 borrows the DLL_ARCNET identity
         return &pSyspars->awNet[NG_PORTS1_NETWORK_IDX];
         break;

      case DLL_ETHERNET:
         return &pSyspars->awNet[NG_ETHERNET_NETWORK_IDX];
         break;

      default:
         return NULL;
         break;
   }
}
/*==== SavePortHWConfig =============================================
   This subroutine is called when hardware configuration changes.
   When called this subroutine saves the current baudrates and mac addresses
   of the PORTS2 and PORTS1 ports to permit ride through a device reinit as
   would occur during a module download.

=========================================================================*/
static void SavePortHWConfig(void)
{
   if (pDRVPARS && pSyspars && pnvSystemMemory)
   {
     nvhw.ulPortS1BaudRate = pDRVPARS->ulPortBaud[PORTS1];
     nvhw.ulPortS2BaudRate = pDRVPARS->ulPortBaud[PORTS2];
     nvhw.bPortS2MACAddr   = pDRVPARS->bPortMACAddr[PORTS2];
     nvhw.bPortS1ARCNETMacAddr = pSyspars->bPortMACAddrS1ARCnet;
     nvhw.bPortS1MSTPMacAddr = pDRVPARS->bPortMACAddr[PORTS1];
     nvhw.bPortS1MstpAutobaud = pDRVPARS->bMstpAutobaud[PORTS1];
     nvhw.bPortS2MstpAutobaud = pDRVPARS->bMstpAutobaud[PORTS2];
     NVRamSave();
     Boot(BOOT_DEBUG,(DBG_MSG_TRACE),"%s: ",
           __FUNCTION__);
   }
}
/*==== DrvLoadPortS1Config =============================================
   This subroutine is called to get the last configurations on PortS1
   Protocol selected, Mac address for ARCNET and MSTP for the boards
   doesn't have rotary switch support.

=========================================================================*/
static void DrvLoadPortS1Config(void)
{
   const char *pEnvString;
   const char cProtocolStringPortS1[ROTARY_MAX_PORTS1_PROTOCOLS][10] = { "NONE",
                                                                         "MS/TP",
                                                                         "ARCNET",
                                                                       "MODBUS" };

   if (IsFeatureSupported(ARCNET_SUPPORT))
   {
      pEnvString = getenv(ENVIRONMENT_ARCNET_ADDRESS);
      if (pEnvString)
      {
         pSyspars->bPortMACAddrS1ARCnet = atoi(pEnvString);
         nvhw.bPortS1ARCNETMacAddr = pSyspars->bPortMACAddrS1ARCnet;
         Boot(BOOT_DEBUG, (DBG_MSG_DLL_MGMT|DBG_MSG_NETWORK|DBG_MSG_DRIVER|DBG_MSG_ARC),
               "%s arcnet address %d", DrvGetPortS1Name(), pSyspars->bPortMACAddrS1ARCnet);
      }
   }   

   if (IsFeatureSupported(MSTP_SUPPORT))
   {
      pEnvString = getenv(ENVIRONMENT_MSTP_ADDRESS);
      if (pEnvString)
      {
         pDRVPARS->bPortMACAddr[PORTS1] = atoi(pEnvString);
         nvhw.bPortS1MSTPMacAddr = pDRVPARS->bPortMACAddr[PORTS1];
         Boot(BOOT_DEBUG, (DBG_MSG_DLL_MGMT|DBG_MSG_NETWORK|DBG_MSG_DRIVER|DBG_MSG_MSTP),
               "%s mstp address %d", DrvGetPortS1Name(), pDRVPARS->bPortMACAddr[PORTS1]);
      }
   }

   pEnvString = getenv(ENVIRONMENT_PORTS1_PROTOCOL);
   if (pEnvString)
   {
      nvhw.bPortS1Protocol = atoi(pEnvString);
      Boot(BOOT_DEBUG, (DBG_MSG_DLL_MGMT|DBG_MSG_NETWORK|DBG_MSG_DRIVER),
            "%s protocol changed to %s", DrvGetPortS1Name(), cProtocolStringPortS1[nvhw.bPortS1Protocol]);
   }
}

/*==== DrvLoadPortS2Config =============================================
   This subroutine is called to get the last configurations on PortS2
   Protocol selected, Mac address for MSTP for the boards.

=========================================================================*/
static void DrvLoadPortS2Config(void)
{
   // Load with Disable as Default value for STAT, Status will be updated on DLLMgmt
   pDRVSTAT->bProtocolSelectedOnPort[PORTS2] = nvhw.bPortS2Protocol;
   pDRVSTAT->bPortDLLType[PORTS2] = DLL_DISABLED;

   /* PortS2 MAC Address */
   pDRVSTAT->bPortMACAddr[PORTS2] = nvhw.bPortS2MACAddr;
}

/*==== ValidatePortS1NvMacSettings =============================================
   Checks whether the Port S1 nvhw MAC setting are in the proper range

   Name        Dir   Description
   bAddr       In    MAC address to use for Port S1 ARCnet

   Returns:
      None

   Cautions:
      This function should only be called after NVRamLoad
=========================================================================*/
static void ValidatePortS1NvMacSettings(void)
{
   if (CheckPortS1Address(ROTARY_PROTO_ARCNET, nvhw.bPortS1ARCNETMacAddr) == FALSE)
   {
      nvhw.bPortS1ARCNETMacAddr = 254;
   }
   if (CheckPortS1Address(ROTARY_PROTO_MSTP, nvhw.bPortS1MSTPMacAddr) == FALSE)
   {
      nvhw.bPortS1MSTPMacAddr = 0;
   }
}
/*==== ChkSavePortS1MacToNv =============================================
   Check whether to save the nvhw Port S1 MAC address values

   Name        Dir   Description
   bAddr       In    MAC address to use for Port S1 ARCnet

   Returns:
      None

   Note:
      This routine is used to detect changes made by WebCTRL and
      update the nvhw settings.
=========================================================================*/
static void ChkSavePortS1MacToNv(void)
{
   BOOL fSave = FALSE;
   if (pDRVPARS && pSyspars)
   {
      // Determine which MAC address to use based on protocol selected
      int iProtocol = GetProtocolValueOnPortS1();
      switch (iProtocol)
      {
         case ROTARY_PROTO_ARCNET:
            if (nvhw.bPortS1ARCNETMacAddr != pSyspars->bPortMACAddrS1ARCnet)
            {
               fSave = TRUE;
            }
            break;
         case ROTARY_PROTO_MSTP:
            if (nvhw.bPortS1MSTPMacAddr != pDRVPARS->bPortMACAddr[PORTS1])
            {
               fSave = TRUE;
            }
            break;
         case ROTARY_PROTO_MODBUS:
         default:
            break;
      }
      if (fSave)
      {
         SavePortHWConfig();
         if(!IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
         {
            // For the boards doesn't have rotary switch to select protocol and mac address.
            // We are loading last saved Mac address and Protocol on Port S1 from ENVIRONMENT Variable
            // This function should not be used for the boards Support rotary Switch
            UpdatePortS1File(NULL);
         }
      }
   }
}
/*==== DrvChkReinitPortS1MacCarrier ====================================
   Check whether the Port S1 MAC address needs to be reinitialized due
   to a Carrier router with an IP/Ethernet home netwwork (non-serial)

   Name        Dir   Description

   Returns:
      None

   Note:
      To be BACnet Rev 14 compliant (Addendum 135-2010ak-1 Specify Address Range
      Requirements) the MAC address of Port S1 must be settable to any valid value.
      It is not allowed to be hardcoded to 0 for MSTP and 254 for ARCnet (as was done
      for Carrier).

      A Carrier product with an IP/Ethernet home network will use a configuration
      (available on Local Access and WebCTRL) to set the MAC address of Port S1

      A Carrier product with a serial home network will use the address rotary
      to set the MAC address of Port S1.

      When functioning as a Carrier product with an IP/Ethernet home network,
      the following must be kept in sync whether the change was originated via
      Local Access or WebCTRL:
         nvhw.bPortS1ARCNETMacAddr <-> pSyspars->bPortMACAddrS1ARCnet
         nvhw.bPortS1MSTPMacAddr   <-> pDRVPARS->bPortMACAddr[PORTS1]

=========================================================================*/
extern void DrvChkReinitPortS1MacCarrier(void)
{
   BOOL fRotarySupport = IsFeatureSupported(ROTARY_SWITCH_SUPPORT);
   // if Carrier device determine what to use for MAC address on Port S1
   // Or the next gen zone controller which doesn't have rotary switch support
   // i.e., address rotary or configuration
   if ((BP_VENDOR_ID == BP_VENDOR_ID_CARRIER))
   {
      // Determine the the MAC address to use for Port S1
      DrvSetMacAddrOnPortS1(sysstat.uLGnetAddr);
   }
}
/*==== DrvSetNvhwProtoOnS1 ====================================
   Set the nvhw Port S1 ARCnet mac address.

   Name        Dir   Description
   bProto      In    Protocol for Port S1

   Returns:
      None

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern void DrvSetNvhwProtoOnS1(BYTE bProto)
{
   if (bProto != nvhw.bPortS1Protocol)
   {
      nvhw.bPortS1Protocol = bProto;
      SavePortHWConfig();
      if(!IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
      {
         // For the boards doesn't have rotary switch to select protocol and mac address.
         // We are loading last saved Mac address and Protocol on Port S1 from ENVIRONMENT Variable
         // This function should not be used for the boards Support rotary Switch
         UpdatePortS1File(NULL);
      }
   }
}
/*==== DrvSetNvhwARCnetMacAddr ====================================
   Set the nvhw Port S1 ARCnet mac address.

   Name        Dir   Description
   bAddr       In    MAC address to use for Port S1 ARCnet

   Returns:
      None

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern void DrvSetNvhwARCnetMacAddr(BYTE bAddr)
{
   BOOL fRotarySupport = IsFeatureSupported(ROTARY_SWITCH_SUPPORT);

   if (CheckPortS1Address(ROTARY_PROTO_ARCNET, bAddr) == TRUE)
   {
      // if Carrier device functioning with an IP/Ethernet home network (non-serial)
      // the Port S1 MAC address is settable from Local Access and WebCTRL
      if ((BP_VENDOR_ID == BP_VENDOR_ID_CARRIER && bFwexProfileType == IP_HOME_ROUTER_TYPE && fRotarySupport == TRUE) || fRotarySupport == FALSE)
      {
         pSyspars->bPortMACAddrS1ARCnet = bAddr;
         if (bAddr != nvhw.bPortS1ARCNETMacAddr)
         {
            SavePortHWConfig();
            if(!IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
            {
               // For the boards doesn't have rotary switch to select protocol and mac address.
               // We are loading last saved Mac address and Protocol on Port S1 from ENVIRONMENT Variable
               // This function should not be used for the boards Support rotary Switch
               UpdatePortS1File(NULL);
            }
         }
         // Why this API need to be call from here we saved mac address on SYSPars,NVRAM and Updated Port S1 config file also Just for STAT ..!
         // if we are calling this function above line should be removed
         DrvSetMacAddrOnPortS1(bAddr);
      }
      Boot(BOOT_DEBUG,(DBG_MSG_TRACE),"%s: bAddr=%i",
            __FUNCTION__, bAddr);
   }
}
/*==== DrvSetNvhwMstpMacAddr ====================================
   Set the nvhw port s1 MSTP mac address.

   Name        Dir   Description
   bAddr       In    MAC address to use for Port S1 MSTP

   Returns:
      None

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern void DrvSetNvhwMstpMacAddr(BYTE bAddr)
{
   BOOL fRotarySupport = IsFeatureSupported(ROTARY_SWITCH_SUPPORT);

   if (CheckPortS1Address(ROTARY_PROTO_MSTP, bAddr) == TRUE)
   {

      // if Carrier device functioning with an IP/Ethernet home network (non-serial) Or the next gen zone controller which doesn't have rotary support.
      // the Port S1 MAC address is settable from Local Access and WebCTRL
      if ((BP_VENDOR_ID == BP_VENDOR_ID_CARRIER && bFwexProfileType == IP_HOME_ROUTER_TYPE && fRotarySupport == TRUE) || (fRotarySupport == FALSE))
      {
         pDRVPARS->bPortMACAddr[PORTS1] = bAddr;
         if (bAddr != nvhw.bPortS1MSTPMacAddr)
         {
            SavePortHWConfig();
            if(!IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
            {
               // For the boards doesn't have rotary switch to select protocol and mac address.
               // We are loading last saved Mac address and Protocol on Port S1 from ENVIRONMENT Variable
               // This function should not be used for the boards Support rotary Switch
               UpdatePortS1File(NULL);
            }
         }
         // Why this API need to be call from here we saved mac address on DRVPars,NVRAM and Updated Port S1 config file also, Just for STAT ..!
         // if we are calling this function above line should be removed
         DrvSetMacAddrOnPortS1(bAddr);
      }
      Boot(BOOT_DEBUG,(DBG_MSG_TRACE),"%s: bAddr=%i",
            __FUNCTION__, bAddr);
   }
}
/*==== DrvGetModbusMacAddr ====================================
   Get the port s1 Modbus mac address.

   Name        Dir   Description

   Returns:
      IPD_MODBUS_PARS->bOurAddress

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern BYTE DrvGetModbusMacAddr(void)
{
   BYTE bModbusMacAddr = ROTARY_MODBUS_MIN;
   ULONG ulVal = 0;

   IpdGetModbusSrlPars(MSRL1_SLAVE_ADDRESS, &ulVal);
   bModbusMacAddr = (BYTE)ulVal;

   return (bModbusMacAddr);
}
/*==== DrvGetNvhwARCnetMacAddr ====================================
   Get the nvhw port s1 ARCnet mac address.

   Name        Dir   Description

   Returns:
      pSyspars->bPortMACAddrS1ARCnet

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern BYTE DrvGetNvhwARCnetMacAddr(void)
{
   BYTE bArcnetMacAddr = ROTARY_ARCNET_MIN;

   if (pSyspars)
   {
      bArcnetMacAddr = pSyspars->bPortMACAddrS1ARCnet;
   }

   return (bArcnetMacAddr);
}
/*==== DrvGetNvhwMstpMacAddr ====================================
   Get the nvhw port s1 MSTP mac address.

   Name        Dir   Description

   Returns:
      pDRVPARS->bPortMACAddr[PORTS1]

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern BYTE DrvGetNvhwMstpMacAddr(void)
{
   BYTE bMstpMacAddr = ROTARY_MSTP_MIN;
   
   if (pDRVPARS) 
   {
      bMstpMacAddr = pDRVPARS->bPortMACAddr[PORTS1];
   }
   
   return (bMstpMacAddr);
}
/*==== DrvGetProtocolOnPortS1 ====================================
   Get the nvhw port s1 MSTP mac address.

   Name        Dir   Description

   Returns:
      nvhw.bPortS1Protocol value

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern BYTE DrvGetProtocolOnPortS1(void)
{
   BYTE bPortProtocolPortS1 = ROTARY_PROTO_NONE;

   if(pnvSystemMemory)
   {
      bPortProtocolPortS1 = nvhw.bPortS1Protocol;
   }

   return (bPortProtocolPortS1);
}
/*==== DrvGetVendorId ====================================
   Get the vendor ID

=========================================================================*/
extern unsigned int DrvGetVendorId(void)
{
   return BP_VENDOR_ID;
}
/*==== ValidateBaudRate =============================================
   Validate whether the supplied baud rate is valid

   Name        Dir   Description
   ulBaudRate  In    Baud rate to check

   Returns:
      TRUE - baud rate is valid
      FALSE - baud rate is not valid

   Cautions:

=========================================================================*/
static BOOL ValidateBaudRate(ULONG ulBaudRate)
{
   BOOL fRetValue=FALSE;

   switch (ulBaudRate)
   {
      case DRV_MSTP_BAUD_9600:
      case DRV_MSTP_BAUD_19200:
      case DRV_MSTP_BAUD_38400:
      case DRV_MSTP_BAUD_57600:
      case DRV_MSTP_BAUD_76800:
      case DRV_MSTP_BAUD_115200:
         fRetValue=TRUE;
         break;
      default:
         fRetValue=FALSE;
         break;
   }
   return fRetValue;
}

/*==== WriteNatRoutingEnable =============================================
   Write NAT routing enable flag to pars

   Name        Dir   Description
   fValue      In    Flag to write

   Returns:
      TRUE - write successful
      FALSE - write unsuccessful

   Cautions:
      pDRVPARS should be instantiated before calling

=========================================================================*/
extern BOOL WriteNatRoutingEnable(BOOL fValue)
{
   BOOL fSuccess = FALSE;

   if (pDRVPARS)
   {
      pDRVPARS->ipnat.fEnableNatRouting = fValue;
      fSuccess = TRUE;
   }

   return fSuccess;
}

/*==== ReadNatRoutingEnable =============================================
   Read NAT routing enable flag from pars

   Name        Dir   Description
   fValue      Out   enable flag from write

   Returns:
      TRUE - read successful
      FALSE - read unsuccessful

   Cautions:
      pDRVPARS should be instantiated before calling

=========================================================================*/
extern BOOL ReadNatRoutingEnable(BOOL *fValue)
{
   BOOL fSuccess = FALSE;

   if (pDRVPARS)
   {
      *fValue = pDRVPARS->ipnat.fEnableNatRouting;
      fSuccess = TRUE;
   }

   return fSuccess;
}

/*==== WriteNatGlobalUdpPort =============================================
   Write NAT global UDP port to pars

   Name        Dir   Description
   wValue      In    port number to write

   Returns:
      TRUE - write successful
      FALSE - write unsuccessful

   Cautions:
      pDRVPARS should be instantiated before calling

=========================================================================*/
extern BOOL WriteNatGlobalUdpPort(WORD wValue)
{
   BOOL fSuccess = FALSE;

   if (pDRVPARS)
   {
      pDRVPARS->ipnat.wNatGlobalUdpPort = wValue;
      fSuccess = TRUE;
   }

   return fSuccess;
}
/*==== ReadNatGlobalUdpPort =============================================
   Read NAT global UDP port from pars

   Name        Dir   Description
   wValue      Out   port number from pars

   Returns:
      TRUE - read successful
      FALSE - read unsuccessful

   Cautions:
      pDRVPARS should be instantiated before calling

=========================================================================*/
extern BOOL ReadNatGlobalUdpPort(WORD *wValue)
{
   BOOL fSuccess = FALSE;

   if (pDRVPARS)
   {
      *wValue = pDRVPARS->ipnat.wNatGlobalUdpPort;
      fSuccess = TRUE;
   }

   return fSuccess;
}
/*==== WriteNatGlobalIpAddress =============================================
   Write NAT global IP address to pars

   Name        Dir   Description
   ulValue      In    IP address number to write

   Returns:
      TRUE - write successful
      FALSE - write unsuccessful

   Cautions:
      pDRVPARS should be instantiated before calling

=========================================================================*/
extern BOOL WriteNatGlobalIpAddress(ULONG ulValue)
{
   BOOL fSuccess = FALSE;

   if (pDRVPARS)
   {
      pDRVPARS->ipnat.ulNatGlobalIPAddress = htonl(ulValue);
      fSuccess = TRUE;
   }

   return fSuccess;
}
/*==== ReadNatGlobalIpAddress =============================================
   Read NAT global IP address from pars

   Name        Dir   Description
   ulValue     Out   IP address from pars

   Returns:
      TRUE - read successful
      FALSE - read unsuccessful

   Cautions:
      pDRVPARS should be instantiated before calling

=========================================================================*/
extern BOOL ReadNatGlobalIpAddress(ULONG *ulValue)
{
   BOOL fSuccess = FALSE;

   if (pDRVPARS)
   {
      *ulValue = ntohl(pDRVPARS->ipnat.ulNatGlobalIPAddress);
      fSuccess = TRUE;
   }

   return fSuccess;
}

/**==== GetPacketCapStat =================================================

   Gets the packet capture-related parameters from the DRVSTAT structure.

   Name                    Dir     Description
   pPacketCapStatStruct    In/Out  pointer to the structure where the
                                   packet capture parameters will be
                                   placed

   Returns:
   none

   Cautions:
   pDRVSTAT should be instantiated before calling

=========================================================================*/
extern void GetPacketCapStat(void *pPacketCapStatStruct)
{
   if (pPacketCapStatStruct && pDRVSTAT)
   {
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.fCaptureAllPackets      = (pDRVSTAT->PortCapture.PortS1PortCapture.usCaptureAllPackets ? TRUE : FALSE);
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.usCaptureType           = pDRVSTAT->PortCapture.PortS1PortCapture.usCaptureMode;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.fCaptureStartStopMode   = (pDRVSTAT->PortCapture.PortS1PortCapture.usCaptureStart ? LATER : NOW);
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.fCaptureCommand         = pDRVSTAT->PortCapture.PortS1PortCapture.fCaptureCommand;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.ulCaptureStartDate      = pDRVSTAT->PortCapture.PortS1PortCapture.ulCaptureStartDate;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.ulCaptureStartTime      = pDRVSTAT->PortCapture.PortS1PortCapture.ulCaptureStartTime;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.bCaptureDurationHours   = pDRVSTAT->PortCapture.PortS1PortCapture.bCaptureDurationHours;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.bCaptureDurationMinutes = pDRVSTAT->PortCapture.PortS1PortCapture.bCaptureDurationMinutes;

      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.fCaptureAllPackets         = (pDRVSTAT->PortCapture.PortS2PortCapture.usCaptureAllPackets ? TRUE : FALSE);
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.usCaptureType              = pDRVSTAT->PortCapture.PortS2PortCapture.usCaptureMode;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.fCaptureStartStopMode      = (pDRVSTAT->PortCapture.PortS2PortCapture.usCaptureStart ?  LATER : NOW);
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.fCaptureCommand            = pDRVSTAT->PortCapture.PortS2PortCapture.fCaptureCommand;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.ulCaptureStartDate         = pDRVSTAT->PortCapture.PortS2PortCapture.ulCaptureStartDate;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.ulCaptureStartTime         = pDRVSTAT->PortCapture.PortS2PortCapture.ulCaptureStartTime;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.bCaptureDurationHours      = pDRVSTAT->PortCapture.PortS2PortCapture.bCaptureDurationHours;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.bCaptureDurationMinutes    = pDRVSTAT->PortCapture.PortS2PortCapture.bCaptureDurationMinutes;

      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.fCaptureAllPackets          = (pDRVSTAT->PortCapture.GigEPortCapture.usCaptureAllPackets ? TRUE : FALSE);
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.usCaptureType               = pDRVSTAT->PortCapture.GigEPortCapture.usCaptureMode;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.fCaptureStartStopMode       = (pDRVSTAT->PortCapture.GigEPortCapture.usCaptureStart ?  LATER : NOW);
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.fCaptureCommand             = pDRVSTAT->PortCapture.GigEPortCapture.fCaptureCommand;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.ulCaptureStartDate          = pDRVSTAT->PortCapture.GigEPortCapture.ulCaptureStartDate;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.ulCaptureStartTime          = pDRVSTAT->PortCapture.GigEPortCapture.ulCaptureStartTime;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.bCaptureDurationHours       = pDRVSTAT->PortCapture.GigEPortCapture.bCaptureDurationHours;
      ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.bCaptureDurationMinutes     = pDRVSTAT->PortCapture.GigEPortCapture.bCaptureDurationMinutes;
   }
}

/**==== GetPacketCapStatPortCapture ======================================

   Gets a copy of the packet capture-related parameters from the DRVSTAT
   structure.

   Name                    Dir     Description
   pPortCapture            Out     pointer to the structure to which the
                                   parameters will be copied

   Returns:
   none

   Cautions:
   pDRVSTAT should be instantiated before calling

=========================================================================*/
extern void GetPacketCapStatPortCapture(void *pPortCapture)
{
   if (pPortCapture && pDRVSTAT)
   {
      memcpy(pPortCapture, &pDRVSTAT->PortCapture, sizeof(pDRVSTAT->PortCapture));
   }
}

/**==== SetPacketCapStatusCommand =================================================

   Sets the packet capture command values to the values supplied.  Used
   for resetting the capture save value to simulate a pseudo-momentary
   parameter.

   Name                    Dir     Description
   pPacketCapStatStruct    In/Out  pointer to the structure where the
                                   packet capture parameters will be
                                   placed

   Returns:
   none

   Cautions:
   pDRVPARS should be instantiated before calling.  The GetPacketCapStat
   function should be called immediately before calling this function
   in order to prevent missing newly-set values.

=========================================================================*/
extern void SetPacketCapStatusCommand(void *pPacketCapStatStruct)
{
   if (pPacketCapStatStruct && pDRVSTAT)
   {
      pDRVSTAT->PortCapture.PortS1PortCapture.fCaptureCommand = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.fCaptureCommand;
      pDRVSTAT->PortCapture.PortS2PortCapture.fCaptureCommand       = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.fCaptureCommand;
      pDRVSTAT->PortCapture.GigEPortCapture.fCaptureCommand       = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.fCaptureCommand;
   }
}

/**==== SetPacketCapStartDateTime =======================================

   Sets the packet capture Start Date and Time values to the values supplied.

   Name                    Dir     Description
   pPacketCapStatStruct    In/Out  pointer to the structure where the
                                   packet capture date and time parameters are
                                   retrieved

   Returns:
   none

   Cautions:
   pDRVPARS should be instantiated before calling.

=========================================================================*/
extern void SetPacketCapStartDateTime(void *pPacketCapStatStruct)
{
   if (pPacketCapStatStruct && pDRVSTAT)
   {
      pDRVSTAT->PortCapture.PortS1PortCapture.ulCaptureStartDate = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.ulCaptureStartDate;
      pDRVSTAT->PortCapture.PortS1PortCapture.ulCaptureStartTime = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS1PortStat.ulCaptureStartTime;
      pDRVSTAT->PortCapture.PortS2PortCapture.ulCaptureStartDate       = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.ulCaptureStartDate;
      pDRVSTAT->PortCapture.PortS2PortCapture.ulCaptureStartTime       = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->PortS2PortStat.ulCaptureStartTime;
      pDRVSTAT->PortCapture.GigEPortCapture.ulCaptureStartDate       = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.ulCaptureStartDate;
      pDRVSTAT->PortCapture.GigEPortCapture.ulCaptureStartTime       = ((PACKET_CAP_STAT *)pPacketCapStatStruct)->EthPortStat.ulCaptureStartTime;
   }
}

/**==== SetPacketCapStatusState =========================================

   Sets the packet capture state values to the values supplied.  Used
   for notifying the UI of the current capture state.

   Name                    Dir     Description
   bDLLType                in      DLL Type of interface
   bDLLInstance            in      enumeration of DLL
   bCaptureState           in      State of Capture (IDLE, PENDING, ACTIVE)

   Returns:
   none

   Cautions:
   pDRVPARS should be instantiated before calling.

=========================================================================*/
extern void SetPacketCapStatusState(BYTE bDLLType, BYTE bDLLInstance, BYTE bCaptureState)
{
   if (pDRVSTAT)
   {
      switch (bDLLType)
      {
         case DLL_MSTP:
         case DLL_MSTP_SLAVE:
            switch(bDLLInstance)
            {
               case 0:
                  if (pDRVSTAT->PortCapture.PortS1PortCapture.usCaptureState != bCaptureState)
                  {
                     pDRVSTAT->PortCapture.PortS1PortCapture.usCaptureState = bCaptureState;
                     PacketCapSave(&pDRVSTAT->PortCapture);
                  }
                  break;

               case 1:
                  if (pDRVSTAT->PortCapture.PortS2PortCapture.usCaptureState != bCaptureState)
                  {
                     pDRVSTAT->PortCapture.PortS2PortCapture.usCaptureState = bCaptureState;
                     PacketCapSave(&pDRVSTAT->PortCapture);
                  }
                  break;

               default:
                  break;

            }
            break;

         case DLL_IP:
         case DLL_ETHERNET:
            if (pDRVSTAT->PortCapture.GigEPortCapture.usCaptureState != bCaptureState)
            {
               pDRVSTAT->PortCapture.GigEPortCapture.usCaptureState = bCaptureState;
               PacketCapSave(&pDRVSTAT->PortCapture);
            }
            break;

         case DLL_ARCNET:
            if (pDRVSTAT->PortCapture.PortS1PortCapture.usCaptureState != bCaptureState)
            {
               pDRVSTAT->PortCapture.PortS1PortCapture.usCaptureState = bCaptureState;
               PacketCapSave(&pDRVSTAT->PortCapture);
            }
            break;
         default:
           break;
      }
   }
}

/**==== GetPacketCapStatPtr ==============================================

   Returns the pointer to the start of the packet capture status
   space.

   Name                    Dir     Description

   Returns:
   void * - pointer to the start of the packet capture status space,
   or NULL if pDRVSTAT is uninitialized.

   Cautions:

=========================================================================*/
extern void *GetPacketCapStatPtr(void)
{
   if (pDRVSTAT)
   {
      return (void *)&pDRVSTAT->PortCapture;
   }
   else
   {
      return NULL;
   }
}

/**==== GetEthernetStatisticsStructPtr ===================================

   "Gets" the location of the Ethernet Statistics

   Returns: Pointer to Ethernet Statistics structure or NULL if Driver Status
   uninitalized.

=========================================================================*/
extern void *GetEthernetStatisticsStructPtr(void)
{
   //
   // If pointer to Driver Status is available then
   // return the address of the Ethernet Statistics
   // structure
   //
   if (pDRVSTAT)
   {
      return ((void *)&pDRVSTAT->EthernetStatistics);
   }
   else
   {
      return NULL;
   }
}

/**==== GetARCnetStatisticsStructPtr ===================================

   "Gets" the location of the ARCnet Statistics

   Returns: Pointer to ARCnet Statistics structure or NULL if Driver Status
   uninitalized.

=========================================================================*/
extern void *GetARCnetStatisticsStructPtr(void)
{
   //
   // If pointer to Driver Status is available then
   // return the address of the ARCnet Statistics
   // structure
   //
   if (pDRVSTAT)
   {
      return ((void *)&pDRVSTAT->ARCnetStatistics);
   }
   else
   {
      return NULL;
   }
}

/**==== GetMSTPStatisticsStructPtr ===================================

   "Gets" the location of the MSTP Statistics

   Name                    Dir     Description
   bMSTPPort               In      enumeration of MSTP Port statistics data
                                   requested

   Returns: Pointer to MSTP Statistics structure or NULL if Driver Status
   uninitalized.

=========================================================================*/
extern void *GetMSTPStatisticsStructPtr(BYTE bMSTPPort)
{
   //
   // If pointer to Driver Status is available then
   // return the address of the MSTP Statistics
   // structure
   //
   if ((pDRVSTAT) && (bMSTPPort < 2))
   {
      if (bMSTPPort)
      {
         return ((void *)&pDRVSTAT->MSTP1Statistics);
      }
      else
      {
         return ((void *)&pDRVSTAT->MSTP0Statistics);
      }
   }
   else
   {
      return NULL;
   }
}

/**==== RouterStatisticsIncrementCounter ===================================

   Increment the enumerated Router Statistic counter

   Name                    Dir     Description
   fBroadcastPacket        In      boolean indicated wheter Broadcast(TRUE) or
                                   Unicast(FALSE) counter should be incremented

   usVal = (((DRVPARS *)(pcp->pPars))->bNumTelnetSessions);
   if(usVal != bTelnetFileCnt)
   {
      bTelnetFileCnt =usVal;
      DrvTelnetLogCnt(bTelnetFileCnt);
      //If old value  and new value are same don't call the function
      ValidateCapturedFiles();
   }

   // Detect Enable Diag parameter changes.
   if(!fEnableDiag && ((DRVPARS *)(pcp->pPars))->fEnableDiag)
   {
     // Start telnet diagnostic tasks.
     fEnableDiag = TRUE;
     TelnetServerStart(0);
   }
   else if(fEnableDiag && !((DRVPARS *)(pcp->pPars))->fEnableDiag)
   {
      TelnetSetTaskShutdown();
      fEnableDiag = FALSE;
      // Turning this feature OFF requires a reset.
      //Boot(BOOT_RESET, sysstat.time, "DIAG off - resources freed.");
   }

   Returns: None
=========================================================================*/
extern void RouterStatisticsIncrementCounter(BOOL fBroadcastPacket)
{
   //
   // If pointer to Driver Status is available then
   // return the address of the Router Statistics
   // structure
   //
   if (pDRVSTAT)
   {
      if (fBroadcastPacket)
      {
         pDRVSTAT->RoutedPacketsStatistic.ulRouterSourcedBroadcastCount++;
      }
      else
      {
         pDRVSTAT->RoutedPacketsStatistic.ulRouterSourcedUnicastCount++;
      }
   }
}

/**==== GetRouterStatisticsStructPtr ===================================

   "Gets" the location of the Router Statistics

   Returns: Pointer to Router Statistics structure or NULL if Driver Status
   uninitalized.

=========================================================================*/
extern void *GetRouterStatisticsStructPtr(void)
{
   //
   // If pointer to Driver Status is available then
   // return the address of the Router Statistics
   // structure
   //
   if (pDRVSTAT)
   {
      return ((void *)&pDRVSTAT->RoutedPacketsStatistic);
   }
   else
   {
      return NULL;
   }
}

/**==== DrvGetIOModuleInformation ===================================

   Updates IO Module Information in Expander Status structures when
   IO is supported. Also retrieves the IO Subsystem specific statuses
   and IO Subsystem generated critical modstat messages

   Called from CpTask().

   Returns: None

=========================================================================*/
extern void DrvGetIOModuleInformation(void)
{
   //
   // Retrieve the data to populate the IOnet and Xnet
   // property pages
   //
   GetInternalIOModuleInformation(pDRVSTAT->aInternalIOpackInfo);
   GetExternalIOModuleInformation(pDRVSTAT->aExpanderInfo, &pDRVSTAT->bNumberOfIOnetExpanders, &pDRVSTAT->bNumberOfXnetExpanders);

   /* Delaying Actnet info as new paramters got updated, this will block updating
    * stat space parallel */
   if((fActnetConfigUpdate != TRUE) && (pcpModuleDriver->fDoReinit != TRUE))
   {
      GetACTnetInformation(pDRVSTAT->aACTnetInfo,pDRVSTAT->aACTnetConfig);
   }

    /*Calling IpdReset in fixed interval affecting modbus serial/tcp functionality
      hence commenting below line
   IpdResetDll(pcpModuleDriver);*/

   //
   // Retrieve the IO Subsystem statuses and modstat messages
   //
   GetIOSubsystemInformation(pDRVSTAT->aExpanderFirmwareInventory);
}

/**==== DrvSetIOModuleCommParameters =====================================

   Checks for changes to the IOnet/Xnet comm parameters on the Ionet/Xnet
   status page and sends any new/updated values to the I/O subsystem.

   Called from CpTask().

   Returns: None

=========================================================================*/
extern void DrvSetIOModuleCommParameters(void)
{
   static EXPANDER_PARS stCurrentExpPars;

   if (memcmp(&pDRVPARS->ExpanderPars, &stCurrentExpPars, sizeof(EXPANDER_PARS)))
   {
      stCurrentExpPars = pDRVPARS->ExpanderPars;
      SetIoSubsystemParameters(&stCurrentExpPars);
   }
}

/**==== DrvGetFlowSensInfo ==================================================

   Updates Flow sensor Module Information
   Called from CpTask().

   Returns: None

=========================================================================*/
extern void DrvGetFlowSensInfo(void)
{
   INT iFlowIndex;

   for(iFlowIndex=0; iFlowIndex < MAX_INTE_FLOW_SUPPORTED; iFlowIndex++)
   {
      if(TRUE == aIntFlowSensor[iFlowIndex].fInit)
      {
         GetFlowInformation(&aIntFlowSensor[iFlowIndex].FlowSensInfo, iFlowIndex);
      }
   }
}

/**==== DrvGetIOHardwareType =============================================

   For the given Expander Number return the IO Board Type for that
   expander.


   Returns: I/O Board Type


=========================================================================*/
extern BYTE DrvGetIOHardwareType(BYTE bExpanderNumber)
{
   return ((pDRVSTAT && (bExpanderNumber >=1) && (bExpanderNumber <= MAX_EXPANDERS_SUPPORTED)) ?
           pDRVSTAT->aExpanderInfo[bExpanderNumber-1].bBoardType : 0);
}

/**==== DrvGetIOModuleStatus =============================================

   For the given Expander Number return the module status for that
   expander.


   Returns: I/O module status


=========================================================================*/
extern BYTE DrvGetIOModuleStatus(BYTE bExpanderNumber)
{
   return ((pDRVSTAT && (bExpanderNumber >=1) && (bExpanderNumber <= MAX_EXPANDERS_SUPPORTED)) ?
           pDRVSTAT->aExpanderInfo[bExpanderNumber-1].bPresentState : EXP_NOT_YET_FOUND);
}

/**==== DrvGetRSSensorCommStatus =============================================

   Retrieve the RS Sensor Fail status and return to calling function


   Returns: RS Sensor Fail status


=========================================================================*/
extern BYTE DrvGetRSSensorCommStatus(void)
{
   return (bSensorFail);
}

/**=== IsInStandAloneMode ================================================

   Function is called from DrvGetBI() of microblock processing to report
   the communication state (Active or Standalone) on specified port.

   Name        Dir   Description
      none

   Returns:
      True - If timer has expired.

   Cautions:

=========================================================================*/
extern BOOL IsInStandAloneMode(void)
{
   return(UtTimerExpired(&CommStandaloneCtrl.sCommStandaloneTimer));
}

/**=== IsPushEvent ================================================

   Function to read the Push button Event from sysfs.

   Name        Dir   Description
      none

   Returns:
      True - If iRotarySwitchProtocol is 1. i.e push event

   Cautions:

=========================================================================*/
extern BOOL IsPushEvent(void)
{
   BOOL fReadVal = FALSE;

   /* Push button is not supported on boards woth rotary switch support */
   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT) == FALSE)
   {
      fReadVal = (BOOL)ReadPushSwitch();
   }

   return (fReadVal);
}

/**=== DrvReinitCommStandaloneConfig =====================================

   Function should be called from DrvReinit() when there is a communication
   standalone parameter change

   Name        Dir   Description
      none

   Returns:
      none

   Cautions:

=========================================================================*/
static void DrvReinitCommStandaloneConfig(void)
{
   if (pDRVPARS)
   {
      if ((CommStandaloneCtrl.wCurrentStandaloneTimeout != pDRVPARS->wStandAloneTimeout) ||
          (CommStandaloneCtrl.bCurrentStandalonePort != pDRVPARS->bStandAlonePort))
      {
         CommStandaloneCtrl.bCurrentStandalonePort = pDRVPARS->bStandAlonePort;
         CommStandaloneCtrl.wCurrentStandaloneTimeout = pDRVPARS->wStandAloneTimeout;

         // Parameter just changed, so provide it a 60 second to get go
         // again before the timer expires
         UtTimerSet(&CommStandaloneCtrl.sCommStandaloneTimer, ONE_MINUTE_IN_SECONDS);
      }
   }
}

/**=== DrvResetCommStandaloneTimer =======================================

   Function is called from CONF_SERV_indication(), it resets the stand
   alone timer when "write-like" service requests received

   Name        Dir   Description
   pBpPacket    In   The request packet pointer

   Returns:
      none

   Cautions:

=========================================================================*/
extern void DrvResetCommStandaloneTimer(BPPACKET *pBpPacket)
{
   if (pBpPacket)
   {
      BOOL  fCommActive = FALSE;
      BYTE  bDLLType;
      BYTE  bInstanceOfDllType;

      if (pBpPacket->idOriginatingDll)
      {
         bDLLType = BpNetGetDllType(pBpPacket->idOriginatingDll);
         bInstanceOfDllType = BpNetGetInstanceOfDllType(pBpPacket->idOriginatingDll);

         if ((pBpPacket->bServiceChoice == BACnetCSC__writeProperty) ||
             (pBpPacket->bServiceChoice == BACnetCSC__writePropertyMultiple) ||
             (pBpPacket->bServiceChoice == BACnetCSC__atomicWriteFile) ||
             (pBpPacket->bServiceChoice == BACnetCSC__confirmedCOVNotification))
         {
            switch (CommStandaloneCtrl.bCurrentStandalonePort)
            {
               case NG_IP_NETWORK_IDX:

                  if ((bDLLType == DLL_IP) && (bInstanceOfDllType == 0))
                  {
                     fCommActive = TRUE;
                  }
                  break;

               case NG_ETHERNET_NETWORK_IDX:

                  if ((bDLLType == DLL_ETHERNET) && (bInstanceOfDllType == 0))
                  {
                     fCommActive = TRUE;
                  }
                  break;

               case NG_PORTS1_NETWORK_IDX:

                  if (((bDLLType == DLL_ARCNET) || (bDLLType == DLL_MSTP) || (bDLLType == DLL_MSTP_SLAVE)) &&
                      (bInstanceOfDllType == PORTS1))
                  {
                     fCommActive = TRUE;
                  }
                  break;

               case NG_PORTS2_NETWORK_IDX:

                  if (((bDLLType == DLL_MSTP) || (bDLLType == DLL_MSTP_SLAVE)) && (bInstanceOfDllType == PORTS2))
                  {
                     fCommActive = TRUE;
                  }
                  break;

               case NG_SEC_IP_NETWORK_1_IDX:

                  if ((bDLLType == DLL_IP) && (bInstanceOfDllType == 1))
                  {
                     fCommActive = TRUE;
                  }
                  break;

               default:
                  break;
            }

            if (fCommActive)
            {
               // We've received a "write-like" request packet on the comm port, consider
               // the module no longer in the standalone mode, then reset the timer.
               UtTimerSet(&CommStandaloneCtrl.sCommStandaloneTimer, CommStandaloneCtrl.wCurrentStandaloneTimeout);
            }
         }
      }
   }
}

//==== DrvServiceRsStat ==================================================
// Service the RS microblock.
//
// This function is called inline with the execution of the
// RS microblock and has access to all of the microblock's
// data areas.  This is the hardware DEPENDENT portion of
// the RS microblock.  This function gets called
// at CPM RESET (CPM_PUPINIT), REINIT (CPM_REINIT), and normal
// execution (CPM_EXEC).
//
// Note that the RS sensor is communicated to by another task,
// asynch with the module driver.  It is entirely possible for
// this function to get called before the sensor is "discovered".
// The microblock should rely on "Green Time" until comm is
// established.
//
// This function's primary responsibility is to write all of
// the "WRITE" variables shown in the comments below.
// These variables should be set using different methods depending on
// the sensor type detected.  The microblock code that calls this
// function will handle the TLO timer (if it is not disabled here)
// and locking the wire outputs of any Lock parameters are set,
// and that is about all.  Most of the hard stuff needs to be done by
// this function, which may be unique for each module driver.
//
// This function READS some or all of these variables:
//    BYTE   pcp->bMode = execution mode; CPM_PUPINIT, CPM_REINIT, or CPM_EXEC
//    BYTE   LOBYTE((pfb)->wMyAddr) = CP number
//    USHORT pPars->bTLOCancel      = seconds to hold cancel button to cancel override
//    USHORT pPars->uTLOIncrement   = minutes to increment override on each key press
//    USHORT pPars->uTLOLimit       = maximum override period in minutes
//    FLOAT  pPars->rSetPtAdjLimit  = maximum positive or negative setpoint adjust allowed in degrees
//    FLOAT  pPars->aiZoneTemp.rCalibrationOffset     = signed offset in degrees to be added to zone temp measurement
//    CHAR   pStat->chTempUnits     = 'C' if Celsius, 'F' if Fahrenheit (RS Pro)
//    BYTE   pStat->fOccupied       = TRUE if LED should be illuminated
//    FLOAT  pStat->rOAT            = Current outside air temp in degrees (RS Pro)
//    BYTE   pStat->fAlarm          = TRUE if alarm bell should be displayed on RS Pro
//
// Before returning, this function should WRITE all of these variables:
//    BYTE   pStat->bType  = LogiStat type
//                               0 = none (default)
//                               1 = Pro
//                               2 = RS Pro
//                               3 = RS Basic
//                               4 = RS Plus
//                             255 = Basic
//    FLOAT  pVars->rOutAdjustedZoneTemp = actual zone temp in degrees after offset is applied
//    FLOAT  pVars->rOutSetPtAdj         = signed setpoint adjustment in degrees
//    SHORT  pVars->sOutTLOTimeRemaining = set new override time remaining
//    BYTE   pVars->fOutUpdateZoneTemp   = set FALSE if rOutAdjustedZoneTemp should be ignored
//    BYTE   pVars->fOutUpdateSetPtAdj   = set FALSE if rOutSetPtAdj should be ignored
//    BYTE   pVars->fOutUpdateTLOTimeRemaining = set FALSE if sOutTLOTimeRemaining should be ignored
//    BYTE   pVars->fOutDisableLocalTLOTimer = set TRUE to prevent the microblock from controlling
//                                             the TLO timer, which means you must set sOutTLOTimeRemaining
//                                             and fOutUpdateTLOTimeRemaining on every call.
//
//   Name                     Dir     Description
//
//   pcp                      in      Pointer to the CPRUN structure.
//   pPars                    in      Pointer to the RS parameter space.
//   pStat                    in      Pointer to the RS status space.
//   pVars                    in      Pointer to the RS variable space.
//   pBAIParsZoneTemp         in      Pointer to the Zone Temperature.
//   pAVStatOvrRem            in      Pointer to the RS Override Time Remaining.
//
//   Returns:
//   none
//
//   Cautions:
//   None
//
//=========================================================================*/
extern void DrvServiceRsStat(CPRUN *pcp, RS_PARS *pPars, RS_STAT *pStat, RS_VARS *pVars, BAIx1_PARS *pBAIParsZoneTemp, ANALOG_VALUE_WRITEABLE_STAT *pAVStatOvrRem)
{
   BYTE bMode = pcp->bMode; // CPM_PUPINIT, CPM_REINIT, or CPM_EXEC
   BYTE bBaseAddress; // used for scanning the array of RS Base sensors
   float rTotal;
   float rMinValue;
   float rMaxValue;
   int iIndex; // number of sensors taking part in averaging
   int iAverage;

   // prevent multiple RS/LogiStat microblocks in multiple GFBs.
   if (wRsStatGfbInstance || wRsfStatGfbInstance)
   { // if there is an instance, make sure it's this one
      if ( (wRsStatGfbInstance && (wRsStatGfbInstance != pcp->wInstance)) ||
           (wRsfStatGfbInstance && (wRsfStatGfbInstance != pcp->wInstance)) )
      {  // kill this GFB :->
         CpHaltProgram(pcp, BACnetProgramError__program, "More than 1 RSmb detected");
      }
   }
   else
   { // else this is the first GFB instance detected
      wRsStatGfbInstance = pcp->wInstance;
   }

   if (bMode == CPM_PUPINIT)
   {
      /*---------------------------------------------------------------------*/
      /* Add sensor addresses.                                               */
      /*---------------------------------------------------------------------*/
      RnetInsertAddress(0x10); // add RS Base 0 to possible addresses
      RnetInsertAddress(0x11); // add RS Base 1 possible addresses
      RnetInsertAddress(0x12); // add RS Base 2 possible addresses
      RnetInsertAddress(0x13); // add RS Base 3 possible addresses
      RnetInsertAddress(0x70); // add RS Plus to possible addresses
      RnetInsertAddress(0x20); // add RS Pro to possible addresses
   }

   // Assume no updates to RS microblock.
   pVars->fOutUpdateZoneTemp = FALSE;
   pVars->fOutUpdateSetPtAdj = FALSE;
   pVars->fOutUpdateTLOTimeRemaining = FALSE;
   pVars->fOutDisableLocalTLOTimer = TRUE;

   bSensorFail = pVars->fLogistatFail; // update flag to indicate if sensor is valid

   iIndex = 0; // skip over the RS Pro array member
   bBaseAddress = 0x20; // RS Pro Address

   while (iIndex < MAX_RS)
   {
      if (!apRnetDevice[iIndex])
      { // find loginet device if not found yet
         apRnetDevice[iIndex] = (RNET_LS2_INFO *) RnetFindDevice(bBaseAddress); // locate pointer at LogiNet device 0

         if (apRnetDevice[iIndex])
         {
            if (!(apRnetDevice[iIndex]->fDeviceInit))
            {
               apRnetDevice[iIndex] = NULL; // wait until device is configured
            }
         }

         if (apRnetDevice[iIndex])
         { // first time through, configure device according to pars of Mb
            memcpy(pStat->aRsStat[iIndex].szRsVersion,
                  ((RNET_DEVICE_HEADER *) apRnetDevice[iIndex])->szFirmwareVersion,
                  sizeof(((RNET_DEVICE_HEADER *) apRnetDevice[iIndex])->szFirmwareVersion));

            // retrieve stored setpoint adjust value for LogiStat Pro
            if (bBaseAddress == 0x20) // Pro always becomes master
            {
               pRnetMaster = apRnetDevice[iIndex]; // Pro should always be master
               pStat->bType = LS2_PRO; // RS type
               pRnetMaster->rSpAdjust = pStat->rSetPtAdj;
            }
            else if (bBaseAddress == 0x70) // Plus becomes master over Basics
            {
               pRnetMaster = apRnetDevice[iIndex];
               pStat->bType = LS2_PLUS;
               pRnetMaster->rSpAdjust = pStat->rSetPtAdj;
            }

            if ((!pRnetMaster) && (bBaseAddress == 0x10)) // Base 0 should be master, if Pro is not
            {
               pRnetMaster = apRnetDevice[iIndex]; // Pro should always be master
               pStat->bType = LS2_BASE; // RS type
               pRnetMaster->rSpAdjust = pStat->rSetPtAdj;
            }
         } // init the device when detected
      }

      if (bBaseAddress == 0x20)
      {
         bBaseAddress = 0x70;
      }
      else
      {
         iIndex++;

         if (bBaseAddress >= 0x70)
         {
            bBaseAddress = 0x10;
         }
         else
         {
            bBaseAddress++;
         }
      }
   }

   rTotal = 0;
   iAverage = 0; // number of sensors taking part in averaging
   rMinValue = 296.0F;
   rMaxValue = 0.0F;

   // Service sensors beginning with the RS-Pro.
   //   RS-Pro/RS-Plus always use index 0.
   //   RS-Basic uses indexes 1-4.
   for (iIndex = 0; iIndex < MAX_RS; iIndex++)
   {
      if (apRnetDevice[iIndex])
      { // make sure the device is still alive, otherwise let it reinit when found again

         apRnetDevice[iIndex]->rZoneTempOffset = pPars->arAuxSensorCalib[iIndex]; // final offset applied to temp

         if (apRnetDevice[iIndex] == pRnetMaster)
         { // master sensor has control of setpoint and override
           // refresh LogiStat parameters in case they have changed
            pRnetMaster->iOverrideTimeIncrement = (LONG) (pPars->uTLOIncrement / 60); // # of minutes to bump override time on every keypress
            pRnetMaster->iOverrideTimeLimit = (LONG) (pPars->uTLOLimit / 60); // max override time in minutes
            pRnetMaster->rSpAdjustRange = pPars->rSetPtAdjLimit; // max +- limit on setpoint adjust
            pRnetMaster->fContinEnab = (BOOL) (pPars->bContinEnab); // allow continuous occupied
            pRnetMaster->fAutoZeroSpAdj = ((!pStat->fOccupied) & pPars->fSetPtAutoZero);

            // Check self-clearing Setpoint Adjust parameter
            if (!IsNaN(pPars->rMomentarySetPtAdj))
            {
               pRnetMaster->rSpAdjust = pPars->rMomentarySetPtAdj;
               pRnetMaster->rEditSpAdjust = pPars->rMomentarySetPtAdj;
               // Clear one-shot parameters for setpoint adjust
               pPars->rMomentarySetPtAdj = FNAN;
            }

            // Check self-clearing TLO parameter
            if (pPars->sMomentaryTLO != S2NAN)
            {
               if (((LONG) (pPars->sMomentaryTLO) > (apRnetDevice[iIndex]->iOverrideTimeLimit * 60)) &&
                     apRnetDevice[iIndex]->fContinEnab)
               {
                  pRnetMaster->bMomContinState = MOM_TLO_CONTIN_SET;
               }
               else
               {
                  pRnetMaster->bMomContinState = MOM_TLO_CONTIN_CLR;
               }

               UtLongTimerSet((ULONG *) (&apRnetDevice[iIndex]->iOverrideTimer), (ULONG) (pPars->sMomentaryTLO));
               // Clear one-shot parameters for Timed Local Override
               pPars->sMomentaryTLO = S2NAN;
            }

            // Check for external modification of TLO
            if (pVars->rLastOvrTimeRemain != pAVStatOvrRem->rPresentValue)
            {
               UtLongTimerSet((ULONG *) (&apRnetDevice[iIndex]->iOverrideTimer), (ULONG) (pAVStatOvrRem->rPresentValue * 60));
            }

            // Make sure we are not already exceeding new limits for TLO or setpt adjust
            if (UtLongTimerRemain((ULONG *) (&apRnetDevice[iIndex]->iOverrideTimer)) > (ULONG) (apRnetDevice[iIndex]->iOverrideTimeLimit * 60))
            {
               UtLongTimerSet((ULONG *) (&apRnetDevice[iIndex]->iOverrideTimer), (ULONG) (apRnetDevice[iIndex]->iOverrideTimeLimit * 60));
            }

            // Update microblock working area from RS working area
            pVars->sOutTLOTimeRemaining = (SHORT) ((UtLongTimerRemain((ULONG *) (&apRnetDevice[iIndex]->iOverrideTimer))));
            pVars->fOutUpdateTLOTimeRemaining = TRUE;

            if (!IsNaN(pRnetMaster->rSpAdjust))
            {
               // Make sure we are not already exceeding new limits for setpoint adjust
               if (ABS(pRnetMaster->rSpAdjust) > pRnetMaster->rSpAdjustRange)
               {
                  pRnetMaster->rSpAdjust = (pRnetMaster->rSpAdjust < 0.0F ? -1.0F : 1.0F) * pRnetMaster->rSpAdjustRange;
               }

               // autozero setpoint adjust if parameter set and unoccupied
               if (pPars->fSetPtAutoZero && !pStat->fOccupied)
               {
                  pRnetMaster->rSpAdjust = 0;
                  pRnetMaster->rSpAdjustRange = 0; // zero this to keep UI clean
               }
               else
               {
                  pRnetMaster->rSpAdjustRange = pPars->rSetPtAdjLimit; // max +- limit on setpoint adjust
               }

               pVars->rOutSetPtAdj = pRnetMaster->rSpAdjust;
               pVars->fOutUpdateSetPtAdj = TRUE;
            }

            pStat->fContinMode = pRnetMaster->fContinuous; // expose data to UI
         } // master processing

         if (((RNET_DEVICE_HEADER *) apRnetDevice[iIndex])->RnetDeviceState == RNET_DEV_ONLINE)
         {
            pStat->bRsCommFail &= ~((BYTE) (1 << iIndex)); // update comm status flags

            if (!IsNaN(apRnetDevice[iIndex]->rZoneTemp))
            {
               pStat->aRsStat[iIndex].arAuxSensor = apRnetDevice[iIndex]->rZoneTemp; // real value available

               if (pPars->abAuxSensorEnab[iIndex])
               {
                  rTotal += apRnetDevice[iIndex]->rZoneTemp;
                  iAverage++; // overload to determine default or calculation operation

                  if (rMinValue > apRnetDevice[iIndex]->rZoneTemp)
                  {
                     rMinValue = apRnetDevice[iIndex]->rZoneTemp;
                  }
                  if (rMaxValue < apRnetDevice[iIndex]->rZoneTemp)
                  {
                     rMaxValue = apRnetDevice[iIndex]->rZoneTemp;
                  }
               }
            }
         } else
         {
            pStat->aRsStat[iIndex].arAuxSensor = 0; // zero if AWOL
            pStat->bRsCommFail |= (BYTE) (1 << iIndex); // update comm status flags
         }

         // Update RS variables from RS microblock data structure
         apRnetDevice[iIndex]->fCelsius = (BOOL) (pStat->chTempUnits == 'C');
         apRnetDevice[iIndex]->fOccupied = pStat->fOccupied;
         apRnetDevice[iIndex]->rOAT = pStat->rOAT;
         apRnetDevice[iIndex]->rHeatSp = pStat->rHeatSetp;
         apRnetDevice[iIndex]->rCoolSp = pStat->rCoolSetp;

         if (pPars->bOptions & OAT_INPUT_PRESENT)
         {
            apRnetDevice[iIndex]->bOptions |= OAT_INPUT_PRESENT;
         }
         else
         {
            apRnetDevice[iIndex]->bOptions &= ~OAT_INPUT_PRESENT;
         }

         apRnetDevice[iIndex]->fAlarm = pStat->fAlarm;
      }
   }

   if (iAverage) // values set if any good ones are returned
   {
      switch (pPars->bAvgMaxMin)
      {
         case AUX_SENSOR_AVERAGE: // valid for IN's 1 and 2 only
            pVars->rOutAdjustedZoneTemp = (rTotal / (FLOAT) iAverage) + pBAIParsZoneTemp->rCalibrationOffset;
            break;

         case AUX_SENSOR_MAXIMUM: // valid for all AI's
            pVars->rOutAdjustedZoneTemp = rMaxValue + pBAIParsZoneTemp->rCalibrationOffset;
            break;

         default: // selected AI input type not supported
            pVars->rOutAdjustedZoneTemp = rMinValue + pBAIParsZoneTemp->rCalibrationOffset;
            break;
      }

      pVars->fOutUpdateZoneTemp = TRUE;
   }
   else if (pRnetMaster) // use master value if good
   {
      if (((RNET_DEVICE_HEADER *) pRnetMaster)->RnetDeviceState == RNET_DEV_ONLINE)
      {
         if (!IsNaN(pRnetMaster->rZoneTemp))
         {
            pVars->rOutAdjustedZoneTemp = pRnetMaster->rZoneTemp + pBAIParsZoneTemp->rCalibrationOffset;
            pVars->fOutUpdateZoneTemp = TRUE;
         }
      }
   }
}

//========================================================================
//
// The following functions are created for the sole purpose of avoiding
// duplicate code when processing RSF microblocks and devices.
//
//=========================================================================

/*==== DrvInitRsfCommon ===================================================
//
//   Helper function that provides some common processing that is called by
//   other RS related local functions
//
//   Name                     Dir     Description
//
//   pLnDevice                in      Pointer RS Master device
//   pPars                    in      Pointer to the RS parameter space.
//   pStat                    in      Pointer to the RS status space.
//
//   Returns:
//   none
//
//   Cautions:
//   None
//
=========================================================================*/
static void DrvInitRsfCommon(RNET_LS2_INFO *pLnDevice, RSF_PARS *pPars, RSF_STAT *pStat)
{
   pLnDevice->rSpAdjust = pStat->rSetPtAdj;
   pLnDevice->bFanSpeed = pStat->bFanSpeed;
   pLnDevice->bMode = pStat->bMode;
   pLnDevice->fContinEnab = (BOOL)(pPars->wOptionPars & MANUAL_ON_CONTINOUS);
   pLnDevice->fContinuous = pStat->fContinMode;

   if (pLnDevice->fContinEnab && pLnDevice->fContinuous)
   {
      pLnDevice->iEditOverrideTimer = (LONG)(pPars->ulTLOLimit);
   }
}

/*==== DrvInitRsfDevice ===================================================
//
//   Helper function called by other local RS functions to initialize RSF
//   device.
//
//   Name                     Dir     Description
//
//   pPars                    in      Pointer to the RS parameter space.
//   pStat                    in      Pointer to the RS status space.
//   iIndex                   in      Device index
//   iSensorIndex             in      Sensor index
//   bBaseAddress             in
//
//   Returns:
//   none
//
//   Cautions:
//   None
//
=========================================================================*/
static void DrvInitRsfDevice(RSF_PARS *pPars, RSF_STAT *pStat, int iIndex, int iSensorIndex, BYTE bBaseAddress)
{
   apRnetDevice[iIndex] = (RNET_LS2_INFO *) RnetFindDevice(bBaseAddress); // locate pointer at Rnet device

   if( (apRnetDevice[iIndex]) && (!apRnetDevice[iIndex]->fDeviceInit))
   {
      apRnetDevice[iIndex] = NULL; // wait until device is configured
   }

   if(apRnetDevice[iIndex])
   {  // first time through, configure device according to pars of Mb
      memcpy(pStat->aRsStat[iSensorIndex].szRsVersion, ((RNET_DEVICE_HEADER *)apRnetDevice[iIndex])->szFirmwareVersion, sizeof(((RNET_DEVICE_HEADER *)apRnetDevice[iIndex])->szFirmwareVersion));

      if(bBaseAddress == 0x20) // Pro becomes master over all
      {
         char *pChar = NULL;

         pRnetMaster = apRnetDevice[iIndex];

         // RS Pro or RS Pro F?
         pChar = strstr(pStat->aRsStat[iSensorIndex].szRsVersion, "RSPRO-F");

         if(pChar)
         {
            pStat->bType = LS2_PRO_F;
         }
         else
         {
            pStat->bType = LS2_PRO;
         }

         DrvInitRsfCommon(pRnetMaster, pPars, pStat);
      }
      else if(bBaseAddress == 0x70) // Plus becomes master over Basics
      {
         pRnetMaster = apRnetDevice[iIndex];
         pStat->bType = LS2_PLUS;
         DrvInitRsfCommon(pRnetMaster, pPars, pStat);
      }
      else if((!pRnetMaster) && (bBaseAddress == 0x10)) // Base 0 master, if Plus or Pro is not
      {
         pRnetMaster = apRnetDevice[iIndex];
         pStat->bType = LS2_BASE;
         DrvInitRsfCommon(pRnetMaster, pPars, pStat);
      }
   }
}

//==== DrvUpdtMasterRsf ===================================================
//
//   Helper function called by other local RS functions to update the
//   master RSF Device
//
//   Name                     Dir     Description
//
//   pPars                    in      Pointer to the RS parameter space.
//   pStat                    in      Pointer to the RS status space.
//   pVars                    in      Pointer to the RS variable space.
//   pAVStatOvrRem            in      Pointer to the RS Override Time Remaining.
//
//   Returns:
//   none
//
//   Cautions:
//   None
//
//=========================================================================
static void DrvUpdtMasterRsf(RSF_PARS *pPars, RSF_STAT *pStat, RSF_VARS *pVars, int iIndex, ANALOG_VALUE_WRITEABLE_STAT *pAVStatOvrRem)
{
   // refresh RSF parameters in case they have changed
   apRnetDevice[iIndex]->iOverrideTimeIncrement  = (LONG)(pPars->ulTLOIncrement/60);  // # of minutes to bump override time on every keypress
   apRnetDevice[iIndex]->iOverrideTimeLimit      = (LONG)(pPars->ulTLOLimit/60);      // max override time in minutes

   if(pPars->bTLOCancel)
   {
      apRnetDevice[iIndex]->bOverrideCancelHold  = pPars->bTLOCancel; // seconds to hold override to cancel
   }
   else
   {
      apRnetDevice[iIndex]->bOverrideCancelHold  = 0x01; // not less than one
   }

   apRnetDevice[iIndex]->rSpAdjustRange = pPars->rSetPtAdjLimit; // max +- limit on setpoint adjust
   apRnetDevice[iIndex]->bOptions = pPars->bOptions; // optional inputs
   apRnetDevice[iIndex]->bAuxType[0] = pPars->bAuxAn1Units; // aux input 1 display units
   apRnetDevice[iIndex]->bAuxType[1] = pPars->bAuxAn2Units; // aux input 2 display units
   apRnetDevice[iIndex]->wOptionPars = pPars->wOptionPars; // option bits for selection
   apRnetDevice[iIndex]->bModeOptions = pPars->bModeOptions; // option bits for selection
   apRnetDevice[iIndex]->fContinEnab = (BOOL)(pPars->wOptionPars & MANUAL_ON_CONTINOUS);  // allow continuous occupied
   apRnetDevice[iIndex]->fAutoZeroSpAdj = ((!pStat->fOccupied) && (pPars->wOptionPars & AUTO_ZERO_SPADJ)) ? TRUE : FALSE;
   memcpy(apRnetDevice[iIndex]->aRSFModes, pPars->aRSFModes, sizeof(RSF_MODE_DISP) * RSF_MAX_MODE); // copy RSF mode display

   // Check self-clearing Setpoint Adjust parameter
   if (!IsNaN(pPars->rMomentarySetPtAdj))
   {
      apRnetDevice[iIndex]->rSpAdjust = pPars->rMomentarySetPtAdj;
      apRnetDevice[iIndex]->rEditSpAdjust =  pPars->rMomentarySetPtAdj;
      // Clear one-shot parameters for setpoint adjust
      pPars->rMomentarySetPtAdj = FNAN;
   }

   // Check self-clearing TLO parameter
   if (pPars->ulMomentaryTLO != UNAN)
   {
      if ((pPars->ulMomentaryTLO > ((ULONG)(apRnetDevice[iIndex]->iOverrideTimeLimit * 60))) && apRnetDevice[iIndex]->fContinEnab)
      {
         apRnetDevice[iIndex]->bMomContinState = MOM_TLO_CONTIN_SET;
      }
      else
      {
         apRnetDevice[iIndex]->bMomContinState = MOM_TLO_CONTIN_CLR;
      }

      UtLongTimerSet((ULONG *)(&apRnetDevice[iIndex]->iOverrideTimer), (ULONG)(pPars->ulMomentaryTLO));
      // Clear one-shot parameters for Timed Local Override
      pPars->ulMomentaryTLO = UNAN;
   }

   // Check self-clearing mode parameter
   if (pPars->bMomentaryMode != U1NAN)
   {
      if(pPars->bMomentaryMode <= pPars->bNumModes)
      {
         apRnetDevice[iIndex]->bMode =  apRnetDevice[iIndex]->bEditMode =  pPars->bMomentaryMode;
      }

      pPars->bMomentaryMode = U1NAN;
   }

   // Check self-clearing fan speed parameter
   if (pPars->bMomentaryFanSpd != U1NAN)
   {
      if(pPars->bMomentaryFanSpd <= pPars->bFanSpeeds)
      {
         apRnetDevice[iIndex]->bFanSpeed =  pPars->bMomentaryFanSpd;
      }

      pPars->bMomentaryFanSpd = U1NAN;
   }

   // Check for external modification of TLO
   if (pVars->rLastOvrTimeRemain != pAVStatOvrRem->rPresentValue)
   {
      UtLongTimerSet((ULONG *)(&apRnetDevice[iIndex]->iOverrideTimer), (ULONG)(pAVStatOvrRem->rPresentValue * 60));
   }

   // Make sure we are not already exceeding new limits for TLO or setpt adjust
   if (UtLongTimerRemain((ULONG *)(&apRnetDevice[iIndex]->iOverrideTimer)) > (ULONG)(apRnetDevice[iIndex]->iOverrideTimeLimit * 60))
   {
      UtLongTimerSet((ULONG *)(&apRnetDevice[iIndex]->iOverrideTimer), (ULONG)(apRnetDevice[iIndex]->iOverrideTimeLimit * 60));
   }

   // Update microblock working area from RS working area
   pVars->ulOutTLOTimeRemaining = (ULONG)((UtLongTimerRemain((ULONG *)(&apRnetDevice[iIndex]->iOverrideTimer))));
   pVars->fOutUpdateTLOTimeRemaining = TRUE;

   if (!IsNaN(apRnetDevice[iIndex]->rSpAdjust))
   {
      // Make sure we are not already exceeding new limits for setpoint adjust
      if (ABS(apRnetDevice[iIndex]->rSpAdjust) > apRnetDevice[iIndex]->rSpAdjustRange)
      {
         apRnetDevice[iIndex]->rSpAdjust = (apRnetDevice[iIndex]->rSpAdjust < 0.0F ? -1.0F : 1.0F) * apRnetDevice[iIndex]->rSpAdjustRange;
      }

      // autozero setpoint adjust if parameter set and unoccupied
      if((pPars->wOptionPars & AUTO_ZERO_SPADJ) && !pStat->fOccupied)
      {
         apRnetDevice[iIndex]->rSpAdjust = 0;
         apRnetDevice[iIndex]->rSpAdjustRange = 0; // zero this to keep UI clean
      }
      else
      {
         apRnetDevice[iIndex]->rSpAdjustRange = pPars->rSetPtAdjLimit; // max +- limit on setpoint adjust
      }

      pVars->rOutSetPtAdj = apRnetDevice[iIndex]->rSpAdjust;
      pVars->fOutUpdateSetPtAdj = TRUE;
   }

   // clear fan speed during unocc mode if defined that way
   if((pPars->wOptionPars & FAN_SPEED_CLEARED) && !pStat->fOccupied && !pStat->fContinMode)
   {
      apRnetDevice[iIndex]->bFanSpeeds = 0; // prevent selection of fan speeds
      pStat->bFanSpeed = apRnetDevice[iIndex]->bFanSpeed = 0;
   }
   else
   {
      apRnetDevice[iIndex]->bFanSpeeds = pPars->bFanSpeeds; // high limit of selected fan speeds
      pStat->bFanSpeed = apRnetDevice[iIndex]->bFanSpeed; // copy from device to Mb
   }

   // preset mode during unocc mode if defined that way
   if((pPars->bModeOptions & MODE_RESET) && !pStat->fOccupied && !pStat->fContinMode)
   {
      apRnetDevice[iIndex]->bNumModes = 0; // prevent selection of modes
      pStat->bMode = apRnetDevice[iIndex]->bMode = pPars->bResetMode; // set to predefined value
   }
   else
   {
      apRnetDevice[iIndex]->bNumModes = pPars->bNumModes; // high limit of selected modes
      pStat->bMode = apRnetDevice[iIndex]->bMode; // copy from device to Mb
   }

   pStat->fContinMode = apRnetDevice[iIndex]->fContinuous; // expose data to UI
}

//==== DrvUpdateRsfDevice ================================================
//
//   Helper function called by other local RS functions to update the
//   a RSF Device
//
//   Name                     Dir     Description
//
//   pPars                    in      Pointer to the RS parameter space.
//   pStat                    in      Pointer to the RS status space.
//   iIndex                   in      Pointer to the RS variable space.
//
//   Returns:
//   none
//
//   Cautions:
//   None
//
//=========================================================================*/
static void DrvUpdateRsfDevice(RSF_PARS *pPars, RSF_STAT *pStat, int iIndex)
{
   // Update RS variables from RS microblock data structure
   apRnetDevice[iIndex]->fCelsius     = (BOOL)(pStat->chTempUnits == 'C');
   apRnetDevice[iIndex]->fOccupied    = pStat->fOccupied;
   apRnetDevice[iIndex]->rOAT         = pStat->rOAT;
   apRnetDevice[iIndex]->rHeatSp      = pStat->rHeatSetp;
   apRnetDevice[iIndex]->rCoolSp      = pStat->rCoolSetp;
   apRnetDevice[iIndex]->fAlarm       = pStat->fAlarm;
   apRnetDevice[iIndex]->bFanRun      = pStat->fFan;

   // Get optional analog input 1 if used
   if(pPars->bOptions & AUX_ANALOG_IN_01)
   {
      apRnetDevice[iIndex]->rAuxDisp[0] = pStat->rAuxAnalog1; // optional wire input
   }

   // Get optional analog input 2 if used
   if(pPars->bOptions & AUX_ANALOG_IN_02)
   {
      apRnetDevice[iIndex]->rAuxDisp[1] = pStat->rAuxAnalog2; // optional wire input
   }

   // Get optional binary input 1 if used
   if(pPars->bOptions & AUX_BINARY_IN_01)
   {
      apRnetDevice[iIndex]->bAuxDisp[0] = (BYTE)(pStat->fAuxBinary1); // optional wire input
   }

   // Get optional binary input 2 if used
   if(pPars->bOptions & AUX_BINARY_IN_02)
   {
      apRnetDevice[iIndex]->bAuxDisp[1] = (BYTE)(pStat->fAuxBinary2); // optional wire input
   }
}

//*==== DrvServiceRsfStat ==================================================
// Service the RSF microblock.
//
// This function is called inline with the execution of the
// RSF microblock and has access to all of the microblock's
// data areas.  This is the hardware DEPENDENT portion of
// the RSF microblock.  This function gets called
// at CPM RESET (CPM_PUPINIT), REINIT (CPM_REINIT), and normal
// execution (CPM_EXEC).
//
// Note that the RSF sensor is communicated to by another task,
// asynch with the module driver.  It is entirely possible for
// this function to get called before the sensor is "discovered".
// The microblock should rely on "Green Time" until comm is
// established.
//
// This function's primary responsibility is to write all of
// the "WRITE" variables shown in the comments below.
// These variables should be set using different methods depending on
// the sensor type detected.  The microblock code that calls this
// function will handle the TLO timer (if it is not disabled here)
// and locking the wire outputs of any Lock parameters are set,
// and that is about all.  Most of the hard stuff needs to be done by
// this function, which may be unique for each module driver.
//
// This function READS some or all of these variables:
//    BYTE   pcp->bMode = execution mode; CPM_PUPINIT, CPM_REINIT, or CPM_EXEC
//    BYTE   LOBYTE((pfb)->wMyAddr) = CP number
//    BYTE   pPars->bTLOCancel      = seconds to hold cancel button to cancel override
//    ULONG  pPars->ulTLOIncrement  = minutes to increment override on each key press
//    ULONG  pPars->ulTLOLimit      = maximum override period in minutes
//    FLOAT  pPars->rSetPtAdjLimit  = maximum positive or negative setpoint adjust allowed in degrees
//    FLOAT  pPars->aiZoneTemp.rCalibrationOffset     = signed offset in degrees to be added to zone temp measurement
//    CHAR   pStat->chTempUnits     = 'C' if Celsius, 'F' if Fahrenheit (RS Pro-F)
//    BYTE   pStat->fOccupied       = TRUE if LED should be illuminated
//    FLOAT  pStat->rOAT            = Current outside air temp in degrees (RS Pro-F)
//    BYTE   pStat->fAlarm          = TRUE if alarm bell should be displayed on RS Pro-F
//
// Before returning, this function should WRITE all of these variables:
//    BYTE   pStat->bType  = LogiStat type
//                               0 = none (default)
//                               1 = Pro
//                               2 = RS Pro
//                               3 = RS Basic
//                               4 = RS Plus
//                               5 = RS Pro F
//                             255 = Basic
//    FLOAT  pVars->rOutAdjustedZoneTemp = actual zone temp in degrees after offset is applied
//    FLOAT  pVars->rOutSetPtAdj         = signed setpoint adjustment in degrees
//    ULONG  pVars->ulOutTLOTimeRemaining = set new override time remaining
//    BYTE   pVars->fOutUpdateZoneTemp   = set FALSE if rOutAdjustedZoneTemp should be ignored
//    BYTE   pVars->fOutUpdateSetPtAdj   = set FALSE if rOutSetPtAdj should be ignored
//    BYTE   pVars->fOutUpdateTLOTimeRemaining = set FALSE if sOutTLOTimeRemaining should be ignored
//    BYTE   pVars->fOutDisableLocalTLOTimer = set TRUE to prevent the microblock from controlling
//                                             the TLO timer, which means you must set sOutTLOTimeRemaining
//                                             and fOutUpdateTLOTimeRemaining on every call.
//
//
//   Name                     Dir     Description
//
//   pcp                      in      Pointer to the CPRUN structure.
//   pPars                    in      Pointer to the RS parameter space.
//   pStat                    in      Pointer to the RS status space.
//   pVars                    in      Pointer to the RS variable space.
//   pBAIParsZoneTemp         in      Pointer to the Zone Temperature.
//   pAVStatOvrRem            in      Pointer to the RS Override Time Remaining.
//
//   Returns:
//   none
//
//   Cautions:
//   None
//
//
//=========================================================================*/
extern void DrvServiceRsfStat(CPRUN *pcp, RSF_PARS *pPars, RSF_STAT *pStat, RSF_VARS *pVars, BAIx1_PARS *pBAIParsZoneTemp, ANALOG_VALUE_WRITEABLE_STAT *pAVStatOvrRem)
{
   BYTE bMode = pcp->bMode;      // CPM_PUPINIT, CPM_REINIT, or CPM_EXEC

   BYTE  bBaseAddress;           // used for scanning the array of RS Base sensors
   float rTotal;
   float rMinValue;
   float rMaxValue;
   int iIndex;                   // number of sensors taking part in averaging
   int iAverage;                 // number of sensors taking part in averaging

   // prevent multiple RS/LogiStat microblocks in multiple GFBs.
   if (wRsfStatGfbInstance || wRsStatGfbInstance)
   { // if there is an instance, make sure it's this one
      if ( (wRsfStatGfbInstance && (wRsfStatGfbInstance != pcp->wInstance)) ||
           (wRsStatGfbInstance && (wRsStatGfbInstance != pcp->wInstance)) )
      { // kill this GFB :->
         CpHaltProgram(pcp, BACnetProgramError__program, "More than 1 RSmb detected");
      }
   }
   else
   { // else this is the first GFB instance detected
      wRsfStatGfbInstance = pcp->wInstance;
   }

   // Assume no updates to LogiStat microblock.
   pVars->fOutUpdateZoneTemp           = FALSE;
   pVars->fOutUpdateSetPtAdj           = FALSE;
   pVars->fOutUpdateTLOTimeRemaining   = FALSE;
   pVars->fOutDisableLocalTLOTimer     = TRUE;

   bSensorFail = pVars->fLogistatFail;  // update flag to indicate if sensor is valid

   if(bMode == CPM_PUPINIT)
   {
      /*---------------------------------------------------------------------*/
      /* Add sensor addresses.                                               */
      /*---------------------------------------------------------------------*/
      RnetInsertAddress(0x10); // add RS Base 0 to possible addresses
      RnetInsertAddress(0x11); // add RS Base 1 possible addresses
      RnetInsertAddress(0x12); // add RS Base 2 possible addresses
      RnetInsertAddress(0x13); // add RS Base 3 possible addresses
      RnetInsertAddress(0x70); // add RS Plus to possible addresses
      RnetInsertAddress(0x20); // add RS Pro(F) to possible addresses
   }

   iIndex = 0; // start with the RS Pro array member
   bBaseAddress = 0x20; // RS Pro Address

   while(iIndex < MAX_RS)
   {
      if(!apRnetDevice[iIndex])
      {
         DrvInitRsfDevice(pPars, pStat, iIndex, iIndex, bBaseAddress);
      }

      if(bBaseAddress == 0x20)
      {
         bBaseAddress = 0x70;
      }
      else
      {
         iIndex++;

         if(bBaseAddress >= 0x70)
         {
            bBaseAddress = 0x10;
         }
         else
         {
            bBaseAddress++;
         }
      }
   }

   rTotal    = 0;
   iAverage  = 0; // number of sensors taking part in averaging
   rMinValue = 296.0F;
   rMaxValue = 0.0F;

   // Service sensors beginning with the RS-Pro.
   //   RS-Pro/RS-Plus always use index 0.
   //   RS-Basic uses indexes 1-4.
   for (iIndex = 0; iIndex < MAX_RS; iIndex++)
   {
      if(apRnetDevice[iIndex])
      {  // make sure the device is still alive, otherwise let it reinit when found again

         apRnetDevice[iIndex]->rZoneTempOffset= pPars->arAuxSensorCalib[iIndex];   // final offset applied to temp

         if(apRnetDevice[iIndex] == pRnetMaster) // master sensor has control of setpoint and override
         {
            DrvUpdtMasterRsf(pPars, pStat, pVars, iIndex, pAVStatOvrRem);
         }

         if(((RNET_DEVICE_HEADER *)apRnetDevice[iIndex])->RnetDeviceState == RNET_DEV_ONLINE)
         {
            pStat->bRsCommFail &= ~((BYTE)(1 << iIndex)); // update comm status flags

            if(!IsNaN(apRnetDevice[iIndex]->rZoneTemp))
            {
               pStat->aRsStat[iIndex].arAuxSensor = apRnetDevice[iIndex]->rZoneTemp; // real value available

               if(pPars->abAuxSensorEnab[iIndex])
               {
                  rTotal += apRnetDevice[iIndex]->rZoneTemp;
                  iAverage++; // overload to determine default or calculation operation

                  if(rMinValue > apRnetDevice[iIndex]->rZoneTemp)
                  {
                     rMinValue = apRnetDevice[iIndex]->rZoneTemp;
                  }

                  if(rMaxValue < apRnetDevice[iIndex]->rZoneTemp)
                  {
                     rMaxValue = apRnetDevice[iIndex]->rZoneTemp;
                  }
               }
            }
         }
         else
         {
            pStat->aRsStat[iIndex].arAuxSensor = 0; // zero if AWOL
            pStat->bRsCommFail |= (BYTE)(1 << iIndex); // update comm status flags
         }
         DrvUpdateRsfDevice(pPars, pStat, iIndex);
      }
   }

   if(iAverage) // values set if any good ones are returned
   {
      switch (pPars->bAvgMaxMin)
      {
         case AUX_SENSOR_AVERAGE: // valid for IN's 1 and 2 only
            pVars->rOutAdjustedZoneTemp = (rTotal / (FLOAT)iAverage) + pBAIParsZoneTemp->rCalibrationOffset;
            break;

         case AUX_SENSOR_MAXIMUM: // valid for all AI's
            pVars->rOutAdjustedZoneTemp = rMaxValue + pBAIParsZoneTemp->rCalibrationOffset;
            break;

         default: // selected AI input type not supported
            pVars->rOutAdjustedZoneTemp = rMinValue + pBAIParsZoneTemp->rCalibrationOffset;
            break;
      }
      pVars->fOutUpdateZoneTemp = TRUE;
   }
   else if(pRnetMaster) // use master value if good
   {
      if(((RNET_DEVICE_HEADER *)pRnetMaster)->RnetDeviceState == RNET_DEV_ONLINE)
      {
         if(!IsNaN(pRnetMaster->rZoneTemp))
         {
            pVars->rOutAdjustedZoneTemp = pRnetMaster->rZoneTemp + pBAIParsZoneTemp->rCalibrationOffset;
            pVars->fOutUpdateZoneTemp = TRUE;
         }
      }
   }
}
#ifdef _TARG_IX86
/**=== InitModuleDriver_UT =======================================

   Initialize module drivers

   Name              Dir   Description
   ulStartupFlags    In    startup flags

   Returns:

   Cautions: This function is only defined/used in the x86 environment.
   for unit testing

=========================================================================*/
extern void InitModuleDriver_UT(ULONG ulStartupFlags)
{
   if (!pcpModuleDriver)
   {
      pcpModuleDriver = SysReset(ulStartupFlags);
   }

   if (pcpModuleDriver)
   {
      // Assign global pointers, for convenience
      pDRVPARS = (DRVPARS *)pcpModuleDriver->pPars;
      pDRVSTAT = (DRVSTAT *)pcpModuleDriver->pStat;
      pDRVVARS = (DRVVARS *)pcpModuleDriver->pVars;
      pDRVHIST = (DRVHIST *)pcpModuleDriver->pHist;

      // don't attempt to do the following if running in the gtest environment
#ifndef _UNIT_TEST
      // Force a reinit pass now.
      SysReinit(pcpModuleDriver);

      // Reset all local IO - all DOs off, all AOs = 0 volts.
      DrvResetIO();
#endif
   }
}
/**=== ResetModuleDriver_UT =======================================

   Reset module drivers

   Name              Dir   Description
   N/A               In    N/A

   Returns:

   Cautions: This function is only defined/used in the x86 environment.
   for unit testing

=========================================================================*/
extern void ResetModuleDriver_UT(void)
{
   pDRVPARS = NULL;
   pDRVSTAT = NULL;
   pDRVVARS = NULL;
   pDRVHIST = NULL;

}
#endif // #ifdef _TARG_IX86
#define DRVPARS_TPIPROTOCOL  pDRVPARS->fEnableMSTPonTPIPort
/*==== DrvGetMstpStatOnPortS2 ============================================

   Get the BACnet DLL type assigned to the MSTP Port.

   Name             Dir   Description


   Returns:
       Returns TRUE if MSTP enabled on TPI port

=========================================================================*/
extern BYTE DrvGetProtocolOnPortS2(void)
{
   return nvhw.bPortS2Protocol;
}

/*==== DrvSetMstpStatOnPortS2 ============================================

   Set protocol on PortS2.

   Name           Dir      Description
   protocol       in       Protocol to select on PortS2 (TPI_DLL_MODBUS,
                           TPI_DLL_MSTP or TPI_DLL_DISABLED)

   Returns:
       Returns TRUE if MSTP enabled on TPI port

=========================================================================*/
extern BYTE DrvSetProtocolOnPortS2(BYTE protocol)
{
   if((protocol != TPI_DLL_MSTP) && (bHomeNetworkIndex == NG_PORTS2_NETWORK_IDX))
   {
      DisableHomeNetworkonPortS2();
   }

   if(protocol == TPI_DLL_MODBUS) //TPI_DLL_MODBUS(2)
   {
      // If Modbus Selected from Drop Down List in Port Page then Enable Modbus
      // and Disable MSTP on Port S2
      IpdSetModbusSrlCommPars(MSRL2_STATUS, 2);
   }
   else if(protocol == TPI_DLL_MSTP)
   {
      // If MSTP Selected from Drop Down List in Port Page then Enable MSTP with
      // Autogenrated Number and Disable Modbus on Port S2
      pSyspars->awNet[NG_PORTS2_NETWORK_IDX]  = AUTO_CONFIGURATION;
      IpdSetModbusSrlCommPars(MSRL2_STATUS, MODBUS_SRL_PORT_DISABLE);
   }
   else
   {
      // If No Protocol Selected Disable Both MSTP and Modbus
      IpdSetModbusSrlCommPars(MSRL2_STATUS, MODBUS_SRL_PORT_DISABLE);
   }

   /* Save protocol in to the file */
   nvhw.bPortS2Protocol = protocol;
   pDRVSTAT->bProtocolSelectedOnPort[PORTS2] = nvhw.bPortS2Protocol;
   SavePortHWConfig();

   return TRUE;
}

/**=== DrvAdjustPortS2NvramContent =======================================
   Driver specific function called to adjust the PortS1 protocol
   in nwhw ram structure.

   Name        Dir   Description

   Returns:
      void

   Cautions:

=========================================================================*/
extern void DrvAdjustPortS2NvramContent(void)
{
   BYTE bProtocolOnPortS2 = TPI_DLL_DISABLED;

   /* Get Protocol on PortS2 */
   bProtocolOnPortS2 = DllMgmtGetPortS2Protocol();
   /* Does nwhw file need updated? */
   if (nvhw.bPortS2Protocol != bProtocolOnPortS2) // Yes
   {
      // Retain the MAC address on PortS2
      DrvSetProtocolOnPortS2(bProtocolOnPortS2);
   }
}

/*==== DrvGetModbusStatOnPortS2 ============================================

   Get the Modbus Status on Port S2.

   Name             Dir   Description


   Returns:
       Returns TPI_DLL_MODBUS if Modbus enabled on TPI port

=========================================================================*/
extern BYTE DrvGetModbusStatOnPortS2(void)
{
      BYTE bIpdIndex = 0;
   BYTE portValue = 0 ;
   BYTE ret = TPI_DLL_DISABLED;
   IPD_RUN *pipd;
   IPD_COMMPARS   *pComm;

   bIpdIndex = IpdGetIpdIndex(MODBUS_SRL2_SGNTR);
   if (bIpdIndex == -1)
   {
      return (FALSE);
   }
   pipd = &aIpd[bIpdIndex];
   pComm = pipd->pComm;
   if(pComm == NULL)
   {
      return (FALSE);
   }

   portValue = IpdGetPortValue(MODBUS_SRL2_SGNTR);

   if(portValue == TPI_DLL_MODBUS)
   {
      ret = TPI_DLL_MODBUS;
   }
   return ret ;
}
/*==== DrvSetMstpModbusProtOnPortS2 ======================================

   Activate Mstp or Modbus on  TPI Port depending on UserInput.

   Name             Dir   Description


   Returns:
       Returns nothing

=========================================================================*/
extern BOOL DrvSetMstpModbusProtOnPortS2(int Mstp_Modbus)
{
   /* We will set DRVPARS_TPIPROTOCOL flag here
   * BpManageTPINetwork function is responsible to
   * activate selected protocol */
    BOOL ret =FALSE ;
    switch (Mstp_Modbus)
    {
        case TPI_DLL_MSTP:
          if (ValidateNetworkNumber(NG_PORTS2_NETWORK_IDX))
          DRVPARS_TPIPROTOCOL = TPI_DLL_MSTP;
          ret = TRUE ;
          break;
        case TPI_DLL_MODBUS:
          //disable Mstp and activate Modbus
          DRVPARS_TPIPROTOCOL = TPI_DLL_MODBUS;
          ret = TRUE ;
          break;
        default:
          DRVPARS_TPIPROTOCOL = TPI_DLL_DISABLED;
          break;
    }
    return ret;
}

/*==== DrvSetFlowCalibData ================================================

   update T&B Flow calibration parameters

   Name           Dir      Description
   rVal           in       Value to set in Driver stat
   iIndex         in       Index of Value to set
   Returns:
       Returns nothing

=========================================================================*/
extern BOOL DrvSetFlowCalibData(FLOAT rVal, INT iIndex)
{
   BOOL ret = FALSE ;
   if (iIndex < 0 && iIndex > 11 && pDRVSTAT)
     ret = FALSE;
   else
   {
      pDRVSTAT->arTabData[iIndex] = rVal;
      ret = TRUE;
   }
   return ret;
}

/*==== DrvSetAutoZero ======================================================

   update T&B Flow calibration parameters

   Name             Dir   Description
   sAutoZero        in    sAutoZero value to set in Driver stat
   iIndex           in       Index of flow sensor to set

   Returns:
       Returns nothing

=========================================================================*/
extern BOOL DrvSetAutoZero(SHORT sAutoZero, INT iIndex)
{
   BOOL ret = FALSE ;
   if (iIndex < 0 && iIndex > 1 && pDRVSTAT)
     ret = FALSE;
   else
   {
      pDRVSTAT->asTabAutoZero[iIndex] = sAutoZero;
      ret = TRUE;
   }
   return ret;
}

/*========================================================================
   TELNET DIAG PORT FEATURES
=========================================================================*/
/*=== DrvTelnetInitialize ===============================================

   Initializes the diagnostic port task and associated kernel objects.

   Returns: void

=========================================================================*/
extern void DrvTelnetInitialize(void)
{
   DiagTelnetInitialize();
}

/*=== DrvTelnetLogin =====================================================

   Checks the login request for acceptance by the diagnostic port
   task.  If the login name matches, and we're able to support the
   telnet session, then the socket will be shared to the diagnostic
   port task.  If this is successful, the calling function is expected
   to close its socket because its handling has been transferred.

   Name     Dir      Desc
   sd       in       socket descriptor from the calling task
   pszLogin in       login name provided by the client at the login prompt

   Returns: -1 if the login is not accepted (failure)
            +1 if the login is accepted (success)

=========================================================================*/
extern int DrvTelnetLogin(int sd, char *pszLogin)
{
   return DiagTelnetLogin(sd, pszLogin);
}

/**=== ActnetAddressConfig =====================================

   Function should be called from DrvReinit(),this function updates the
   address configuration parameters for Actnet Module

   Name        Dir   Description
      none

   Returns:
      none

   Cautions:
=========================================================================*/
static void ActnetAddressConfig(void)
{
   if (pDRVSTAT)
   {
      fActnetConfigUpdate =TRUE;
      SetActnetConfigParms(pDRVSTAT->aACTnetConfig);
   }
}

/*==== DrvHasPortSetAutobaud =======================================

   Monitor DRVPARS for a PORTS1 and POSRTS2 for autobaud selection
   If autobaud selected , return TRUE.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     TRUE/FALSE

=========================================================================*/
extern BOOL DrvHasPortSetAutobaud(ULONG ulPort)
{
   BOOL fRetValue=FALSE;
   if (pDRVPARS)
   {
      if(pDRVPARS->bMstpAutobaud[ulPort])
         fRetValue=TRUE;
   }
   return(fRetValue);
}


/*==== DrvPortSetAutobaud =======================================

   Sets DRVPARS for a PORTS1 and POSRTS2 for autobaud selection
   from Local Access , return TRUE on success ,FALSE on failure.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     TRUE/FALSE

=========================================================================*/
extern BOOL DrvPortSetAutobaud(ULONG ulPort, BOOL fSetvalue )
{
   BOOL fRetValue=FALSE;
   if (pDRVPARS)
   {
      pDRVPARS->bMstpAutobaud[ulPort] = (BYTE)fSetvalue;
      fRetValue=TRUE;
      SavePortHWConfig();
   }
   return(fRetValue);
}

/*==== DrvUpdateAutobaudStats =======================================
   This function update the DRVSTAT parameters of mstp autobaud.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     NA

=========================================================================*/
extern void DrvUpdateAutobaudStats(ULONG ulPort, char *status, ULONG ulAutobaud)
{
   if(pDRVSTAT)
   {
      if(status)
      {
         sprintf(pDRVSTAT->achAutoStatus[ulPort],"%s",status);
         if(ulAutobaud)
         {
            pdevStat->aulPortBaud[ulPort] = ulAutobaud;
         }
      }
   }
}

/*==== DrvGetAutobaudStatus =======================================

   This function returns the DRVSTAT autobaud status in string format
   If autobaud selected , return TRUE.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     autobaud status to pszString

=========================================================================*/
extern void DrvGetAutobaudStatus(ULONG ulPort,char* pszString, size_t sizeString)
{
   if(pDRVSTAT)
   {
      if(pszString)
      {
         sprintf(pszString,"%s",pDRVSTAT->achAutoStatus[ulPort]);
      }
   }

}

/*==== GetAutobaudFromFile =======================================

   This function returns integer value (autobaud) from the pcFilepath


   Name                    Dir   Description
   N/A                     N/A   None

   Returns: -1 if read/file not exists (failure)
            >0 if read baudrate (success)
=========================================================================*/
extern INT GetAutobaudFromFile(char *pcFilepath)
{
   CHAR achAutobaud[MAX_AUTOBAUD_LENGTH];
   INT iFileDes, iDataReadCount, iBaudrate;

   if(!pcFilepath)
   {
      return -1;
   }

   memset(achAutobaud, 0, MAX_AUTOBAUD_LENGTH);
   iBaudrate = -1;

   // Open the file in which baudrate stored
   iFileDes = open (pcFilepath, O_RDONLY | O_NOCTTY);
   if(iFileDes < 0)
   {
      return -1;
   }
   else
   {
      // Reading the baudrate from the file fd
      iDataReadCount = read(iFileDes, achAutobaud, MAX_AUTOBAUD_LENGTH);
      if(iDataReadCount != 0)
      {
         // Converting the baudrate string to integer
         iBaudrate = strtol(achAutobaud, NULL, 10);
      }
   }
   close(iFileDes);
   return iBaudrate;
}

/**=== SetAutobaudToFile =============================================

   This function stores the autobaud value to pcFilepath path

   Name                    Dir   Description
   N/A                     N/A   None

   Returns: -1 if write/file not exists (failure)
            >0 if  baudrate (success)
=========================================================================*/
extern INT SetAutobaudToFile(char *pcFilepath,ULONG ulAutobaud)
{
   INT iFileDes, iRet;
   CHAR achAutobaud[MAX_AUTOBAUD_LENGTH];

   if(!pcFilepath)
   {
      return -1;
   }

   // Open the file in which baudrate stored
   iFileDes = open (pcFilepath, O_WRONLY | O_NOCTTY | O_CREAT, 0777);
   if(iFileDes < 0)
   {
      return -1;
   }
   else
   {
      // Converting the baudrate integer to string
      sprintf(achAutobaud, "%ld", ulAutobaud);
      // Writing the baudrate string to the file
      iRet = write(iFileDes, achAutobaud, MAX_AUTOBAUD_LENGTH);
      if(iRet < 0)
      {
         close(iFileDes);
         return -1;
      }
   }
   close(iFileDes);
   return 0;
}


/*==== GetMstpAutobaudValue =======================================

   This function returns the autobaud value which is read from file if
   no file is present then value 0 is returned.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     0 if no file exists from file if any.
     / any value from file

=========================================================================*/
extern INT GetMstpAutobaudValue(ULONG ulPort)
{
   INT iAutobaudValue=0L;
   CHAR achAutobaudFile[DEFAULT_MAX_STR_LEN];

   // construct the autobaud file name based upon the port number provided
   snprintf(achAutobaudFile, sizeof(achAutobaudFile), BPMSTP_AUTOBAUD_PATH "%lu", ulPort);
   if (access(achAutobaudFile, F_OK) == 0)
   {
      iAutobaudValue = GetAutobaudFromFile(achAutobaudFile);
      if(iAutobaudValue <= 0)
      {
         // opening of file/read failed, so set autobaud to default baud
         // setting default baud will make kernel module, assume the baud is from file so
         // all auto-baud enabled devices will wait till a Manual-Configured / Router
         // device to communicate first
         iAutobaudValue = 76800L;
      }
   }
   else
   {
      // setting default baud as there is no file present,
      // this will make kernel module, assume the baud is from file so
      // all auto-baud enabled devices will wait till a Manual-Configured / Router
      // device to communicate first
      iAutobaudValue = 76800LL;
   }
   return iAutobaudValue;
}

/*==== DrvPortSetAutobaudSync =======================================

   Sets DEVICE_STAT for a PORTS1 or POSRTS2 for autobaud sync selection
   from Local Access , return TRUE on success ,FALSE on failure.

   Name                    Dir   Description
   N/A                     N/A   None

   Returns:
     TRUE/FALSE

=========================================================================*/
extern BOOL DrvPortSetAutobaudSync(ULONG ulPort, BOOL fSetvalue )
{
   BOOL fRetValue=FALSE;
   if (pdevStat)
   {
      if(ulPort == PORTS1)
      {
         pdevStat->fPortS1AutoSync = (BYTE)fSetvalue;
      }
      else
      {
         pdevStat->fPortS2AutoSync = (BYTE)fSetvalue;
      }
      fRetValue=TRUE;
   }
   return(fRetValue);
}

/*==== DrvIsHWPortS1Supported ============================================
   Returns TRUE if H/W PortS1 supported

   Name        Dir   Description
   None

   Returns:
      TRUE or FALSE

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern BOOL DrvIsHWPortS1Supported(void)
{
   WORD wProductType = (WORD)Boot(BOOT_GET_PRODUCT_TYPE);
   BOOL fIsHwPortS1Supported = FALSE;

   // If no portS1 hardware, return false
   if (!isPortS1HdwSupported())
   {
      return fIsHwPortS1Supported;
   }

   fIsHwPortS1Supported = DrvIsPortS1SupportedByProdType();

   return (fIsHwPortS1Supported);
}


/*==== DrvIsHWPortS2Supported ============================================
   Returns TRUE if Product Type supports H/W PortS2

   Name        Dir   Description
   None

   Returns:
      TRUE or FALSE

   Cautions:
      This should only be called via Local Access (data_functions.c)
=========================================================================*/
extern BOOL DrvIsHWPortS2Supported(void)
{
   WORD wProductType = (WORD)Boot(BOOT_GET_PRODUCT_TYPE);
   BOOL fIsHwPortS2Supported = FALSE;

   switch(wProductType)
   {
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_HIGH_TIER_ROUTER_CARRIER:
      case GEN5_HIGH_TIER_ROUTER_ALC:
      case GEN5_BASE_TIER_ROUTER_CARRIER:
      case GEN5_BASE_TIER_ROUTER_ALC:
      case GEN5_MECPU_ALC:
      case GEN5_MECPU_CARRIER:
      case GEN5_MECPU_OEM:
      case GEN5_MECPU_PTR:
      case GEN5_MECPU_OFHI:
      case GEN5_MECPU_OFHI_A:
      case GEN5_NON_ROUTER_ALC:
      case GEN5_TV_MPCXP1628:
      case GEN5_TV_MPCXP1628_NR:
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_OF1628:
      case GEN5_OF1628_NR:
      case GEN5_OF028_NR:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
      /* Next Gen Zone Controllers  */

      case GEN5_OF683T_E2:
      case GEN5_SS_683TX:
      case GEN5_NGZNDEV:
         fIsHwPortS2Supported = TRUE;
         break;

      /* Next Gen Zone Controllers  */
      case GEN5_OF683XT_E2:
      case GEN5_OF683_ARC:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UC683_ARC:
      case GEN5_TV_UC683T:
      case GEN5_TV_FC:
      case GEN5_TV_UV:
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_SS_683ETX:
      case GEN5_OF253A_E2:
      case GEN5_TV_UCXP683T_V:
      case GEN5_OF561_E2:
      case GEN5_OF683_E2:
      case GEN5_TV_UC561:
      case GEN5_TV_UC253_V:
      case GEN5_TV_SQ_PSM:
      case GEN5_OF141_E2:
      case GEN5_OF342_E2:
      case GEN5_TV_VVTZC_E2:
      case GEN5_TV_VVTBP_E2:
      case GEN5_TV_VAVB3_E2:
      case GEN5_SS_342A_E2:
      case GEN5_OF342_ARC:
      case GEN5_TV_VVTZC:
      case GEN5_TV_VVTBP:
      case GEN5_TV_VAVB3:
      case GEN5_SS_342A:
      case GEN5_NGVAVDEV:
         fIsHwPortS2Supported = FALSE;
         break;

      default:
         Boot(BOOT_LOG_FATAL, (ULONG)0, (ULONG)0, "Invalid Product Type");
         fIsHwPortS2Supported = FALSE;
         break;
   }

   return (fIsHwPortS2Supported);
}

/*==== DrvIsOEMChannel ===================================================
   Returns TRUE if Product Type is from OEM

   Name        Dir   Description
   None

   Returns:
      TRUE or FALSE

=========================================================================*/
extern BYTE DrvIsOEMChannel(void)
{
   switch ((WORD)Boot(BOOT_GET_PRODUCT_TYPE))
   {
      /* OEM Variant */
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_MECPU_OEM: 
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
         return (TRUE);
   }

   return (FALSE);
}

/**=== DrvGetDefaultProtocolOnPortS1 =====================================

   Get Default Value of Protocol Selection for different variant of
   product type

   Name        Dir   Description


   Returns: UINT8 - Protocol Value on Port S1


=========================================================================*/
extern BYTE DrvGetDefaultProtocolOnPortS1(USHORT usEepromProductType)
{
   BYTE ucProtocolOnPortS1 = ROTARY_PROTO_NONE;

   switch(usEepromProductType)
   {
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_HIGH_TIER_ROUTER_CARRIER:
      case GEN5_HIGH_TIER_ROUTER_ALC:
      case GEN5_BASE_TIER_ROUTER_CARRIER:
      case GEN5_BASE_TIER_ROUTER_ALC:
      case GEN5_MECPU_ALC:
      case GEN5_MECPU_CARRIER:
      case GEN5_MECPU_OEM:
      case GEN5_MECPU_PTR:
      case GEN5_MECPU_OFHI:
      case GEN5_MECPU_OFHI_A:
      case GEN5_NON_ROUTER_ALC:
      case GEN5_TV_MPCXP1628:
      case GEN5_TV_MPCXP1628_NR:
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_OF1628:
      case GEN5_OF1628_NR:
      case GEN5_OF028_NR:
      /* Next Gen Zone Controllers  */
      case GEN5_OF683XT_E2:
      case GEN5_OF683T_E2:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_OF253A_E2:
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UCXP683T_V:
      case GEN5_TV_UC683T:
      case GEN5_TV_FC:
      case GEN5_TV_UV:
      case GEN5_SS_561TX:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
      case GEN5_NGZNDEV:
      case GEN5_OF561_E2:
      case GEN5_OF683_E2:
      case GEN5_TV_UC561:
      case GEN5_TV_UC253_V:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
      case GEN5_TV_SQ_PSM:
      case GEN5_OF141_E2:
      case GEN5_OF342_E2:
      case GEN5_TV_VVTZC_E2:
      case GEN5_TV_VVTBP_E2:
      case GEN5_TV_VAVB3_E2:
      case GEN5_SS_342A_E2:
      case GEN5_NGVAVDEV:
         ucProtocolOnPortS1 = ROTARY_PROTO_NONE;
         break;

      case GEN5_OF683_ARC:
      case GEN5_OF342_ARC:
         ucProtocolOnPortS1 = ROTARY_PROTO_ARCNET;
         break;

      case GEN5_TV_UC683_ARC:
      case GEN5_SS_683:
      case GEN5_TV_VVTZC:
      case GEN5_TV_VVTBP:
      case GEN5_TV_VAVB3:
      case GEN5_SS_342A:
         ucProtocolOnPortS1 = ROTARY_PROTO_MSTP;
         break;

      default:
         Boot(BOOT_LOG_FATAL, (ULONG)0, (ULONG)0, "Invalid Product Type");
         ucProtocolOnPortS1 = ROTARY_PROTO_NONE;
         break;
   }

   // Exit with ROTARY_PROTO_NONE if no PortS1 hardware available
   // or if FPGA in bypass, change arcnet to ROTARY_PROTO_NONE
   if (!isPortS1HdwSupported() ||
       (!isArcnetHdwSupported() && ucProtocolOnPortS1 == ROTARY_PROTO_ARCNET) )
   {
      ucProtocolOnPortS1 = ROTARY_PROTO_NONE;
   }

   return (ucProtocolOnPortS1);
}

#define IP_MODE_DEFAULT    (1 << 0)
#define IP_MODE_STATIC     (1 << 1)
#define IP_MODE_DHCP       (1 << 2)

#define IP_MODE_CAPABILITY_WITH_ROTARY_SWITCH         (IP_MODE_DEFAULT | IP_MODE_DHCP | IP_MODE_STATIC)
#define IP_MODE_CAPABILITY_WITHOUT_ROTARY_SWITCH      (IP_MODE_DHCP | IP_MODE_STATIC)

/**=== DrvGetIPModeCapability ============================================

   Get IP Mode capability of device
   E.g: Default IP, DHCP and Static IP

   Name        Dir   Description


   Returns: UINT8 - support of different IP mode is indicated using bit

   Default IP        (1 << 0)
   Static IP         (1 << 1)
   DHCP              (1 << 2)

   NOTE:
      This is currently used for Local Network feature

=========================================================================*/
extern UINT8 DrvGetIPModeCapability(void)
{
   UINT8 ipModeCapability = IP_MODE_CAPABILITY_WITH_ROTARY_SWITCH;

   switch((WORD)Boot(BOOT_GET_PRODUCT_TYPE))
   {
      case GEN5_HIGH_TIER_ROUTER_OEM:
      case GEN5_HIGH_TIER_ROUTER_CARRIER:
      case GEN5_HIGH_TIER_ROUTER_ALC:
      case GEN5_BASE_TIER_ROUTER_CARRIER:
      case GEN5_BASE_TIER_ROUTER_ALC:
      case GEN5_MECPU_ALC:
      case GEN5_MECPU_CARRIER:
      case GEN5_MECPU_OEM:
      case GEN5_MECPU_PTR:
      case GEN5_MECPU_OFHI:
      case GEN5_MECPU_OFHI_A:
      case GEN5_NON_ROUTER_ALC:
      case GEN5_TV_MPCXP1628:
      case GEN5_TV_MPCXP1628_NR:
      case GEN5_LS_1628:
      case GEN5_VLINK_IO_1628U:
      case GEN5_OF1628:
      case GEN5_OF1628_NR:
      case GEN5_OF028_NR:
      case GEN5_OF_PSM:
      case GEN5_OF_OPT:
      case GEN5_TV_PSM:
      case GEN5_TV_OPT:
         ipModeCapability = IP_MODE_CAPABILITY_WITH_ROTARY_SWITCH;
         break;

      /* Next Gen Zone Controllers  */
      case GEN5_OF683XT_E2:
      case GEN5_OF683T_E2:
      case GEN5_TV_UCXP683T:
      case GEN5_TV_UCXP683T_V:
      case GEN5_SS_683TX:
      case GEN5_SS_683ETX:
      case GEN5_NGZNDEV:
      case GEN5_OF683_ARC:
      case GEN5_OF561T_E2:
      case GEN5_OF253T_E2:
      case GEN5_OF253A_E2:
      case GEN5_TV_UC683_ARC:
      case GEN5_TV_UC683T:
      case GEN5_TV_FC:
      case GEN5_TV_UV:
      case GEN5_SS_561TX:
      case GEN5_SS_683:
      case GEN5_OF561_E2:
      case GEN5_OF683_E2:
      case GEN5_TV_UC561:
      case GEN5_TV_UC253_V:
      case GEN5_TV_SQ_PSM:

      /* Next Gen VAV Controllers  */
      case GEN5_OF141_E2:
      case GEN5_OF342_E2:
      case GEN5_TV_VVTZC_E2:
      case GEN5_TV_VVTBP_E2:
      case GEN5_TV_VAVB3_E2:
      case GEN5_SS_342A_E2:
      case GEN5_OF342_ARC:
      case GEN5_TV_VVTZC:
      case GEN5_TV_VVTBP:
      case GEN5_TV_VAVB3:
      case GEN5_SS_342A:
      case GEN5_NGVAVDEV:
         ipModeCapability = IP_MODE_CAPABILITY_WITHOUT_ROTARY_SWITCH;
         break;

      default:
         Boot(BOOT_LOG_FATAL, (ULONG)0, (ULONG)0, "Invalid Product Type");
         ipModeCapability = IP_MODE_CAPABILITY_WITH_ROTARY_SWITCH;
         break;
   }

   return (ipModeCapability);
}

/**=== GetCurrentProtocolValueOnPortS1 ====================================

   Returns current rotary protocol switch value for H/W with rotary switch
   support.
   For H/W which does not support rotary switches, It reads stored
   value of protocol selected on port s1.

   Name        Dir   Description


   Returns: int - positive rotary switch value on success, negative value
   if error(s)

   Cautions:
   For H/W with rotary switch support, Only reads IO if error on previous read.
   Depends on dll manager to read IO for up to date value

=========================================================================*/
extern int GetCurrentProtocolValueOnPortS1(void)
{
   int bProtocolOnPortS1 = 0;
   // if error with protocol or protocol has not been read
   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      bProtocolOnPortS1 = ReadProtocolRotaryIO();
   }
   else
   {
      bProtocolOnPortS1 = (int)DrvGetProtocolOnPortS1();
   }

   // If PortS1 hardware missing or FPGA in bypass & set to arcnet, return zero
   if (!isPortS1HdwSupported() || 
       (!isArcnetHdwSupported() && bProtocolOnPortS1 == ROTARY_PROTO_ARCNET) )
   {
      bProtocolOnPortS1 = 0;
   }

   return bProtocolOnPortS1;
}

/**=== GetProtocolValueOnPortS1 ===========================================

   Returns previously read rotary protocol switch for H/W supporting
   rotary switch and
   For H/W which does not support rotary switches, It reads stored
   value of protocol selected on port s1.

   Name        Dir   Description


   Returns: int - positive rotary switch value on success, negative value
   if error(s)

   Cautions:
   For H/W with rotary switch support, Only reads IO if error on previous read.
   Depends on dll manager to read IO for up to date value

=========================================================================*/
extern int GetProtocolValueOnPortS1(void)
{
   int bProtocolOnPortS1 = 0;
   // if error with protocol or protocol has not been read
   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      bProtocolOnPortS1 = GetProtocolRotaryValue();
   }
   else
   {
      bProtocolOnPortS1 = (int)DrvGetProtocolOnPortS1();
   }

   // If PortS1 hardware missing or FPGA in bypass & set to arcnet, return zero
   if (!isPortS1HdwSupported() || 
       (!isArcnetHdwSupported() && bProtocolOnPortS1 == ROTARY_PROTO_ARCNET) )
   {
      bProtocolOnPortS1 = 0;
   }

   return bProtocolOnPortS1;
}

/**=== GetCurrentAddressValueOnPortS1 =====================================

   Returns mac address for MSTP and ARCNET , Slave ID for MODBUS from three
   address rotary switch for the H/W supporting rotary switches.
   For H/W which does not support rotary switches, It reads stored MAC
   address based on protocol selected on port s1.

   Name        Dir   Description


   Returns: int - Returns mac address for MSTP and ARCNET , Slave ID for MODBUS


=========================================================================*/
extern int GetCurrentAddressValueOnPortS1(void)
{
   int iAddrOnPortS1 = 0,iProtocolOnPortS1 = 0;

   // if error with address or address has not been read
   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      iAddrOnPortS1 = ReadAddressRotaryIO();
   }
   else
   {
      iProtocolOnPortS1 = (int)DrvGetProtocolOnPortS1();
      if (iProtocolOnPortS1 == ROTARY_PROTO_ARCNET)
      {
         iAddrOnPortS1 = DrvGetNvhwARCnetMacAddr();
      }
      else if(iProtocolOnPortS1 == ROTARY_PROTO_MSTP)
      {
         iAddrOnPortS1 = DrvGetNvhwMstpMacAddr();
      }
      else if(iProtocolOnPortS1 == ROTARY_PROTO_MODBUS)
      {
         iAddrOnPortS1 = DrvGetModbusMacAddr();
      }
   }

   return iAddrOnPortS1;
}

/**=== GetCurrentAddressValueOnPortS2 =====================================

   Returns mac address for MSTP and Master/Slave ID for MODBUS address
   based on protocol selected on port s2.

   Name        Dir   Description
   N/A

   Returns: int - Returns mac address for MSTP and Slave ID for the
                  MODBUS address. -1 for MODBUS Master.

   Cautions: Returns -1 for MODBUS master address. (See above.)

=========================================================================*/
static int GetCurrentAddressValueOnPortS2(void)
{
   int iAddrOnPortS2 = 0, iProtocolOnPortS2 = 0;
   ULONG ulModbusVal = 0;

   iProtocolOnPortS2 = (int)DrvGetProtocolOnPortS2();


   if (iProtocolOnPortS2 == TPI_DLL_MSTP)
   {
      iAddrOnPortS2 = DrvGetMacAddrOnPortS2();
   }
   else if (iProtocolOnPortS2 == TPI_DLL_MODBUS)
   {
      IpdGetModbusSrlPars(MSRL2_MASTER_SLAVE, &ulModbusVal);
      if (ulModbusVal == MODBUS_IS_MASTER)
      {
         // Modbus slaves are address 1-255. Technically, address 0
         // is reserved for broadcast messages, sending back -1
         // for master. (Caller must deal with it.)
         iAddrOnPortS2 = -1;
      }
      else if (ulModbusVal == MODBUS_IS_SLAVE)
      {
         IpdGetModbusSrlPars(MSRL2_SLAVE_ADDRESS, &ulModbusVal);
         iAddrOnPortS2 = (int)ulModbusVal;
      }
   }

   return iAddrOnPortS2;
}

/**=== GetCurrentMstpArcnetAddressValueOnPortS1 ===========================

   Returns mac address for MSTP and ARCNET from three
   address rotary switch for the H/W supporting rotary switches.
   For H/W which does not support rotary switches, It reads stored MAC
   address based on protocol selected on port s1.

   Name        Dir   Description


   Returns: int - Returns mac address for MSTP and ARCNET , Slave ID for MODBUS


=========================================================================*/
extern int GetCurrentMstpArcnetAddressValueOnPortS1(void)
{
   int iAddrOnPortS1 = 0, iProtocolOnPortS1 = 0;

   // if error with address or address has not been read
   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      iAddrOnPortS1 = ReadAddressRotaryIO();
   }
   else
   {
      iProtocolOnPortS1 = (int)DrvGetProtocolOnPortS1();
      if (iProtocolOnPortS1 == ROTARY_PROTO_ARCNET)
      {
         iAddrOnPortS1 = DrvGetNvhwARCnetMacAddr();
      }
      else if(iProtocolOnPortS1 == ROTARY_PROTO_MSTP)
      {
         iAddrOnPortS1 = DrvGetNvhwMstpMacAddr();
      }
   }

   return iAddrOnPortS1;
}

/**=== GetMstpArcnetAddressValueOnPortS1 =================================

   Returns mac address for MSTP and ARCNET from three
   address rotary switch for the H/W supporting rotary switches.
   For H/W which does not support rotary switches, It reads stored MAC
   address based on protocol selected on port s1.

   Name        Dir   Description


   Returns: int - Returns mac address for MSTP and ARCNET , Slave ID for MODBUS


=========================================================================*/
extern int GetMstpArcnetAddressValueOnPortS1(void)
{
   int iAddrOnPortS1 = 0, iProtocolOnPortS1 = 0;

   // if error with address or address has not been read
   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      iAddrOnPortS1 = GetAddressRotaryValue();
   }
   else
   {
      iProtocolOnPortS1 = (int)DrvGetProtocolOnPortS1();
      if (iProtocolOnPortS1 == ROTARY_PROTO_MODBUS)
      {
         iAddrOnPortS1 = DrvGetModbusMacAddr();
      }
      else if (iProtocolOnPortS1 == ROTARY_PROTO_ARCNET)
      {
         iAddrOnPortS1 = DrvGetNvhwARCnetMacAddr();
      }
      else if(iProtocolOnPortS1 == ROTARY_PROTO_MSTP)
      {
         iAddrOnPortS1 = DrvGetNvhwMstpMacAddr();
      }
   }

   return iAddrOnPortS1;
}

/**=== GetAddressValueOnPortS1 ===========================================

   Returns previously read rotary address switch for the H/W supporting
   rotary switches.
   For H/W which does not support rotary switches, It reads stored MAC
   address based on protocol selected on port s1.

   Name        Dir   Description


   Returns: int - positive rotary switch value on success, negative value
   if error(s)

   Cautions:
   Only reads IO if error on previous read.
   Depends on dll manager to read IO for up to date value

=========================================================================*/
extern int GetAddressValueOnPortS1(void)
{
   int iMacAddrOnPortS1 = 0,iProtocolOnPortS1 = 0;

   // if error with address or address has not been read
   if(IsFeatureSupported(ROTARY_SWITCH_SUPPORT))
   {
      iMacAddrOnPortS1 = GetAddressRotaryValue();
   }
   else
   {
      iProtocolOnPortS1 = (int)DrvGetProtocolOnPortS1();

      if (iProtocolOnPortS1 == ROTARY_PROTO_ARCNET)
      {
         iMacAddrOnPortS1 = DrvGetNvhwARCnetMacAddr();
      }
      else if(iProtocolOnPortS1 == ROTARY_PROTO_MSTP)
      {
         iMacAddrOnPortS1 = DrvGetNvhwMstpMacAddr();
      }
      else
      {
         iMacAddrOnPortS1 = (htonl(DrvGetIpInfo(IP_CURRENT_ADDR)) & (~htonl(DrvGetIpInfo(IP_CURRENT_SUBNET_MASK))));
         // if host number is equal to braodcast number then make value to zero
         if (iMacAddrOnPortS1 == (~htonl(DrvGetIpInfo(IP_CURRENT_SUBNET_MASK))))
         {
            iMacAddrOnPortS1 = 0;
         }
         else
         {
            // Maximum number we can use is 999 as first two digit is used for Vendor Id for autogenerate device id
            iMacAddrOnPortS1 = iMacAddrOnPortS1 % MAX_IP_DIGIT_WO_ROTARY;
         }
      }
   }

   return iMacAddrOnPortS1;
}

/**=== UpdateVendorInfoOnReinit ==========================================

   Updates the BACnet vendor ID and name on reinit, from a shadow copy,
   to cover the case where WebCTRL downloads BACnet file 1 in its entirety
   ignoring XML access specifies without "D" for download.

   Name        Dir   Description
   None.

   Returns: None.

=========================================================================*/
static void UpdateVendorInfoOnReinit(void)
{
   pdevPars->wVendorId = (WORD)BP_VENDOR_ID;

   // Add in the size of the variable size data header.
   pdevPars->strVendorName.vs.ulSize = EVENSIZE(VSDATA) + EVENSIZE(VSSTR) + DEFAULT_MAX_STR_LEN;
   pdevPars->strVendorName.vs.wType = VSTYPE_CHAR_STRING;

   pdevPars->strVendorName.vss.usMax = DEFAULT_MAX_STR_LEN;
   pdevPars->strVendorName.vss.usCurr = strlen(BP_VENDOR_STR);
   if (pdevPars->strVendorName.vss.usCurr > pdevPars->strVendorName.vss.usMax)
   {
      pdevPars->strVendorName.vss.usCurr = pdevPars->strVendorName.vss.usMax;
   }

   // Character set is always ANSI = 0.
   pdevPars->strVendorName.vss.bCharSet = 0;

   // Copy the ansi string to its destination.
   if (pdevPars->strVendorName.vss.usCurr)
   {
      memcpy(&(pdevPars->strVendorName.abStr), BP_VENDOR_STR, pdevPars->strVendorName.vss.usCurr);
   }
}

/*==== DrvProtocolOnPortS2 ============================================

   This subroutine is called periodically by the DLL management task to
   the derive the Protocol on PortS2 based on the user's input

   Name        Dir   Description
   none

   Returns:
   Returns the protocol to be run on port S2

=========================================================================*/

extern UINT8 DrvProtocolOnPortS2(void)
{
   //if user enables the modbus on ports2 from WebCtrl then if needed nvram value for ports2 should be in sync with user's input
   if((DrvGetModbusStatOnPortS2() == TPI_DLL_MODBUS) && (nvhw.bPortS2Protocol != TPI_DLL_MODBUS))
   {
      nvhw.bPortS2Protocol = TPI_DLL_MODBUS;
     SavePortHWConfig();
   }
   //if user disables the modbus on ports2 from WebCtrl then if needed nvram value for ports2 should be in sync with user's input
   else if((DrvGetModbusStatOnPortS2() != TPI_DLL_MODBUS) && (nvhw.bPortS2Protocol == TPI_DLL_MODBUS))
   {
      nvhw.bPortS2Protocol = TPI_DLL_DISABLED;
     SavePortHWConfig();
   }

   return nvhw.bPortS2Protocol;
}

/**=== DrvLoadEthPortConfiguration =======================================
   function called to load the current configurartion of ethernet port , so 
   as to detect future chnage in ethernet port configuration for the device

   Name        Dir   Description

   Returns:
      void

   Cautions:

=========================================================================*/
extern void DrvLoadEthPortConfiguration(void)
{
	bEthPortConfig = pDRVPARS->bDisableEthPort; 
}

/**=== DrvIsEthPortConfigChanged =======================================
   function called to check the change in the ethernet port configuration 

   Name        Dir   Description

   Returns:
     True - If configuration changed by user from webctrl by checking/unchecking the correspnoding checkbox
     False- If there is no change in configuration
   Cautions:

=========================================================================*/
extern BOOL DrvIsEthPortConfigChanged(void)
{
	if(bEthPortConfig != pDRVPARS->bDisableEthPort)
	{
	    Boot(BOOT_DEBUG, (DBG_MSG_API), "Ethernet port configuration has changed");
	    bEthPortConfig = pDRVPARS->bDisableEthPort;
	    return TRUE;
	}
	
	return FALSE;

}

/*==== ConfigureEthernetPort ============================================

   This subroutine is called to enable/disable the "eth0 port in MECPU " and "eth1 port in NGZN/NGVAV"
   based on the users input

   Name        Dir   Description
   none

   Returns:

=========================================================================*/

extern void ConfigureEthernetPort(void)
{
	UpdateEthConfigFile(NULL);
	Boot(BOOT_DEBUG, (DBG_MSG_API), "Updated eth_port_config.source file");
	BinaryFileCreateEmptyFile(ETH_PORT_CONFIG_MARKER_FILE, 4);
	Boot(BOOT_DEBUG, (DBG_MSG_API), "Successfully created eth_port_config.marker file");
}
