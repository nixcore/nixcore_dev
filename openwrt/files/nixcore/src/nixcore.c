/*  nixcore.c - The simplest kernel module.
 * From: https://forum.openwrt.org/viewtopic.php?id=20066
 */
#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */

#define DRV_NAME	"testmodule"

static int __init test_init(void)
{
    printk(KERN_ALERT "Test module started\n");
	return 0;
}

static void __exit test_exit(void)
{
    printk(KERN_ALERT "Test module unloaded\n");
    return;
}

module_init(test_init);
module_exit(test_exit);

MODULE_AUTHOR("NixCores <admin@nixcores.com>");
MODULE_DESCRIPTION("Test driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DRV_NAME);
