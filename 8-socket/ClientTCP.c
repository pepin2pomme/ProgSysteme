#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: %s <adresse_IP> <port> <message>\n", argv[0]);
        return 1;
    }

    char *ipServeur = argv[1];
    int port = atoi(argv[2]);
    char *messageEnvoyer = argv[3];

    int descripteurSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (descripteurSocket < 0) {
        perror("Erreur lors de la création de la socket");
        return 1;
    }

    struct sockaddr_in adresseServeur;
    memset(&adresseServeur, 0, sizeof(adresseServeur));
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(port);

    if (inet_pton(AF_INET, ipServeur, &adresseServeur.sin_addr) <= 0) {
        perror("Erreur de conversion d'adresse IP");
        close(descripteurSocket);
        return 1;
    }

    if (connect(descripteurSocket, (struct sockaddr *)&adresseServeur, sizeof(adresseServeur)) < 0) {
        perror("Erreur de connexion");
        close(descripteurSocket);
        return 1;
    }

    if (send(descripteurSocket, messageEnvoyer, strlen(messageEnvoyer), 0) < 0) {
        perror("Erreur lors de l'envoi du message");
        close(descripteurSocket);
        return 1;
    }

    printf("Message envoyé au serveur: %s\n", messageEnvoyer);

    char buffer[1024];
    int bytesReçus = recv(descripteurSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReçus < 0) {
        perror("Erreur de réception du message");
        close(descripteurSocket);
        return 1;
    }

    buffer[bytesReçus] = '\0';
    printf("Réponse du serveur: %s\n", buffer);

    close(descripteurSocket);

    return 0;
}
