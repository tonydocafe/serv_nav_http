#ifndef HTTP_H
#define HTTP_H


int enviar_get( int socket, char *host, char *path);

int receber_resposta( int socket, char *arquivo);

#endif
