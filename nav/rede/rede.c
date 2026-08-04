#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "rede.h"

int conectar(char *host, int porta){

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("Erro socket");
        return -1;
    }

    struct hostent *server = gethostbyname(host);

    if(!server){
        fprintf(stderr,"Host não encontrado\n");
        close(sock);
        return -1;
    }

    struct sockaddr_in endereco;
    memset(&endereco,0,sizeof(endereco));

    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(porta);

    memcpy( &endereco.sin_addr.s_addr, server->h_addr, server->h_length);

    if(connect( sock, (struct sockaddr *)&endereco, sizeof(endereco)) < 0){
        perror("Erro conexão");
        close(sock);
        return -1;
    }

    return sock;
}
