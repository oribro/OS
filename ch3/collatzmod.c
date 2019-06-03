//
// Created by OriB on 03/06/2019.
//

//
// Created by OriB on 03/06/2019.
//

#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include<linux/slab.h>
#include <linux/list.h>
#include <linux/moduleparam.h>

#define PROC_NAME "collatz"

typedef struct collatz {
    int val;
    struct list_head list;
}Collatz;

void store_val(int val);

static int start = 1;
module_param(start, int, 0);

static struct list_head collatz_seq;
static LIST_HEAD(collatz_seq);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE
};

static int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    Collatz* col;
    while (start != 1) {
        col = kmalloc(sizeof(Collatz), GFP_KERNEL);
        col->val = start;
        INIT_LIST_HEAD(&col->list);
        list_add(&col->list, &collatz_seq);

        if (start % 2)
            start = start * 3 + 1;
        else
            start = start / 2;
    }

    Collatz* itr;
    list_for_each_entry(itr, &collatz_seq, list) {
        printk(KERN_INFO "%d", itr->val);
    }

    return 0;
}


static void proc_exit(void)
{
    Collatz* ptr, *next;
    list_for_each_entry_safe(ptr, next, &collatz_seq, list) {
        printk(KERN_INFO "Removing %d", ptr->val);
        list_del(&ptr->list);
        kfree(ptr);
    }

    Collatz* itr;
    list_for_each_entry(itr, &collatz_seq, list) {
        printk(KERN_INFO "%d", itr->val);
    }
    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}


module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");
