#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void handler_sigusr1(int sig){
    printf("\n ----------- \nSignal SIGUSR1 reçu par le fils\n ----------- \n");
}

int main(){

    pid_t pid;
    
    printf("Je suis le père, PID = %d\n", getpid()); 

    pid = fork();

    if(pid<0){
        printf("erreur fork");
        exit(EXIT_FAILURE);
    }

    if(pid==0){
        signal(SIGUSR1, handler_sigusr1);
        while(1){sleep(1);printf("le fils tourne\n");}
    }

    printf("1e sleep\n");
    sleep(3);

    kill(pid, SIGUSR1);

    printf("2e sleep\n");
    sleep(3);

    printf("\n ----------- \nEn attente de la fin du fils\n ----------- \n");

    int status;
    wait(&status);

    printf("\n ----------- \nFils terminé par le signal = %d ; fin du programme\n ----------- \n", WTERMSIG(status));

}