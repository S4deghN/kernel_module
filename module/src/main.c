#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/file.h>
#include <linux/netdevice.h>
#include <net/sock.h>

#define DRIVER_AUTHOR "The Dude"
#define DRIVER_DESC   "A sample driver"

static int __init init_hello(void)
{
    printk(KERN_INFO "Hello, world!\n");

    struct file* file = filp_open("/dev/net/tun", O_RDWR, 0);
    printk(KERN_INFO "Opened tun file\n");

    struct ifreq ifr = {0};
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    strncpy(ifr.ifr_name, "tun0", 4);

    // Disable memory address validity checking since we are passing ioctl, args alcolated in kernel
    // address space.
    mm_segment_t fs;
    fs = get_fs();
    set_fs(KERNEL_DS);
    long res = vfs_ioctl(file, TUNSETIFF, (u_long)&ifr);
    set_fs(fs);
    if (res != 0) {
        printk(KERN_INFO "setting tun options failed with code: %ld\n", res);
    }

    struct socket* socket = tun_get_socket(file);
    if (IS_ERR_OR_NULL(socket)) {
        printk(KERN_INFO "Not nice!: %lu\n", (long)socket);
    } else {
        printk(KERN_INFO "Nice!: %lx\n", (long)socket);
    }

    return 0;
}

static void __exit cleanup_hello(void)
{
    printk(KERN_INFO "Goodbye, world\n");
}

module_init(init_hello);
module_exit(cleanup_hello);

/*
 * Get rid of taint message by declaring code as GPL.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);	/* Who wrote this module? */
MODULE_DESCRIPTION(DRIVER_DESC);	/* What does this module do */
