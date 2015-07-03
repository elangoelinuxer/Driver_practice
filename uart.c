#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/sched.h>
#include<linux/uaccess.h>
#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/device.h>

#include<linux/ioport.h>
#include<asm/io.h>
#include<linux/interrupt.h>



#define PORT 0x3F8
#define ier 0x03f9
#define iir 0x03fa
#define lcr 0x03fb
#define mcr 0x03fc
#define lsr 0x03fd
#define msr 0x03fe
#define scr 0x03ff

char kbuf[20];
char msg;
int major,i;
unsigned int dt;
unsigned long x;


dev_t dev;
struct cdev my_char_driver;


static int ela_open(struct inode *i, struct  file *f)
{

	printk("opened the device file \n");

	return 0;

}

static int ela_read(struct file *f, char __user *ubuff, size_t len, loff_t *off)
{

	printk("reading the device file \n");

	return 0;

}

static int ela_write(struct file *f,  size_t len, loff_t *off)
{

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





irqreturn_t serial_isr(int irq,void *dev_id,struct pt_regs *reg)
{
        unsigned val,check;
        val = inb(iir);
        check=(val & 0x0F);
        if(check==0x01)
                printk("no interrupt pending\n");
        if(check==0x04)
        {
        msg=inb(PORT);
        printk("%c",msg);
        }
        return IRQ_HANDLED;
}


static int ela_init(void)
{

	int ret_val,a;

	ret_val=alloc_chrdev_region(&dev,7,3,"uart1");

	if(ret_val<0)
	{
		printk("error in allocating character driver..\n");
	}

	cdev_init(&my_char_driver,&fopz);
	cdev_add(&my_char_driver,dev,3);

	class_var=class_create(THIS_MODULE,"uart1");
	device_create(class_var,NULL,dev,NULL,"uart1");

///----------

       request_region(PORT,8,"serial port");
       a=request_irq(4,serial_isr,IRQF_SHARED,"kb",&x);       


        outb(0x80,lcr);
        outb(0x0C,PORT);
        outb(0x03,lcr);
        outb(0x00,iir);
        outb(0x0b,mcr);
        outb(0x01,ier);


	printk("ELA: In init function ... \n");

	return 0;
}

static int ela_exit(void)
{
	printk("ELA:  In exit function ... \n");


        outb(0x00,ier);
        outb(0x00,mcr);
        free_irq(4,&x);
        release_region(PORT,8);
//----------------

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

