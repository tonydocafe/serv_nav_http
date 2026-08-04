#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "url.h"

int analisar_url(char *url, URL *dados){
    if (strncmp(url, "http://", 7) != 0){
        return -1;
    }

    char *inicio_host = url + 7;
    strcpy(dados->path, "/");
    dados->port = 80;

    char *inicio_path = strchr(inicio_host, '/');

    if (inicio_path){
        strncpy(dados->host, inicio_host, inicio_path - inicio_host);
        dados->host[inicio_path - inicio_host] = '\0';
        strcpy(dados->path, inicio_path);
    }else{
        strcpy(dados->host, inicio_host);
    }
    char *porta = strchr(dados->host, ':');
   if (porta){
        *porta = '\0';
        dados->port = atoi(porta + 1);
    }
    return 0;
}

char *nome_arquivo(URL *dados){
    char *nome = strrchr(dados->path, '/');
    if (!nome || strlen(nome) == 1) return "index.html";
    return nome + 1;
}
