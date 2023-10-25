#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>


MODULE_AUTHOR("konstantin89");
MODULE_DESCRIPTION("");
MODULE_LICENSE("GPL");

static int __init init(void)
{
    int exit_code = 0;

    return exit_code;
}

static void __exit cleanup(void)
{

}

module_init(init);
module_exit(cleanup);
