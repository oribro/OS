//
// Created by OriB on 30/05/2019.
//

#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init_task.h>

#define BUFFER_SIZE 128
#define PROC_NAME "tasks_list"

void get_task_details(struct task_struct* tsk, char buf[]);
void linear_print(struct task_struct* tsk, char buf[]);
void dfs_print(struct task_struct* tsk, char buf[]);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE
};

static int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    char kernel_buf[BUFFER_SIZE];

    printk(KERN_INFO "S   PID   PPID   CMD");
    struct task_struct *task = NULL;
    //linear_print(task, kernel_buf);
    dfs_print(task, kernel_buf);

    return 0;
}

void get_task_details(struct task_struct* tsk, char buf[]) {
    char* command = tsk->comm;
    pid_t pid = tsk->pid;
    long state = tsk->state;
    pid_t ppid = (tsk->real_parent)->pid;

    sprintf(buf, "%ld      %d      %d      %s", state, pid, ppid, command);
}

void linear_print(struct task_struct* tsk, char buf[]){
    for_each_process(tsk) {
        get_task_details(tsk, buf);
        printk(KERN_INFO "%s", buf);
    }
}

void dfs_print(struct task_struct* tsk, char buf[]) {
    struct list_head *list = NULL;
    list_for_each(list, &(init_task.children)) {
        get_task_details(tsk, buf);
        printk(KERN_INFO "%s", buf);
        tsk = list_entry(list, struct task_struct, sibling);
    }
}

static void proc_exit(void)
{

    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}




module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");