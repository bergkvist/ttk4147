#include <linux/module.h>  // Included for all kernel modules
#include <linux/init.h>    // Included for __init and __exit macros
#include <linux/kernel.h>  // Included for KERN_INFO
#include <linux/proc_fs.h> // Allow the use of /proc files
#include <linux/kthread.h>
#include <linux/delay.h>

#define DRIVER_AUTHOR  "Tobias Bergkvist <tobias@bergkv.ist>"
#define DRIVER_DESC    "Should catch all network packets and print them"
#define DRIVER_DEVICE  "NGW100"
#define DRIVER_LICENSE "GPL"

struct proc_dir_entry *Proc_File;
static struct task_struct *thread_st;
volatile int seconds_since_started;

static int thread_fn(void *unused) {
	for (seconds_since_started = 0; !kthread_should_stop(); seconds_since_started++) {
		ssleep(1);
	}
	printk(KERN_INFO "thread stopping\n");
	do_exit(0);
	return 0;
}

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data) {
	if (offset > 0) return 0;
	return sprintf(buffer, "Number of seconds since starting: %d\n", seconds_since_started);
}

static int __init counter_init (void) {
	printk(KERN_INFO "Creating proc entry...\n");
	thread_st = kthread_run(thread_fn, NULL, "counter");
	Proc_File = create_proc_entry("counter", 0644, NULL);
	Proc_File->read_proc = procfile_read;
	return 0;
}

static void __exit counter_cleanup(void) {
	remove_proc_entry("counter", NULL);
	printk(KERN_INFO "Removed /proc/counter.\n");
	if (thread_st) kthread_stop(thread_st);
	printk(KERN_INFO "Stopped kthread. Exiting...\n");
}

module_init(counter_init);
module_exit(counter_cleanup);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE(DRIVER_DEVICE);
