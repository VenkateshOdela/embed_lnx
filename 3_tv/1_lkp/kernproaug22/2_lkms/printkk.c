/*
 * ******************************************************************************
 * printk loglevel
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
 
/*
  pr_emerg : #define KERN_EMERG      KERN_SOH "0"     system is unusable

  pr_alert : #define KERN_ALERT      KERN_SOH "1"     action must be taken immediately

  pr_crit  : #define KERN_CRIT       KERN_SOH "2"     critical conditions

  pr_err   : #define KERN_ERR        KERN_SOH "3"     error conditions

  pr_warn  : #define KERN_WARNING    KERN_SOH "4"     warning conditions

  test logs:
  pr_notice: #define KERN_NOTICE     KERN_SOH "5"     normal but significant condition
  pr_info  : #define KERN_INFO       KERN_SOH "6"     informational

  debug logs:
  pr_debug : #define KERN_DEBUG      KERN_SOH "7"     debug-level messages
*/


static __init int log_init(void)
{
	printk(KERN_EMERG "loglevel 0\n");
	printk(KERN_ALERT "loglevel 1\n");
	printk(KERN_CRIT "loglevel 2\n");
	printk(KERN_ERR "loglevel 3\n");
	printk(KERN_WARNING "loglevel 4\n");
	printk(KERN_NOTICE "loglevel 5\n");
	printk(KERN_INFO "loglevel 6\n");
	printk(KERN_DEBUG "loglevel 7\n");

	return 0;
}

static __exit void log_exit(void)
{
	pr_emerg("loglevel 0\n");
	pr_alert("loglevel 1\n");
	pr_crit("loglevel 2\n");
	pr_err("loglevel 3\n");
	pr_warn("loglevel 4\n");
	pr_notice("loglevel 5\n");
	pr_info("loglevel 6\n");
	pr_debug("loglevel 7\n");
}

module_init(log_init);
module_exit(log_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("raghu@techveda.org");
MODULE_DESCRIPTION("printk log levels");

/*
 *
 * user@ubuntu:/var/log$ ls -l | grep syslog
-rw-r-----  1 syslog            adm             162076 Sep 25 21:02 syslog
-rw-r-----  1 syslog            adm              22651 Sep 25 11:57 syslog.1
-rw-r-----  1 syslog            adm               9126 Sep 24 00:06 syslog.2.gz
-rw-r-----  1 syslog            adm              13975 Sep 23 14:29 syslog.3.gz
-rw-r-----  1 syslog            adm              38089 Sep 22 10:29 syslog.4.gz
-rw-r-----  1 syslog            adm              21582 Sep 21 10:41 syslog.5.gz
-rw-r-----  1 syslog            adm              11242 Sep 20 12:51 syslog.6.gz
-rw-r-----  1 syslog            adm               6368 Sep 19 11:50 syslog.7.gz
 *
 */

/*
 * kernel ring buffer :
 * menuconfig
 * -- configuration
 * --  kernel log buffer size (CONFIG_LOG_BUF_SHIFT):
 * --  18 = 256 KB
 * --  17 = 128 KB
 * --  16 = 64 KB
 * --  15 = 32 KB
 * --  14 = 16 KB
 * --  13 = 8  KB
 * --  12 = 4  KB
 *
 *.config
 *
 * To see default size
 *  cat /boot/linux-gerneric
 *
 */
