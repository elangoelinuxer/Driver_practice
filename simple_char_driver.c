#include<linux/init.h>
#include<linux/module.h>


static int ela_init(void)
{

printk("ELA:  In init function ... \n");

return 0;
}

static int ela_exit(void)
{

printk("ELA:  In exit function ... \n");

return 0;
}


module_init(ela_init);
module_exit(ela_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elango");
MODULE_DESCRIPTION("Tiny character driver");

