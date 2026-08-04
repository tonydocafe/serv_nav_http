#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "http.h"


int enviar_get(int socket, char *host, char *path){
    char requisicao[1024];

    snprintf(requisicao, sizeof(requisicao), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path, host);

    return send(socket, requisicao, strlen(requisicao), 0);
}

int receber_resposta(int socket, char *arquivo){

    char buffer[4096];
    int bytes;
    int header_feito = 0;
    char *header_fim;
    FILE *f = fopen(arquivo,"wb");

    if(!f) return -1;

    while((bytes = recv(socket,buffer,sizeof(buffer),0)) > 0){

        if(!header_feito){

            header_fim = strstr(buffer,"\r\n\r\n");
            if(header_fim){

                *header_fim = '\0';

                printf("Cabeçalhos HTTP:\n%s\n\n",buffer);

                if(strstr(buffer,"200 OK")==NULL)
                {
                    fclose(f);
                    return -1;
                }
                char *body = header_fim + 4;
		fwrite( body, 1, bytes-(body-buffer), f);
                header_feito = 1;
            }
        }else{
            fwrite(buffer,1,bytes,f);

        }
    }
    fclose(f);
    return 0;
}
