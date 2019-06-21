#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stewart Sentanoe");
MODULE_DESCRIPTION("Hello World!");
MODULE_VERSION("0.01");

int g_time_interval = 1000;
struct timer_list g_timer;

void _vmcall(void)
{
	__asm__ (
                        "mov $0xbf05000000000006, %rax;"
                        "vmcall;"
                );	
}

void _TimerHandler(struct timer_list *t)
{
    /*Restarting the timer...*/
    mod_timer( &g_timer, jiffies + msecs_to_jiffies(g_time_interval));
    _vmcall();

    printk(KERN_INFO "Timer Handler called.\n");
}


static int __init lkm_example_init(void) {
	printk(KERN_INFO "Hello, World!\n");
	_vmcall();

	/*Starting the timer.*/
	timer_setup(&g_timer, _TimerHandler, 0);
	mod_timer( &g_timer, jiffies + msecs_to_jiffies(g_time_interval));

	return 0;
}

static void __exit lkm_example_exit(void) {
	del_timer(&g_timer);
	printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
