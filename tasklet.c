#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/sched.h>
#include<linux/uaccess.h>
#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/device.h>

#include<linux/interrupt.h>

dev_t dev;
struct cdev my_char_driver;


char my_data[]="tasklet was called ...";


void tasklet_func(unsigned long data)
{

printk("IN tasket function ...%s",(char *)data);


}

DECLARE_TASKLET(ela_tasklet,tasklet_func,(unsigned long)&my_data);



static int ela_open(struct inode *i, struct  file *f)
{

printk("opened the device file \n");

return 0;

}

static int ela_read(struct file *f, char __user *ubuff, size_t len, loff_t *off)
{

//tasklet_enable(&ela_tasklet);


printk("reading the device file \n");

return 0;

}

static int ela_write(struct file *f,  size_t len, loff_t *off)
{


//tasklet_disable(&ela_tasklet);

printk("writing the device file \n");

return 0;

}

static int ela_release(struct inode *i, struct  file *f)
{

printk("closing the device file  \n");

return 0;

}


static struct file_operations fopz=
{
.open=ela_open,
.write=ela_write,
.read=ela_read,
.release=ela_release,

};

struct class *class_var;


static int ela_init(void)
{

int ret_val;

tasklet_schedule(&ela_tasklet);


ret_val=alloc_chrdev_region(&dev,7,3,"Char_dvr");

if(ret_val<0)
{

printk("error in allocating character driver..\n");
}

cdev_init(&my_char_driver,&fopz);
cdev_add(&my_char_driver,dev,3);

class_var=class_create(THIS_MODULE,"Char_dvr");

device_create(class_var,NULL,dev,NULL,"Char_dvr");


printk("ELA:  In init function ... \n");

return 0;
}

static int ela_exit(void)
{

printk("ELA:  In exit function ... \n");

tasklet_kill(&ela_tasklet);


cdev_del(&my_char_driver);

device_destroy(class_var,dev);
class_destroy(class_var);

unregister_chrdev_region(dev,3);

return 0;
}


module_init(ela_init);
module_exit(ela_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elango");
MODULE_DESCRIPTION("Tiny character driver");

