#include <linux/module.h>  // Included for all kernel modules
#include <linux/init.h>    // Included for __init and __exit macros
#include <linux/kernel.h>  // Included for KERN_INFO

static int __init hello_init (void) {
	printk(KERN_INFO "Hello world! (LOADING...)\n");
	return 0;
}

static void __exit hello_cleanup(void) {
	printk(KERN_INFO "Hello world! (EXITING...)\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
