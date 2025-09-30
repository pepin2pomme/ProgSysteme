#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void handler_sigusr1(int sig){
    printf("\n ----------- \nSignal SIGUSR1 reçu\n ----------- \n");
}

void handler_sigusr2(int sig){
    printf("\n ----------- \nSignal SIGUSR2 reçu\n ----------- \n");
}

int main(){

    pid_t pid;
    int i;

    pid_t pidp = getpid();

    printf("Je suis le père, PID = %d\n", pidp); 

    signal(SIGUSR1, handler_sigusr1);
    signal(SIGUSR2, handler_sigusr2);

    for(i=0; i<3; i++){
        pid = fork();

        if(pid<0){
            printf("erreur fork");
            exit(EXIT_FAILURE);
        }

        if(pid==0){
            printf("Je suis le fils n°%d de %d, mon PID = %d\n", i+1, pidp, getpid());

            if(i==0){
                kill(getppid(), SIGUSR1);
            }
            else if(i==1){
                kill(getppid(), SIGUSR2);
            }
            else{
                printf("Fils 3\n");
            }

            exit(0);
        }
    }

    for(i=0; i<3; i++){
        wait(NULL);
    }
    
    printf("Terminé\n");

    return 0;

}