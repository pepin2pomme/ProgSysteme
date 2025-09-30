#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

//#define BUFFER_SIZE 256

int main(){

    int p1[2];
    int p2[2];
    pid_t pid;

    if(pipe(p1) == -1){perror("pipe"); exit(EXIT_FAILURE);}
    if(pipe(p2) == -1){perror("pipe"); exit(EXIT_FAILURE);}

    pid = fork();
    if(pid == -1){perror("fork"); exit(EXIT_FAILURE);}

    if(pid==0){
        close(p1[1]); // le fils  n'ecrit pas dans le pere pipe1
        close(p2[0]); // le fils ne lis pas le père pipe2
        char chaine;

        int cpt=0;

        while(read(p1[0], &chaine, 1)>0){
            if(isalpha(chaine) || chaine==32){
                printf("%c", toupper(chaine));
                cpt++;
            }
        }
        printf("\n");
        close(p1[0]);

        write(p2[1], &cpt, sizeof(int));
        close(p2[1]);

        exit(0);
    } else { 
        char c[100];
        close(p1[0]); // le pere ne lit pas le fils pipe1
        close(p2[1]); // le pere n'ecrit pas dans le fils pipe2
        int val;

        printf("Ecrivez ici: ");
        scanf(" %[^\n]", c);
        write(p1[1], &c, strlen(c));

        close(p1[1]);

        read(p2[0], &val, sizeof(int));
        printf("Valeur reçue par le pere: %d\n", val);
        close(p2[0]);
    }

}
/*
int main(){

    int pipefd1[2];
    int pipefd2[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t size;

    if (pipe(pipefd1) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd2) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid= fork();
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid==0){
        int compteur=0;
        close(pipefd2[0]); //fils ne lit pas le père dans 2eme pipe
        close(pipefd1[1]); //fils n'ecrit pas au pere dans 1er pipe
        printf("dans le fils\n");
        while(1){
            size = read(pipefd1[0], buffer, sizeof(buffer));
            if(size == -1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            if(size == 0){
                break;
            }

            for(ssize_t i = 0; i<size; i++){
                if (isalpha((unsigned char)buffer[i])) {
                    putchar((toupper(buffer[i])));
                    compteur++;
                }
            }
        }
        close(pipefd1[0]);
        printf("compteur à : %d\n", compteur);

        write(pipefd2[1], &compteur, sizeof(compteur));
        close(pipefd2[1]);
        exit(EXIT_SUCCESS);
    } else {
        int val_recue;
        close(pipefd1[0]); //pere ne lit pas le fils pipe1
        close(pipefd2[1]); //pere n'ecrit pas dans le fils pipe2

        printf("père: entrer des caractères à envoyer au fils (ctrl+d):\n");

        while(fgets(buffer, sizeof(buffer), stdin) != NULL){
            if(write(pipefd1[1], buffer, strlen(buffer)) == -1){
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        close(pipefd1[1]);

        read(pipefd2[0], &val_recue, sizeof(val_recue));
        printf("%d", val_recue);

        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;


}
*/