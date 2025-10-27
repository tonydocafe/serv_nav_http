CC = gcc
CFLAGS = -Wall -Wextra -O2

all: meu_navegador

meu_navegador: meu_navegador.c
	$(CC) $(CFLAGS) -o meu_navegador meu_navegador.c

clean:
	
	@for file in *; do \
		if [ "$$file" != "Makefile" ] && [ "$$file" != "meu_navegador.c" ]; then \
			rm -rf "$$file"; \
		fi; \
	done

.PHONY: all clean