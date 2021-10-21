#define EXPORT_SYMTAB
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/kprobes.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
#include <asm/apic.h>
#include <linux/syscalls.h>
#include "./include/vtpmo.h"
#include "./lib/vtpmo.c"
#include "services.c"
#include "data_structures.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mirko Leandri");
MODULE_DESCRIPTION("syscall_filler");



#define MODNAME "syscall_filler"


extern int sys_vtpmo(unsigned long vaddr);


#define ADDRESS_MASK 0xfffffffffffff000//to migrate
#define REQUIRED_SYS_NI_SYSCALL  4 //numero di spazi necessari per allocare syscall di cui necessitiamo
#define START 			0xffffffff00000000ULL		// use this as starting address --> this is a biased search since does not start from 0xffff000000000000
#define MAX_ADDR		0xfffffffffff00000ULL
#define FIRST_NI_SYSCALL	134
#define SECOND_NI_SYSCALL	174
#define THIRD_NI_SYSCALL	182 
#define FOURTH_NI_SYSCALL	183
#define FIFTH_NI_SYSCALL	214	
#define SIXTH_NI_SYSCALL	215	
#define SEVENTH_NI_SYSCALL	236	

#define ENTRIES_TO_EXPLORE 256


unsigned long *hacked_ni_syscall=NULL;
unsigned long **hacked_syscall_tbl=NULL;

//in questa variabile inserisco gli indirizzi delle sys_ni_syscall trovate nella tabella delle syscall
unsigned long *ni_syscall_founded = NULL;

unsigned long sys_call_table_address = 0x0;
module_param(sys_call_table_address, ulong, 0660);

unsigned long sys_ni_syscall_address = 0x0;
module_param(sys_ni_syscall_address, ulong, 0660);



//verifica semplicemente se la posizione i-esima della tabella è quella relativa alla nostra First-ni-Syscall
int good_area(unsigned long * addr){

	int i;
	
	for(i=1;i<FIRST_NI_SYSCALL;i++){
		if(addr[i] == addr[FIRST_NI_SYSCALL]) goto bad_area;
	}	

	return 1;

bad_area:

	return 0;

}



/* This routine checks if the page contains the begin of the syscall_table.  */
int validate_page(unsigned long *addr){
	int i = 0;
	unsigned long page 	= (unsigned long) addr;
	unsigned long new_page 	= (unsigned long) addr;
	for(; i < PAGE_SIZE; i+=sizeof(void*)){		
		new_page = page+i+SEVENTH_NI_SYSCALL*sizeof(void*);
			
		// If the table occupies 2 pages check if the second one is materialized in a frame
		if( 
			( (page+PAGE_SIZE) == (new_page & ADDRESS_MASK) )
			&& sys_vtpmo(new_page) == NO_MAP
		) 
			break;
		// go for patter matching
		addr = (unsigned long*) (page+i);
		if(
			   ( (addr[FIRST_NI_SYSCALL] & 0x3  ) == 0 )		
			   && (addr[FIRST_NI_SYSCALL] != 0x0 )			// not points to 0x0	
			   && (addr[FIRST_NI_SYSCALL] > 0xffffffff00000000 )	// not points to a locatio lower than 0xffffffff00000000	
	//&& ( (addr[FIRST_NI_SYSCALL] & START) == START ) 	
			&&   ( addr[FIRST_NI_SYSCALL] == addr[SECOND_NI_SYSCALL] )
			&&   ( addr[FIRST_NI_SYSCALL] == addr[THIRD_NI_SYSCALL]	 )	
			&&   ( addr[FIRST_NI_SYSCALL] == addr[FOURTH_NI_SYSCALL] )
			&&   ( addr[FIRST_NI_SYSCALL] == addr[FIFTH_NI_SYSCALL] )	
			&&   ( addr[FIRST_NI_SYSCALL] == addr[SIXTH_NI_SYSCALL] )
			&&   ( addr[FIRST_NI_SYSCALL] == addr[SEVENTH_NI_SYSCALL] )	
			&&   (good_area(addr))
		){
			hacked_ni_syscall = (void*)(addr[FIRST_NI_SYSCALL]);				// save ni_syscall
			sys_ni_syscall_address = (unsigned long)hacked_ni_syscall;
			hacked_syscall_tbl = (void*)(addr);				// save syscall_table address
			sys_call_table_address = (unsigned long) hacked_syscall_tbl;
			return 1;
		}
	}
	return 0;
}

/* This routines looks for the syscall table.  */
void syscall_table_finder(void){
	unsigned long k; // current page
	unsigned long candidate; // current page

	for(k=START; k < MAX_ADDR; k+=4096){	
		candidate = k;
		if(
			(sys_vtpmo(candidate) != NO_MAP) 	
		){
			// check if candidate maintains the syscall_table
			if(validate_page( (unsigned long *)(candidate)) ){
				printk("%s: syscall table found at %px\n",MODNAME,(void*)(hacked_syscall_tbl));
				printk("%s: sys_ni_syscall found at %px\n",MODNAME,(void*)(hacked_ni_syscall));
				break;
			}
		}
	}
	
}

//vado a settare il numero della syscall table con ni_sys_call all'interno della prima entry libera del nostro array
int fill_ni_syscall_founded(int i, int c){
    int *temp;
    int k;
	if(ni_syscall_founded[c] == NULL){
		printk("inserisco nel posto %d dell'array la posizione sys_ni_syscall %d ", c, i);
		temp = i;
		ni_syscall_founded[c] = (unsigned long*)temp;
        for(k = 0; k < c; k ++){
            printk("ecco le entry: %lu", ni_syscall_founded[k]);
        }
    }
	else{
		printk("lo spazio %d è pieno, passo al successivo", c);
        //questa funzione ricorsiva in ambito di sicurezza non va bene quindi va tolta (anche no)
		fill_ni_syscall_founded(i,c+1);
	}
    return 0;

}



#define MAX_FREE 15
int free_entries[MAX_FREE];
module_param_array(free_entries,int,NULL,0660);//default array size already known - here we expose what entries are free

int syscall_number_finder(void){
	int i,j,counter;
    counter = 0;
	syscall_table_finder();
	if(!hacked_syscall_tbl){
		printk("%s: failed to find the sys_call_table\n",MODNAME);
		return -1;
	}

	j=0;
	for(i=0;i<ENTRIES_TO_EXPLORE;i++)
		if(hacked_syscall_tbl[i] == hacked_ni_syscall){
			printk("%s: found sys_ni_syscall entry at syscall_table[%d]\n",MODNAME,i);	
			free_entries[j++] = i;
            if(counter == REQUIRED_SYS_NI_SYSCALL){
                printk("la tabella è piena\n");
                return 0;
            }
            fill_ni_syscall_founded(i,0);
            counter = counter +1;
			if(j>=MAX_FREE) break;
		}
		return 0;

}

//Qui sto inserendo le mie system call nuove, il cui frontend verrà implementato in un altro file
#define SYS_CALL_INSTALL

#ifdef SYS_CALL_INSTALL
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0)
__SYSCALL_DEFINEx(3, _tag_get, int, key, int, command, int, permission){
#else
asmlinkage int sys_tag_get(int key, int command, int permission){
#endif
     return tag_get(key,command,permission);

}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0)
__SYSCALL_DEFINEx(4, _tag_send, int, tag, int, level, char *, buffer, size_t, size ){
#else
asmlinkage int sys_tag_send(int tag, int level, char * buffer, size_t size){
#endif
    return tag_send(tag,level,buffer, size);

}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0)
__SYSCALL_DEFINEx(4, _tag_receive, int, tag, int, level, char *, buffer, size_t, size ){
#else
asmlinkage int sys_tag_receive(int tag, int level, char * buffer, size_t size){
#endif
    return tag_receive(tag,level,buffer,size);

}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0)
__SYSCALL_DEFINEx(2, _tag_ctl, int, tag, int, command){
#else
asmlinkage int sys_tag_ctl(int tag, int command){
#endif
    return tag_ctl(tag,command);
}


unsigned long cr0;

static inline void
write_cr0_forced(unsigned long val)
{
    unsigned long __force_order;

    /* __asm__ __volatile__( */
    asm volatile(
        "mov %0, %%cr0"
        : "+r"(val), "+m"(__force_order));
}

static inline void
protect_memory(void)
{
    write_cr0_forced(cr0);
}

static inline void
unprotect_memory(void)
{
    write_cr0_forced(cr0 & ~X86_CR0_WP);
}

#else
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0)
static unsigned long sys_tag_get = (unsigned long) __x64_sys_tag_get;
static unsigned long sys_tag_send = (unsigned long) __x64_sys_tag_send;
static unsigned long sys_tag_receive = (unsigned long) __x64_sys_tag_receive;
static unsigned long sys_tag_ctl = (unsigned long) __x64_sys_tag_ctl;
#else
#endif


int init_module(void) {
    printk("%s: initializing\n",MODNAME);
    ni_syscall_founded = kmalloc(sizeof(unsigned long) * REQUIRED_SYS_NI_SYSCALL, GFP_KERNEL);
    syscall_number_finder();

#ifdef SYS_CALL_INSTALL
	cr0 = read_cr0();
        unprotect_memory();
        hacked_syscall_tbl[FIRST_NI_SYSCALL] = (unsigned long*)sys_tag_get;
        hacked_syscall_tbl[SECOND_NI_SYSCALL] = (unsigned long*)sys_tag_send;
        hacked_syscall_tbl[THIRD_NI_SYSCALL] = (unsigned long*)sys_tag_receive;
        hacked_syscall_tbl[FOURTH_NI_SYSCALL] = (unsigned long*)sys_tag_ctl;


    protect_memory();
	printk("%s: Ho inserito le mie 4 nuove syscall nelle entry della sys call table: %d, %d, %d, %d \n",MODNAME,FIRST_NI_SYSCALL, SECOND_NI_SYSCALL, THIRD_NI_SYSCALL, FOURTH_NI_SYSCALL);
#else
#endif

        printk("%s: module correctly mounted\n",MODNAME);

        return 0;

}
//Quando smonto il modulo mi assicuro di rimettere nella loro posizione le entry corrispondenti a ni_syscall per avere la possibilità di rieseguire il tutto una seconda volta senza problemi e senza dover riavviare la macchina virtuale
void cleanup_module(void) {
                
#ifdef SYS_CALL_INSTALL
	    cr0 = read_cr0();
        unprotect_memory();
        //qui libero l'area di memoria che mi riempie le dinamicamente questo array con le entry ni_syscall che per ora resta inutilizzata
        kfree(ni_syscall_founded);
        kfree(TAG_list);
        kfree(level_list);
        hacked_syscall_tbl[FIRST_NI_SYSCALL] = (unsigned long*)hacked_ni_syscall;
        printk("resetto la entry della syscall table numero %d", FIRST_NI_SYSCALL);
        hacked_syscall_tbl[SECOND_NI_SYSCALL] = (unsigned long*)hacked_ni_syscall;
        printk("resetto la entry della syscall table numero %d", SECOND_NI_SYSCALL);
        hacked_syscall_tbl[THIRD_NI_SYSCALL] = (unsigned long*)hacked_ni_syscall;
        printk("resetto la entry della syscall table numero %d", THIRD_NI_SYSCALL);
        hacked_syscall_tbl[FOURTH_NI_SYSCALL] = (unsigned long*)hacked_ni_syscall;
        printk("resetto la entry della syscall table numero %d", FOURTH_NI_SYSCALL);
        protect_memory();

#else
#endif
        printk("%s: shutting down\n",MODNAME);
        
}
