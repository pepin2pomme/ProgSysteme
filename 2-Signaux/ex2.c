#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handler_sigusr1(int sig){
    printf("Signal SIGUSR1 reçu\n");

    signal(SIGUSR1, SIG_IGN);
}

void handler_sigusr2(int sig){
    printf("Signal SIGUSR2 reçu\n");
}

int main()  {

    int i;

    printf("Je suis le PID = %d\n", getpid()); 

    signal(SIGUSR1, handler_sigusr1);
    signal(SIGUSR2, handler_sigusr2);

    read(0, &i, sizeof(int));

    return 0;
    
}