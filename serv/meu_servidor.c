#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define PORTA 8080
#define BUFFER_TAM 4096

void enviar_arquivo(int cliente, const char *caminho);
void enviar_listagem(int cliente, const char *dir_caminho);
void enviar_erro(int cliente, int codigo, const char *mensagem);
void remover_crlf(char *s);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <diretorio>\n", argv[0]);
        return 1;
    }

    const char *diretorio_base = argv[1];
    struct stat st; 
    if (stat(diretorio_base, &st) != 0 || !S_ISDIR(st.st_mode)) {
        perror("Diretório inválido");
        return 1;
    }

//-------------------CONFIGURAÇÃO SOCKET CLIENTE E SERVIDOR------------------------------------------------------
    int servidor, cliente;
    struct sockaddr_in addr_servidor, addr_cliente;
    socklen_t addrlen = sizeof(addr_cliente);
    char buffer[BUFFER_TAM];

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor < 0) {
        perror("Erro ao criar socket");
        return 1;
    }



// TIME_WAIT
    int opt = 1;
    if (setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }



    addr_servidor.sin_family = AF_INET;
    addr_servidor.sin_addr.s_addr = INADDR_ANY;
    addr_servidor.sin_port = htons(PORTA);

    if (bind(servidor, (struct sockaddr*)&addr_servidor, sizeof(addr_servidor)) < 0) {
        perror("Erro no bind");
        close(servidor);
        return 1;
    }

    if (listen(servidor, 5) < 0) {
        perror("Erro no listen");
        close(servidor);
        return 1;
    }

    printf("Servidor HTTP rodando na porta %d\n", PORTA);
    printf("Servindo diretório: %s\n", diretorio_base);
//-----------------------------LOOP PRINCIPAL--------------------------------------------------------------------
    while (1) {
        cliente = accept(servidor, (struct sockaddr*)&addr_cliente, &addrlen);
        if (cliente < 0) {
            perror("Erro no accept");
            continue;
        }

        int bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            close(cliente);
            continue;
        }

        buffer[bytes] = '\0';
        remover_crlf(buffer);

        
        char metodo[8], caminho[512];
        sscanf(buffer, "%s %s", metodo, caminho);

        if (strcmp(metodo, "GET") != 0) {
            enviar_erro(cliente, 405, "Método não suportado");
            close(cliente);
            continue;
        }

//-------envio, tratamento e caminho completo para o arquivo servido-----------  
        char caminho_completo[1024];
        snprintf(caminho_completo, sizeof(caminho_completo), "%s%s", diretorio_base,
                 strcmp(caminho, "/") == 0 ? "/index.html" : caminho);

        if (stat(caminho_completo, &st) == 0) {
            if (S_ISREG(st.st_mode)) {
                enviar_arquivo(cliente, caminho_completo);
            } else if (S_ISDIR(st.st_mode)) {
                char caminho_index[1024];
               strncpy(caminho_index, caminho_completo, sizeof(caminho_index) - 1);
                caminho_index[sizeof(caminho_index) - 1] = '\0';
                strncat(caminho_index, "/index.html", sizeof(caminho_index) - strlen(caminho_index) - 1);

                if (stat(caminho_index, &st) == 0) {
                    enviar_arquivo(cliente, caminho_index);
                } else {
                    enviar_listagem(cliente, caminho_completo);
                }
            } else {
                enviar_erro(cliente, 403, "Acesso negado");
            }
        } else {
            enviar_erro(cliente, 404, "Arquivo não encontrado");
        }

        close(cliente);
    }

    close(servidor);
    return 0;
}

//-------------------------FUNÇOES AUXILIARES-----------------------------------------------------------------------



void enviar_arquivo(int cliente, const char *caminho) {
    FILE *f = fopen(caminho, "rb");
    if (!f) {
        enviar_erro(cliente, 404, "Arquivo não encontrado");
        return;
    }

    char header[BUFFER_TAM];
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    snprintf(header, sizeof(header),
             "HTTP/1.0 200 OK\r\n"
             "Content-Length: %ld\r\n"
             "Content-Type: text/html\r\n\r\n", tamanho);

    send(cliente, header, strlen(header), 0);

    char buffer[BUFFER_TAM];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        send(cliente, buffer, bytes, 0);
    }

    fclose(f);
}

void enviar_listagem(int cliente, const char *dir_caminho) {
    DIR *dir = opendir(dir_caminho);
    if (!dir) {
        enviar_erro(cliente, 404, "Diretório não encontrado");
        return;
    }

    char header[BUFFER_TAM];
    snprintf(header, sizeof(header),
             "HTTP/1.0 200 OK\r\n"
             "Content-Type: text/html\r\n\r\n");
    send(cliente, header, strlen(header), 0);

    char body[BUFFER_TAM];
    snprintf(body, sizeof(body),
             "<html><body><h1>Listagem de %s</h1><ul>", dir_caminho);
    send(cliente, body, strlen(body), 0);

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        snprintf(body, sizeof(body),
                 "<li><a href=\"new/%s\">%s</a></li>",
                  ent->d_name, ent->d_name);
        send(cliente, body, strlen(body), 0);
    }

    snprintf(body, sizeof(body), "</ul></body></html>");
    send(cliente, body, strlen(body), 0);
    closedir(dir);
}



void enviar_erro(int cliente, int codigo, const char *mensagem) {
    char buffer[BUFFER_TAM];
    snprintf(buffer, sizeof(buffer),
             "HTTP/1.0 %d %s\r\nContent-Type: text/html\r\n\r\n"
             "<html><body><h1>%d %s</h1></body></html>",
             codigo, mensagem, codigo, mensagem);
    send(cliente, buffer, strlen(buffer), 0);
}

void remover_crlf(char *s) {
    for (; *s; s++) {
        if (*s == '\r' || *s == '\n') *s = '\0';
    }
}
