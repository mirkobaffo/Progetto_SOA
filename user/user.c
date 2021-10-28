#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define AWAKE_ALL 1
#define REMOVE 2
#define CREATE 1
#define OPEN 2
#define NUM_THREADS 5
#define RAND_MAX 32
#define MAJOR 0
#define MINOR 4
#define LEVELS 32
#define MAX_TAG_NUMBER 256
#define MSG_MAX_SIZE 4096


//Frontend della tag_get
int tag_get(int key, int command, int permission){
    printf("interno della tag_get()");
    return syscall(134, key, command, permission);
}

//Frontend della tag_send
int tag_send(int tag, int level, char *buffer, size_t size){
    return syscall(174, tag, level, *buffer, size);
}

//Frontend della tag_receive
int tag_receive(int tag, int level, char *buffer, size_t size){
    return syscall(182, tag, level, *buffer, size);
}

//Frontend della tag_ctl
int tag_ctl(int tag, int command){
    return syscall(183,tag,command);
}

void test_create_open_remove_tag(){
    /*
     * Testa la creazione un numero di tag (1)
     * Testa la logica IPC_PRIVATE (2)
     * Testa la rimozione di un numero di tag (3)
     * Testa l'apertura di un tag rimosso e quindi rimuovilo nuovamente (4)
     */

    int ret;

    // crea n tag (1)
    for (int i=0; i<256; i++){
        ret = tag_get(i, 1, 0);
        printf("Creato correttamente il tag con key: %d.\n", ret);
    }

    // apri il tag IPC_PRIVATE [0] (2)
    ret = tag_get(0, 1, 0);
    if (ret < 0)
        printf("Il tag con key: %d è IPC_PRIVATE.\n", 0);

    // rimuovi n tag (3)
    for (int i=0; i<256; i++){
        ret = tag_ctl(i, REMOVE);
        if (ret == 0)
            printf("Rimosso correttamente il tag con key: %d.\n", i);
    }

    // apri un tag rimosso e successivamente rimuovilo (4)
    ret = tag_get(100, 2, 0);
    printf("Riaperto correttamente il tag con key: %d.\n", ret);
    ret = tag_ctl(100, REMOVE);
    if (ret == 0)
        printf("Rimosso correttamente il tag con key 100.\n");
}

void test_write_read(){
    /*
     * Test di scrittua rifiutata (1)
     * Test di scrittua accettata (2)
     * Test di risveglio dei thread e inviare un messaggio e riceverlo (3)
     */

    int ret;

    // scrivere su tag che non stanno leggendo (1)
    char msg = "Ciao mamma ti voglio bene";
    int lenMsg = strlen(msg);
    for (int i=0; i<5; i++){
        ret = tag_send(i, 3,msg, lenMsg);
        printf("Ho perso correttamente il messaggio sul tag con key: %d; "
               "non c'è nessuno in attesa di riceverlo.\n", msg, ret);
    }

    // scrivere su tag che stanno leggendo (2)
    // alloco lo spazio per il buffer su cui ricevo il messaggio
    char *s_test2 = malloc(256);
    int lenS_test2 = strlen(s_test2);
    if(s_test2 == NULL){
        fprintf(stderr, "Errore nella malloc.\n");
        return 1;
    }
    // creo un tag su cui voglio ricevere il messaggio
    ret = tag_get(1, 1, 0);
    printf("Creato correttamente il tag con key: %d.\n", ret);
    // mi metto in attesa di ricevere il messaggio
    tag_receive(1,1,s_test2,lenS_test2);
    printf("Sono in attesa di ricevere un messaggio sul tag con key: %d.\n", ret);
    // invio il messaggio sul tag aperto
    ret = tag_send(1, 1,msg, lenMsg);
    if (s_test2 == msg)
        printf("Ho inviato correttamente il messaggio sul tag con key: %d.\n", msg, ret);

    // TODO --> rifare il test rendendolo multithread
    // risveglio tutti i tag e invio un messaggio (3)
    // alloco lo spazio per il buffer su cui ricevo il messaggio
   /* char *s_test3 = malloc(256);
    int lenS_test3 = strlen(s_test3);
    if(s_test3 == NULL){
        fprintf(stderr, "Errore nella malloc.\n");
        return 1;
    }
    // addormento il tag 1
    tag_receive(1, 1, s_test3, lenS_test3);
    // risveglio il tag 1
    // TODO --> errore serve un thread che lo risveglia
    ret = tag_ctl(1, AWAKE_ALL);
    printf("Risvegliati tutti i thread, %d.\n", ret);
    // invio il messaggio sul tag risvegliato
    char msg = "Ciao mamma mi sono svegliato";
    int lenMsg = strlen(msg);
    ret = tag_send(1, 1,msg, lenMsg);
    printf("Ho ricevuto correttamente il messaggio sul tag con key: %d.\n", msg, ret);

    if (s_test3 == msg):
    printf("Ho inviato correttamente il messaggio sul tag con key: %d.\n", msg, ret); */
}

void * the_thread(void* path){
    char* device;
    int fd, ret;
    char mess[MSG_MAX_SIZE];
    device = (char*) path;
    fd = open(device, O_RDWR);
    if(fd < 0) {
        printf("Error opening device %s\n",device);
        pthread_exit(EXIT_SUCCESS);
    }
    fprintf(stdout, "device %s aperto\n",device);
    size_t len = MSG_MAX_SIZE;
    ret = read(fd,mess,len);
    if (ret < 0)
        printf("Errore nella read del device driver\n");
    else
        printf(stdout, "Questo è presente nel tag attualmente \n%s\n", mess);
    close(fd);
    pthread_exit(EXIT_SUCCESS);
}

void test_device_driver(){
    char *path = "/dev/driver";
    char buff[MSG_MAX_SIZE];
    int i;
    int ret;
    int tag_array[MINOR];
    pthread_t tid[MINOR];
    for(i = 0; i < MINOR; i ++){
        tag_array[i] = tag_get(i,CREATE,0);
        if(tag_array[i] < 0){
            printf("errore nella tag_get di test_device_driver\n");
            return -1;
        }
    }
    printf("creazione dei thread\n");
    for(i = 0; i< MINOR; i++){
        fflush(buff);

        snprintf(buff,MSG_MAX_SIZE, "mknod %s%d c %d %d\n", path, i, MAJOR, i);
        printf("sto per fare una system\n");
        system(buff);
        fflush(buff);
        sprintf(buff,"%s%d",path, i);
        printf("buff %s\n", buff);
        pthread_create(&tid[i], NULL, the_thread, *buff);
        fflush(buff);
    }
    printf("aspetto che terminino l'esecuzione tutti i thread");
    for(i = 0; i< MINOR; i++){
        pthread_join(&tid[i], NULL);
    }
    printf("rimuovo i tag");
    for(i = 0; i< MINOR; i++){
      ret = tag_ctl(i, REMOVE);
    }
    sprintf(buff,"rm  %s*\n",path);
    system(buff);
    printf("Esecuzione terminata\n");
}

int test_waiting_for_message(int key){
    //genero un numero random da 1 a 32 per il livello
    int level=key;
    char *return_buff;
    return_buff = malloc(MSG_MAX_SIZE);
    sleep(2);
    if(return_buff == NULL){
        printf("errore nella malloc della test_waiting_for_message\n");
        return -1;
    }
    //inserisco queste funzioni per misurare il tempo in cui i thread sono bloccati in attesa di un messaggio
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int ret;
    printf("sto per entrare nella receive, sono il tag: %d\n", key);
    sleep(2);
    ret = tag_receive(key, level, return_buff, MSG_MAX_SIZE);
    if(ret<0){
        printf("errore nella tag receive, ret:%d\n", ret);
        return -1;
    }
    //Quindi quando arriva qui significa che è arrivato un messaggio ed è stato letto
    end = clock();
    cpu_time_used = ((double) (end - start));
    printf("ho ricevuto il messaggio, ma ho dovuto aspettare %f\n", cpu_time_used);
    printf("il messaggio è: %s\n", return_buff);
    //mi faccio tornare il livello per poter inviare un messaggio
    free(return_buff);
    return level;
}


int test_delete_and_open(int tag, int tid){
    //in questa funzione cancelliamo un tag e proviamo a riaprirlo per verificare l'effettiva riuscita dell'azione
    int ret;
    int ctl;
    ret = tag_get(tag,OPEN,tid);
    if(ret < 0){
        printf("il tag che stai cercadno di aprire per il test delete and open non esiste");
        return -1;
    }
    ctl = tag_ctl(ret, REMOVE);
    if(ctl <0){
        printf("non sono riuscito a cancellare il tag %d", ret);
        return -1;
    }
    ret = tag_get(tag,OPEN,tid);
    if(ret < 0){
        printf("il risultato è atteso, per verificare meglio usare il comando seguente:\n sudo dmesg");
        return 0;
    }
    return 0;
}

int test_sending_message(int tag, int level, int tid){
    char *send_buf;
    send_buf = malloc(MSG_MAX_SIZE);
    if(send_buf == NULL){
        printf("errore nella malloc della test_sending_message");
        return -1;
    }
    send_buf = sprintf(send_buf,"messaggio di prova del thread %d al livello %d del tag %d", tid, level, tag);
    int ret;
    printf("ho copiato il messaggio, ora invio\n");
    ret = tag_send(tag,level,send_buf,MSG_MAX_SIZE);
    if(ret < 0){
        printf("errore nella tag_send: %d\n", ret);
        return -1;
    }
    return 0;
}

int test_multithread(void *i){
    pthread_t tid;
    tid = pthread_self();
    int int_tid = (int) tid;
    int ret;
    //apertura del tag
    printf("sto per aprire i tag\n");
    sleep(1);
    ret = tag_get(i, 1, int_tid);
    if(ret < 0){
        printf("errore nella creazione del tag %d", i);
        return -1;
    }
    printf("creato il tag con key = %d\n", ret);
    ret = test_waiting_for_message(ret);
    if(ret< 0){
        printf("errore nella test_waiting_for_message");
        return -1;
    }


}



void test_create_multithread(){
    int i;
    int rc;
    pthread_t threads[NUM_THREADS];
    printf("sto per creare i thread\n");
    for( i = 0; i < NUM_THREADS; i++ ) {
        rc = pthread_create(&threads[i], NULL, test_multithread, (void *)i);
        if (rc) {
            printf("Error:unable to create thread, %d\n", rc);
            return -1;
        }
    }
    int tid = (int)pthread_self();
    printf("sono il thread %d e sto continuando l'esecuzione\n", tid);
    printf("WAITING...\n");
    sleep(8);
    printf("ora invio il messaggio\n");
    test_sending_message(1,1,0);
    printf("inviato\n");
    printf("WAITING...\n\n\n");
    sleep(10);
    //pthread_exit(NULL);
    return 0;
}

int remove_for_test(){
    int i;
    int ret;
    for(i = 0; i < 5; i++){
        ret = tag_ctl(i, REMOVE);
        if(ret < 0){
            printf("errore nella ctl per rimuovere tutti i tag\n");
        }
    }
    return 0;
}
int main(int argc, char *argv[]){
    //printf("Eseguo i test di creazione e rimozione dei tag.\n");
    //test_create_open_remove_tag();
    //printf("Eseguo i test di lettura e scrittura dei tag.\n");
    //test_write_read();
    //remove_for_test();
    //printf("Eseguo il test del device driver.\n");
    //test_device_driver();
    //TODO controllo della lista se vuota
    //remove_for_test();
    printf("Eseguo il test multithread.\n");
    test_create_multithread();
    return 0;
}


