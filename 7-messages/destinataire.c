#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include <errno.h>

typedef struct {
    long type;
    char message[30];
} MSG;

int main(int argc, char *argv[]){

    if(argc!=2){
        printf("Il faut 1 seul arguments\n");
        return 1;
    }

    long type = atol(argv[1]);
    if(type < 0){
        printf("Le premier paramètre doit être un entier strictement positif\n");
        return 1;
    }

    printf("Arguments valides\n");

    //--------------RECUPERATION CLE--------------------

    key_t cle = ftok("cle.txt",5);

    if (cle == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);

        printf("Cle recuperee\n");
    }

    //---------RECUPERATION FILE DE MESSAGES-------------

    int msgid = msgget(cle, IPC_EXCL | 0666);
    if (msgid == -1){
        perror("Erreur lors de msgget, la file n'existe probablement pas.");
        return -1;
    }

    printf("File de messages recuperée\n");

    //---------INFORMATIONS FILE DE MESSAGES-------------

    struct msqid_ds info;
    if(msgctl(msgid, IPC_STAT, &info) == -1){
        perror("msgctl");
        return -1;
    }

    int nb_messages = info.msg_qnum;

    printf("\n-------INFORMATIONS--------\n");
    printf("File de messages:\nClé: %d;\nmsgid: %d;\n", cle, msgid);
    printf("Nb messages dans la file: %ld\n", info.msg_qnum);
    printf("---------------------------\n\n");

    //-------------------LECTURE MESSAGE------------------

    if(nb_messages > 0){
        MSG msg;    

        if(msgrcv(msgid, &msg, 30, type, IPC_NOWAIT) == -1){
            perror("msgrcv");
            return -1;
        }
        
        printf("Message reçu : \n  Type: %ld;\n  Message: %s;\n\n", msg.type, msg.message);
        printf("-------FIN PROGRAMME-------\n\n");
    }
    
}