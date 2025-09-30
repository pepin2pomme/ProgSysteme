#include <sys/ipc.h>  
#include <sys/shm.h>  
#include <sys/types.h>  
#include <stdio.h>   
#include <stdlib.h>     
#include <string.h>
#include <time.h>



int creerZDC(int size, char *name, int ncle){
    
    key_t cle = ftok("./",5);
    int shmId = shmget(cle, size, IPC_CREAT | IPC_EXCL | 0600);

    if (shmId == -1) {
        shmId=shmget(cle,size,IPC_EXCL);
        if(shmId==-1){
            perror("Erreur lors de la création de la ZDC");
            return -1;
        }
        printf("La ZDC existait déjà\n");
    }

    return shmId;
}

int infosZDC(int shmId){

    if(shmId==-1){
        perror("Erreur: la ZDC n'existe pas");
        return -1;
    }

    struct shmid_ds info;

    shmctl(shmId, IPC_STAT, &info);

    printf("-----------INFORMATIONS-----------\n");
    printf("ShmId              : %d\n", shmId);
    printf("Taille             : %zu octets\n", info.shm_segsz);
    printf("UID propriétaire   : %u\n", info.shm_perm.uid);
    printf("GID propriétaire   : %u\n", info.shm_perm.gid);
    printf("Créateur UID       : %u\n", info.shm_perm.cuid);
    printf("Créateur GID       : %u\n", info.shm_perm.cgid);
    printf("Dernier PID attach : %d\n", info.shm_lpid);
    printf("PID créateur       : %d\n", info.shm_cpid);
    printf("Nb attaches actifs : %lu\n", info.shm_nattch);
    printf("----------------------------------\n");

    return 0;
}

int detruireZDC(int shmId){

    if(shmId==-1){
        perror("Erreur: ZDC introuvable ou n'existe pas");
        return -1;
    }

    shmctl(shmId, IPC_RMID, NULL);
    printf("ZDC %d Supprimée avec succès\n", shmId);

    return 0;
}

int main(){

    int shmId = creerZDC(100, "test2", 2);
    printf("%d\n",shmId);

    infosZDC(shmId);

    detruireZDC(shmId);

    return 0;
}