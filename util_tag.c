//
// Created by Mirko Leandri on 18/10/21.
//


#include "data_structures.h"

int search_for_level(struct tag TAG_list){
    int i;
    int j;
    for(j = 0; j < LEVELS; j ++) {
        if (TAG_list.structlevels[j].is_empty){
            continue;
        }
        else{
            printk("un livello contiene un messaggio\n");
            return -1;
        }
    }
}
    return 0;
};