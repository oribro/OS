//
// Created by OriB on 03/06/2019.
//

#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include<linux/slab.h>
#include <linux/list.h>

typedef struct color {
    char name[20];
    int red;
    int green;
    int blue;
    struct list_head list;
}Color;

#define PROC_NAME "kernel_dast"

void create_color(Color* color, char name[], int red, int green, int blue);
void print_color(char name[], char buf[]);

static struct list_head color_list;
static LIST_HEAD(color_list);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE
};

static int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    Color* colors[4];
    int i;
    for (i = 0; i < 4; i++)
        colors[i] = kmalloc(sizeof(Color), GFP_KERNEL);

    create_color(colors[0], "gold", 255, 215, 0);
    create_color(colors[1], "lime", 0, 255, 0);
    create_color(colors[2], "olive", 128, 128, 0);
    create_color(colors[3], "aqua", 0, 255, 255);

    for (i = 0; i < 4; i++)
        list_add_tail(&colors[i]->list, &color_list);

    Color* ptr;
    char buf[128];

    list_for_each_entry(ptr, &color_list, list) {
        print_color(ptr->name, buf);
    }

    return 0;
}

void create_color(Color* color, char name[], int red, int green, int blue) {
    sprintf(color->name, "%s", name);
    color->red = red;
    color->green = green;
    color->blue = blue;
    INIT_LIST_HEAD(&color->list);
}

void print_color(char name[], char buf[]) {
    sprintf(buf,"%s", name);
    printk(KERN_INFO "%s", buf);
}

static void proc_exit(void)
{
    Color* ptr, *next;
    list_for_each_entry_safe(ptr, next, &color_list, list) {
        printk(KERN_INFO "Removing %s", ptr->name);
        list_del(&ptr->list);
        kfree(ptr);
    }
    Color* itr;
    char buf[128];
    list_for_each_entry(itr, &color_list, list) {
        print_color(itr->name, buf);
    }
    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}


module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");
