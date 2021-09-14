#include <linux/kernel.h>
#include <linux/module.h>


///< The license type
MODULE_LICENSE("GPL");

///< The author
MODULE_AUTHOR("Mirkobaffo");


int init_module()
{
    printk(KERN_INFO "Caricamento del modulo...\n");

    return 0;
}

void cleanup_module()
{
    printk(KERN_INFO "Scaricamento del modulo...\n");
    return;
}
