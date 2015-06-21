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
#include<linux/semaphore.h>
#include<linux/mutex.h>


dev_t dev;
struct cdev my_char_driver;
struct class *class_var;
struct task_struct *thrd1,*thrd2;
struct semaphore sem_mine;
static DEFINE_MUTEX(ela_mutex);

struct file *fop;

static int ela_open(struct inode *i, struct  file *f)
{

	printk("opened the device file \n");
	return 0;

}

static int ela_read(struct file *f, char __user *ubuff, size_t len, loff_t *off)
{

	printk("Reading the device file \n");

	printk("starting the threads....\n");
	wake_up_process(thrd1);                // while reading the device node , thread will start executing
	wake_up_process(thrd2);                // while reading the device node , thread will start executing

	return 0;

}

static int  ela_write(struct file *f,const char __user *ubuff  ,size_t len, loff_t *off)
{

	int val=2;


	while(val>0)
	{


		printk("writing the device file %c\n",*ubuff);
		--val;
		msleep(600);


	}


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


int func(char data)
{


//	down_interruptible(&sem_mine);   //locking the shared resource 
mutex_trylock(&ela_mutex);
	fop=filp_open("/home/elango/ela.txt",O_WRONLY|O_CREAT|O_APPEND,0777);

	if(fop<0)
	{
		printk("file cannot opened ....\n");
	}

       if(data>64 && data <91)
       {
	printk("in critcal section of the code ---> thread_1 writing charcter --> %c  & thread_2 waiting \n\n",data);
       }
       else
       { 
	printk("in critcal section of the code ---> thread_2 writing number --> %c  & thread_1 waiting \n\n",data);
       }

	fop->f_op->write(fop,(char *)&data,1,&fop->f_pos);  //writing to the file
	msleep(2000);

	filp_close(fop,0);
mutex_unlock(&ela_mutex);
//	up(&sem_mine);        //releasing the shared resource


	return 0;

}

int my_thread_1(int data)
{

	int n=0;
	char ac=65;

	while(!kthread_should_stop())  // once stsrted this will execute untill  this kthread_should stop reurns 1 ie) while kthread_stop is executed
	{


		if(ac<91)
		{ 
			func(ac);       
			ac++; 
		}
		else
		{
			ac=65;
		}


	}


	do_exit(1);  //exiting the thread properly


}


int my_thread_2(int data)
{

	int n=0;


	char ac=48;


	while(!kthread_should_stop())  // once stsrted this will execute untill  this kthread_should stop reurns 1 ie) while kthread_stop is executed
	{

		if(ac<58)
		{ 
			func(ac);       
			ac++; 
		}
		else
		{
			ac=48;
		}

	}



	do_exit(1);


}





static int ela_init(void)
{

	printk("ELA:  In init function ... \n");

	int ret_val;

	int data=77;


//	sema_init(&sem_mine,1);


	ret_val=alloc_chrdev_region(&dev,7,3,"Char_dvr");


	if(ret_val<0)
	{

		printk("error in allocating character driver..\n");
	}


	cdev_init(&my_char_driver,&fopz);
	cdev_add(&my_char_driver,dev,3);

	class_var=class_create(THIS_MODULE,"Char_dvr");

	device_create(class_var,NULL,dev,NULL,"Char_dvr");

	thrd1=kthread_create(&my_thread_1,data,"Elango_thread_1");        //thread will be created but it wont be executed (note: use thread_run for                                                                         both creating and run in single call)
	thrd2=kthread_create(&my_thread_2,data,"Elango_thread_2");        //thread will be created but it wont be executed (note: use thread_run for                                                                         both creating and run in single call)

	return 0;
}

static int ela_exit(void)
{


	printk("ELA:  In exit function ... \n");

	kthread_stop(thrd1);   //stoping the running thread1

	kthread_stop(thrd2);   //stoping the running thread2

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

