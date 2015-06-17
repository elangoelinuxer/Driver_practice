#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/sched.h>
#include<linux/uaccess.h>
#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kthread.h>
#include<linux/delay.h>

dev_t dev;
struct cdev my_char_driver;
struct class *class_var;
struct task_struct *thrd;

static int ela_open(struct inode *i, struct  file *f)
{

	printk("opened the device file \n");

	return 0;

}

static int ela_read(struct file *f, char __user *ubuff, size_t len, loff_t *off)
{

	printk("reading the device file \n");
        printk("starting the thread ....\n");

        wake_up_process(thrd);          // while reading the device node , thread will start executing

	return 0;

}

static int  ela_write(struct file *f,const char __user *ubuff  ,size_t len, loff_t *off)
{

	printk("writing the device file \n");
        printk("stoping the thread ....\n");
      
   //   kthread_stop(thrd);

	return len;

}

static int ela_release(struct inode *i, struct  file *f)
{

	printk("closing the device file  \n");

	return 0;

}


static const struct file_operations fopz=
{
	.open=ela_open,
	.write=ela_write,
	.read=ela_read,
	.release=ela_release,

};



void my_thread(int data)
{

int n=0;




while(!kthread_should_stop())  // once stsrted this will execute untill  this kthread_should stop reurns 1 ie) while kthread_stop is executed
{


printk("In thread function ....%d\n",n);
n++;

msleep(400);


}


//do_exit(1);


}

static int ela_init(void)
{

	printk("ELA:  In init function ... \n");

	int ret_val;

	int data=77;

	ret_val=alloc_chrdev_region(&dev,7,3,"Char_dvr");

	if(ret_val<0)
	{

		printk("error in allocating character driver..\n");
	}

	cdev_init(&my_char_driver,&fopz);
	cdev_add(&my_char_driver,dev,3);

	class_var=class_create(THIS_MODULE,"Char_dvr");

	device_create(class_var,NULL,dev,NULL,"Char_dvr");

        thrd=kthread_create(&my_thread,data,"Elango_thread");        //thread will be created but it wont be executed (note: use thread_run for                                                                         both creating and run in single call)

	return 0;
}

static int ela_exit(void)
{

	printk("ELA:  In exit function ... \n");

         kthread_stop(thrd);

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

