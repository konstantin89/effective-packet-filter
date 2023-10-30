#ifndef __ASSERT_H
#define __ASSERT_H

#define ASSERT(expression) \
do{\
    if (!(expression)) printk(KERN_ALERT "Assertion failed [" #expression "] \n"); \
}while(0); 

#endif