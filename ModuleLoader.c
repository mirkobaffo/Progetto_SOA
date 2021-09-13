//
// Created by mirkobaffo on 13/09/21.
//
#include <linux/module.h>     /* Needed by all modules */
#include <linux/kernel.h>     /* Needed for KERN_INFO */
#include <linux/init.h>


///< The license type
MODULE_LICENSE("GPL");

///< The author
MODULE_AUTHOR("Mirkobaffo");

///< The description
MODULE_DESCRIPTION("A simple Hello world LKM!");

///< The version of the module
MODULE_VERSION("1.0");

    static int __init hello_start(void){
        printk(KERN_INFO, "sto montando il mio primo modulo, ciao!/n");
        return 0;
    }

    static void __exit hello_end(void){
        printk(KERN_INFO, "ciao ciao, io smonto");
    }
    module_init(hello_start);
    module_exit(hello_end);

