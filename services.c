//
// Created by mirkobaffo on 10/10/21.
//

#include <linux/module.h>
#include <linux/kernel.h>
#include "data_structures.h"
#include <linux/unistd.h>
#include <linux/spinlock.h>


//possibilità di fare una lista collegata per ora accantonata
struct tag *TAG_list = NULL;
struct level *level_list = NULL;
spinlock_t lock;

int tag_get(int key, int command, int permission){
    TAG_list = kmalloc(sizeof(struct tag)*MAX_TAG_NUMBER,GFP_KERNEL);
    if(TAG_list == NULL){
        printk("errore nell'assegnazione dell'area di memoria dedicata ai tag");
        return -1;
    }

    int i;
    //devo implementare la parte che riguarda la key IPC_PRIVATE
    //if(key == IPC_PRIVATE){
      //  my_tag.private = 0;
    //}
    if(command == 1) {
        //in questo caso devo creare un nuovo tag
        struct tag my_tag;
        my_tag.exist = 1;
        if(permission == 0){
            my_tag.permission = 0;
        }
        else {
            //il thread che crea il tag dovrà inserire il proprio tid nel tag.
        }
        for (i = 0; i < MAX_TAG_NUMBER; i++) {
            //controllare se ci restituisce null pointer
            if (TAG_list[i].exist != 1) {
                //probabilmente qua ci devo mettere un semaforo
                my_tag.tag_id = i;
                TAG_list[i] = my_tag;
                printk("Hai creato un nuovo tag nella posizione %d", i);
                //ora creo la serie di livelli associati al tag di riferimento
                level_list = kmalloc(sizeof(struct level) * LEVELS, GFP_KERNEL);
                if (level_list == NULL) {
                    printk("errore nell'assegnazione dell'area di memoria dedicata ai livelli per il tag: %d", i);
                    return -1;
                }
                TAG_list[i].structlevels = &level_list;
                int lvl;
                //adesso assegno ad ogni livello il tag e il proprio livello appunto
                for(lvl = 0; lvl< LEVELS; lvl ++){
                    level_list[lvl].tag = i;
                    level_list[lvl].lvl=lvl;
                }
                }
                //il valore che rappresenta il tag nell'array sarà il descrittore univoco del tag
                return i;
            }
            if (i == MAX_TAG_NUMBER) {
                printk("hai raggiunto il limite di tag che è possibile creare");
                return -1;
            }
        }
    }

    else if (command == 2){
        //in questo caso devo aprire un tag esistente, che vuol dire di preciso aprire un tag esistente?
        for (i = 0; i < MAX_TAG_NUMBER; i++) {

        }
    }
    //da verificare se va proprio qua
    return i;
}

int tag_send(int tag, int level, char *buffer, size_t size){
    if(tag < 0 || tag > MAX_TAG_NUMBER){
        printk("purtroppo il valore del tag %d inserito non è valido \n", tag);
        return -1;
    }
    //la prima cosa da fare è vedere se il tag può essere aperto da questo thread
    //per ora non credo dovremmo occuparci della gestione dei risvegli del thread
    //questo controllo lo dovrà fare il thread a livello user
   /* if(tag.permission == 0 || (int) tid == tag.permission){

    }
    else{
        printk("non puoi utilizzare questo tag, solo il thread proprietario puo %d", tag);
        return -1;
    } */
    spin_lock(&lock);
    //controllo se c'è almeno un thread in attesa altrimenti butto il messaggio
    //QUESTA FUNZIONE VA SCRITTA (IN UN ALTRO FILE MAGARI)
    if(search_wait_thread()) {
        copy_from_user(TAG_list[tag].structlevels[level].buf, buffer, size);
    }
    spin_unlock(&lock);
    return 0;
}

int tag_receive(int tag, int level, char *buffer, size_t size) {
    //da capire come ipmlementare un thread in attesa di un messaggio
    return 0;
}

int tag_ctl(int tag, int permission) {
    return 0;

}

