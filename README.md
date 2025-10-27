# Projetos em C: Navegador e Servidor HTTP

Este repositório contém dois projetos em C que implementam um **navegador HTTP** e um **servidor HTTP** simples. Ambos foram desenvolvidos para sistemas com suporte a sockets (Linux, macOS).

---

## Sumário

- [1. Navegador HTTP](#1-navegador-http-meu_navegadorc)
- [2. Servidor HTTP](#2-servidor-http-meu_servidorc)
- [3. Compilação](#3-compilação-makefile)
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

## 4. Uso (`./`)

```bash
# executar navegador -> nav/
./meu_navegador <URL>

# executar servidor -> serv/
./meu_servidor <diretorio>

```

