//
// Created by mirkobaffo on 10/10/21.
//

#include <linux/module.h>
#include <linux/kernel.h>
#include "data_structures.h"

//possibilità di fare una lista collegata per ora accantonata
struct tag TAG_list[MAX_TAG_NUMBER];

int tag_get(int key, int command, int permission){
    int i;
    //devo implementare la parte che riguarda la key IPC_PRIVATE
    //if(key == IPC_PRIVATE){
      //  my_tag.private = 0;
    //}
    if(command == 1) {
        //in questo caso devo creare un nuovo tag
        struct tag my_tag;
        if(permission == 0){
            tag.permission = 0;
        }
        else {
            //da vedere se questo codice che inserisce l'id del thread funziona
            pid_t tid = syscall(SYS_gettid);
            tag.permission((int) tid);
        }
        for (i = 0; i < MAX_TAG_NUMBER; i++) {
            //devo cercare di capire se questo codice puo essere scritto cosi o serve necessariamente una kmalloc
            if (TAG_list[i] == NULL) {
                //probabilmente qua ci devo mettere un semaforo
                my_tag.tag_id = i;
                TAG_list[i] = my_tag;
                printk("Hai creato un nuovo tag nella posizione %d", i);
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
}

int tag_send(int tag, int level, char *buffer, size_t size){
    if(tag < 0 || tag > MAX_TAG_NUMBER){
        printk("purtroppo il valore del tag %d inserito non è valido \n", tag);
        return -1;
    }
    //la prima cosa da fare è vedere se il tag può essere aperto da questo thread
    //per ora non credo dovremmo occuparci della gestione dei risvegli del thread
    pid_t tid = syscall(SYS_gettid);
    if(tag.permission == 0 || (int) tid == tag.permission){

    }
    else{
        printk("non puoi utilizzare questo tag, solo il thread proprietario puo %d", tag);
        return -1;
    }
    //dove dobbiamo appoggiare questo buffer?
    //probabilmente va inserito nel livello, come capire quale livello?
}
