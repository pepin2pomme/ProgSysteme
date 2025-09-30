#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

int main() {

    pid_t pidA, pidB;
    int p1[2];
    int p2[2];
    if (pipe(p1) == -1) { perror("pipe1"); exit(EXIT_FAILURE); }
    if (pipe(p2) == -1) { perror("pipe2"); exit(EXIT_FAILURE); }

    pidA = fork();
    if (pidA < 0) {
        perror("fork A");
        exit(EXIT_FAILURE);
    }

    if (pidA == 0) {

        close(p1[0]);
        close(p2[1]);

        char c[100];
        printf("Écrivez ici: ");
        scanf(" %[^\n]", c);

        write(p1[1], c, strlen(c));
        close(p1[1]);

        int val;
        read(p2[0], &val, sizeof(int));
        printf("Valeur reçue par le fils B: %d (espaces compris).\n", val);
        close(p2[0]);

        exit(0);
    } else {
        pidB = fork();
        if (pidB < 0) {
            perror("fork B");
            exit(EXIT_FAILURE);
        }

        if (pidB == 0) {

            close(p1[1]);
            close(p2[0]);

            char chaine;
            int cpt = 0;
            while (read(p1[0], &chaine, 1) > 0) {
                if (isalpha(chaine) || chaine == ' ') {
                    printf("%c", toupper(chaine));
                    cpt++;
                }
            }
            printf("\n");

            write(p2[1], &cpt, sizeof(int));
            close(p2[1]);

            close(p1[0]);
            exit(0);
        } else {
            close(p1[0]);
            close(p1[1]);
            close(p2[1]);

            wait(NULL);
            wait(NULL);

            printf("Pere terminé\n");
            exit(0);
        }
    }

    return 0;
}
