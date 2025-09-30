#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define BUFFER_SIZE 256

int main(){

    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t size;

    if (pipe(pipefd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid= fork();
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid==0){
        close(pipefd[1]);
        printf("dans le fils\n");
        while(1){
            size = read(pipefd[0], buffer, sizeof(buffer));
            if(size == -1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            if(size == 0){
                break;
            }

            for(ssize_t i = 0; i<size; i++){
                if (isalpha((unsigned char)buffer[i])) {
                    putchar(buffer[i]);
                }
            }
        }
        close(pipefd[0]);
        printf("\n");
        exit(EXIT_SUCCESS);
    } else {
        close(pipefd[0]);

        printf("père: entrer des caractères à envoyer au fils (ctrl+d):\n");

        while(fgets(buffer, sizeof(buffer), stdin) != NULL){
            if(write(pipefd[1], buffer, strlen(buffer)) == -1){
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(pipefd[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;


}
