#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void gestionnaire(int numSig) 
{
  printf("Signal %d (%s) reçu\n", numSig, strsignal(numSig));
}

int main() 
{
   int i;
   struct sigaction action;
   sigset_t ensemble;
   
   printf("Je suis le PID = %d\n", getpid()); 
   
   /* on installe le gestionnaire */
   action.sa_handler = gestionnaire;
   sigemptyset(&action.sa_mask);
   action.sa_flags = 0;
   
   /* on intercepte tous les signaux */
   for (i=1;i<NSIG;i++) sigaction(i, &action, NULL);
   
   /* on bloque tous les signaux sauf SIGINT */
   sigfillset(&ensemble);
   sigdelset(&ensemble, SIGINT);
   sigprocmask(SIG_BLOCK, &ensemble, NULL);
   
   /* appel système lent */
   read(0, &i, sizeof(int)); /* on continue par une saisie */
   
   /* quels sont les signaux en attente ? */
   sigpending(&ensemble);
   for (i=1;i<NSIG; i++)
      if (sigismember(&ensemble, i))
         printf("Signal %d (%s) en attente\n", i, (i>31) ? "sans nom" : strsignal(i));
   
   /* on débloque tous les signaux */
   sigemptyset(&ensemble);
   sigprocmask(SIG_SETMASK, &ensemble, NULL);
 read(0, &i, sizeof(int)); /* on continue par une saisie */
   return 0;
}
