#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int command;
    printf("Scegli quale servizio vuoi utilizzare: \n \n \n");
    scanf("%d",&command);
    //CHIEDERE ALL'UTENTE DI METTERE UNA CHIAVE DA 0 A 255, CON 8 COME IPC PRIVATE
    printf("questa è la tua scelta: %d \n", command);
    char *s = malloc(256);
    if(s == NULL){
        fprintf(stderr, "errore nella malloc\n");
        return 1;
    }
    *s = "questo è il messaggio che voglio mandare";
    syscall(command,134,136);
    return 0;


}
