#ifndef URL_H
#define URL_H

typedef struct {
    char host[256];
    char path[512];
    int port;
} URL;


int analisar_url(char *url, URL *dados);

char *nome_arquivo(URL *dados);

#endif
