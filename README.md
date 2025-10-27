# Projetos em C: Navegador e Servidor HTTP

Este repositório contém dois projetos em C que implementam um **navegador HTTP** e um **servidor HTTP** simples. Ambos foram desenvolvidos para sistemas com suporte a sockets (Linux).

## Navegador HTTP

O **navegador HTTP** é um programa que funciona como um navegador simples em modo texto, permitindo acessar sites e baixar arquivos especificados pelo usuário.

Ele recebe o endereço do site, separa o nome do servidor e o caminho do arquivo, ajusta a porta se necessário e estabelece uma conexão com o servidor.

Após enviar a solicitação pelo protocolo HTTP, o programa lê a resposta, exibe os cabeçalhos na tela para informar se houve algum erro, e salva o conteúdo em um arquivo local com nome baseado no recurso solicitado ou em index.html por padrão.

Ao final, fecha a conexão e confirma que o arquivo foi salvo, permitindo que o usuário baixe qualquer recurso indicado de forma automática.

## Servidor HTTP

O **servidor HTTP** funciona como um servidor de páginas, permitindo que clientes acessem arquivos de um diretório específico.

Ele recebe o caminho do diretório e espera conexões, lendo os pedidos feitos pelos clientes.

Se o arquivo solicitado existir, ele é enviado; se houver um diretório sem index.html, o servidor gera uma página listando os arquivos disponíveis.

Em caso de erro, como arquivo não encontrado, uma mensagem é enviada.

O servidor continua ativo, respondendo a novos pedidos e permitindo acessar de forma simples e organizada o conteúdo do diretório.

---

## Sumário

- [1. Navegador HTTP](#1-navegador-http-meu_navegadorc)
- [2. Servidor HTTP](#2-servidor-http-meu_servidorc)
- [3. Compilação](#3-compilação-makefile)
- [4. Uso](#4-uso-executavel)
---
### Requisitos

- Sistema com suporte a sockets (Linux).
- Compilador C (ex.: `gcc`).

---

## 1. Navegador HTTP (`meu_navegador.c`)

### Funcionalidade

- Aceita uma URL HTTP (ex.: `http://example.com/index.html`).
- Conecta ao servidor remoto via TCP.
- Realiza uma requisição HTTP GET.
- Salva o conteúdo retornado em arquivo local.
- Exibe cabeçalhos HTTP no terminal.


## 2. Servidor HTTP (`meu_servidor.c`)

### Funcionalidade

- Configura socket servidor na porta 8080 usando TCP.
- Valida diretório base e processa requisições GET.
- Serve arquivos regulares ou lista diretórios (com fallback para index.html).
- Implementa respostas de erro HTTP para casos comuns.
- Usa funções auxiliares para envio de arquivos, listagem e tratamento de erros.


## 3. Compilação (`makefile`)

```bash
# Compilar navegador -> nav/
make

# Compilar servidor -> serv/
make

```

## 4. Uso (`executavel`)

```bash
# executar navegador -> nav/
./meu_navegador <URL>

# executar servidor -> serv/
./meu_servidor <diretorio>


```

## 5. Acesso (porta 8080)

Para acessar a interface do servidor localmente :

```bash
# Acessar servidor na mesma máquina link 
  http://localhost:8080/arquivo

# Acessar servidor em outra maquina na mesma rede
http://ip_da_maquina_que_esta_o_servidor:8080/arquivo

# Para conferir o IP da maquina (192.68. ...) 
hostname -I



```

