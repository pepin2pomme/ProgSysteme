#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345

int main(int argc, char *argv[]){

    if(argc!=3){
        printf("Il faut 1 seul arguments\n");
        return 1;
    }

    char *ipServeur = argv[1];
    char *messageEnvoyer = argv[2];
    
    int descripteurSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (descripteurSocket < 0) {
        perror("Erreur lors de la création de la socket");
        exit(-1);
    }
    printf("Socket créée avec succès ! (%d)\n", descripteurSocket);

    int connect(int sockfd, const struct sockaddr* serv_addr,socklen_t addrlen);



    
    close(descripteurSocket);

    return 0;
}