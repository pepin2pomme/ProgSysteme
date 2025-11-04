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

    //----------VERIFICATIONS PARAMETRES-----------------

    if(argc!=3){
        printf("Il faut 2 arguments\n");
        return 1;
    }
    
    long type = atol(argv[1]);
    if(type < 0){
        printf("Le premier parametre doit etre entier strictement positif\n");
        return 1;
    }

    if(strlen(argv[2]) > 30){
        printf("Longueur max parametre 2 de 30 caractères\n");
        return 1; 
    }

    //--------------RECUPERATION CLE--------------------

    key_t cle = ftok("cle.txt",5);

    if (cle == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    //-----CREATION-RECUPERATION FILE DE MESSAGES--------

    int msgid = msgget(cle, IPC_CREAT | 0666);
    if (msgid == -1){
        msgid = msgget(cle, IPC_EXCL | 0666);
        if (msgid == -1){
            perror("Erreur lors de msgget");
            return -1;
        }
        printf("La file de message existait déjà.\n");
    }

    //---------INFORMATIONS FILE DE MESSAGES-------------

    struct msqid_ds info;
    if(msgctl(msgid, IPC_STAT, &info) == -1){
        perror("msgctl");
        return -1;
    }

    printf("\n-------INFORMATIONS--------\n");
    printf("File de messages:\nClé: %d;\nmsgid: %d;\n", cle, msgid);
    printf("UID: %d; GID: %d; DROITS: %o;\n", info.msg_perm.uid, info.msg_perm.gid, info.msg_perm.mode);
    printf("Nb messages dans la file (avant ajout): %ld\n", info.msg_qnum);
    printf("---------------------------\n\n");

    //----------CREATION-ECRITURE MESSAGE------------------

    MSG msg;
    msg.type = type;
    strncpy(msg.message, argv[2], 30);

    if(msgsnd(msgid, &msg, strlen(msg.message), 0) == -1){
        perror("erreur envoi du message dans: msgsnd");
        return -1;
    }

    printf("MESSAGE ECRIT\n\n");
    printf("-------FIN PROGRAMME-------\n\n");

}