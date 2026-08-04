#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "url.h"
#include "rede.h"
#include "http.h"

void uso(){
    printf("Uso:\n");
    printf("./meu_navegador <URL>\n");
    exit(1);
}

int main(int argc,char *argv[]){

    if(argc != 2)uso();

    URL url;

    if(analisar_url(argv[1], &url)<0){
        printf("URL invalida\n");
        return 1;
    }

    printf("Host: %s\nPorta: %d\nCaminho: %s\n",url.host,url.port,url.path);

    int sock = conectar(url.host,url.port);

    if(sock < 0)return 1;

    if(enviar_get(sock,url.host,url.path)<0){
        printf("Erro enviando GET\n");
        close(sock);
        return 1;
    }

    char *arquivo = nome_arquivo(&url);

    if(receber_resposta(sock,arquivo)<0){
        printf("Erro recebendo resposta\n");
        close(sock);
        return 1;
    }

    printf("Arquivo salvo: %s\n",arquivo);
    close(sock);
    return 0;
}
