//
// Created by OriB on 28/05/2019.
//

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

/* the current pid */
static long l_pid;

static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
        .write = proc_write
};


/* This function is called when the module is loaded. */
int proc_init(void)
{

    // creates the /proc/hello entry
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
    proc_create("pid", 0, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

    // removes the /proc/hello entry
    remove_proc_entry("pid", NULL);

    printk( KERN_INFO "/proc/%s removed\n", "pid");
}

/**
 * This function is called each time the /proc/hello is read.
 *
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
int rv = 0;
char buffer[BUFFER_SIZE];
struct task_struct *tsk = NULL;
static int completed = 0;

if (completed) {
completed = 0;
return 0;
}

tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);
if (tsk == NULL) {
printk( KERN_INFO "Wrong pid %ld\n", l_pid);
return 0;
}
completed = 1;
char* command = tsk->comm;
pid_t pid = tsk->pid;
long state = tsk->state;

rv = sprintf(buffer, "command = %s pid = %d state = %ld\n", command, pid, state);

// copies the contents of buffer to userspace usr_buf
if (copy_to_user(usr_buf, buffer, rv)) {
printk( KERN_INFO "Error copying to user\n");
    rv = -1;
}

return rv;
}

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos){
    int rv = 0;
    char *k_mem;
    /* allocate kernel memory */
    k_mem = kmalloc(count, GFP_KERNEL);
    /* copies user space usr buf to kernel memory */
    if (copy_from_user(k_mem, usr_buf, count)) {
    printk( KERN_INFO "Error copying from user\n");
    return -1;
    }
    sscanf(k_mem, "%ld", &l_pid);

    /* return kernel memory */
    kfree(k_mem);
    return count;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");
