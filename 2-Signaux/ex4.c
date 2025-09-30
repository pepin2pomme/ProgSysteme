#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int a = 1;

void handler_ctrlc(int sig){
    a++;
}

void handler_quit(int sig){
    printf("\nSignal SIGQUIT reçu, SIGINT bloqué");
    signal(SIGINT, SIG_IGN);
}

int main()  {

    signal(SIGINT, handler_ctrlc);
    signal(SIGQUIT, handler_quit);
    signal(SIGTSTP, SIG_IGN);

    int i=0;
    while(i<20){
        sleep(1);
        printf("\na = %d", a);
        i++;
    }

    return 0;
    
}