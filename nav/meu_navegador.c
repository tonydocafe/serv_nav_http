#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

void uso() {
    printf("Uso: meu_navegador <URL>\n");
    printf("Exemplo: meu_navegador http://httpbin.org/image/png\n./meu_navegador http://httpbin.org/json\n./meu_navegador http://example.com\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) uso();

    char *url = argv[1];

    if (strncmp(url, "http://", 7) != 0) {
        fprintf(stderr, "Erro: a URL deve começar com 'http://'\n");
        return 1;
    }
//--------------------TRATAMENTO DE CAMINHO E HOST--------------------------------------
    char *inicio_do_host = url + 7;
    char host[256] = {0};
    char path[512] = "/";
    int port = 80;

    
    char *inicio_do_caminho = strchr(inicio_do_host, '/');
    if (inicio_do_caminho) {
        strncpy(host, inicio_do_host, inicio_do_caminho - inicio_do_host);
        strcpy(path, inicio_do_caminho);
    } else {
        strcpy(host, inicio_do_host);
    }

    char *porta_extra = strchr(host, ':');
    if (porta_extra) {
        *porta_extra = '\0';
        port = atoi(porta_extra + 1);
    }

    printf("Conectando em %s:%d\n", host, port);
    printf("Recurso: %s\n", path);

//----------------- CRIAÇÃO E CONEXÃO DE SOCKET E RESOLUÇÃO DNS-----------------------------
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Erro ao criar socket");
        return 1;
    }
    // resolve o nome do host e pega o IP
    struct hostent *server = gethostbyname(host);
    if (!server) {
        fprintf(stderr, "Host não encontrado.\n");
        return 1;
    }
    // define IPv4, porta e copia o IP.
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    //conexão
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro na conexão");
        return 1;
    }

//------------------ENVIO DE REQUISIÇÃO E LEITURA DE RESPOSTA-------------------------------
    char requis[1024];
    snprintf(requis, sizeof(requis),"GET %s HTTP/1.0\r\nHost: %s\r\n\r\n",path, host);
    send(sock, requis, strlen(requis), 0);

   //leitura
    char buffer[BUFFER_SIZE];
    int bytes;
    char *header_fim;
    FILE *f = NULL;
    int header_feito = 0;

    while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        if (!header_feito) {
            header_fim = strstr(buffer, "\r\n\r\n");
            if (header_fim) {
                *header_fim = '\0';
                printf("Cabeçalhos HTTP:\n%s\n\n", buffer);

               
                if (strstr(buffer, "200 OK") == NULL) {
                    fprintf(stderr, "Erro: arquivo não encontrado ou resposta inválida.\n");
                    close(sock);
                    return 1;
                }

//-----------------MANIPULAÇÃO DO ARQUIVO ------------------------------------------------------- 
                
                char *nome_do_arquivo = strrchr(path, '/');
                if (!nome_do_arquivo || strlen(nome_do_arquivo) == 1)
                    nome_do_arquivo = "index.html";
                else
                    nome_do_arquivo++;

                f = fopen(nome_do_arquivo, "wb");
                if (!f) {
                    perror("Erro ao criar arquivo");
                    close(sock);
                    return 1;
                }

                //escrever apos o cabeçalho 
                char *body = header_fim + 4;
                int body_largura = bytes - (body - buffer);
                fwrite(body, 1, body_largura, f);

                header_feito = 1;
            }
        } else {
            fwrite(buffer, 1, bytes, f);
        }
    }

    if (f) {
        fclose(f);
        printf("Arquivo salvo com sucesso.\n");
    }

    close(sock);
    return 0;
}
