/*
 *  hello-4.c - Demonstrates module documentation.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include "linux/err.h"
#include "linux/file.h"

#define DRIVER_AUTHOR "Peter Jay Salzman <p@dirac.org>"
#define DRIVER_DESC   "A sample driver"

static int __init init_hello_4(void)
{
    printk(KERN_INFO "Hello, world!\n");

    struct file* file = filp_open("/dev/net/tun", O_RDWR, 0);
    printk(KERN_INFO "Opened tun file\n");

    struct ifreq ifr; memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    strcpy(ifr.ifr_name, "tun0");
    long res = vfs_ioctl(file, TUNSETIFF, (u_long)&ifr);
    if (res != 0) {
        printk(KERN_INFO "setting tun options failed with code: %ld\n", res);
    }

    struct socket* socket = tun_get_socket(file);
    if (IS_ERR_OR_NULL(socket)) {
        printk(KERN_INFO "fuck!: %lu\n", (long)socket);
    }

    return 0;
}

static void __exit cleanup_hello_4(void)
{
    printk(KERN_INFO "Goodbye, world 4\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

/*
 *  You can use strings, like this:
 */

/*
 * Get rid of taint message by declaring code as GPL.
 */
MODULE_LICENSE("GPL");

/*
 * Or with defines, like this:
 */
MODULE_AUTHOR(DRIVER_AUTHOR);	/* Who wrote this module? */
MODULE_DESCRIPTION(DRIVER_DESC);	/* What does this module do */

/*
 *  This module uses /dev/testdevice.  The MODULE_SUPPORTED_DEVICE macro might
 *  be used in the future to help automatic configuration of modules, but is
 *  currently unused other than for documentation purposes.
 */
// MODULE_SUPPORTED_DEVICE("testdevice");
