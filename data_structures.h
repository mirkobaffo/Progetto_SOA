//
// Created by mirkobaffo on 01/10/21.
//

#define LEVELS 32
#define MAX_TAG_NUMBER 256
#define MSG_MAX_SIZE 4096


#ifndef PROGETTO_SOA_DATA_STRUCTURES_H
#define PROGETTO_SOA_DATA_STRUCTURES_H



struct tag{
    int exist;
    int key;
    int command;
    int permission;
    int private;
    int tag_id;
    int opened;
    struct level *structlevels;
};


struct level{
    char bufs[MSG_MAX_SIZE];
    //descrittore univoco con il livello specifico
    int lvl;
    //descrittore univoco con il TAG proprietario del livello
    int tag;
    //se è vuoto rimane in attesa fino all'arrivo del segnale o se si riempie;
    int is_empty;
    int is_queued;
    wait_queue_head_t wq;
    int reader;
};

#endif //PROGETTO_SOA_DATA_STRUCTURES_H
