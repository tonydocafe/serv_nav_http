CC = gcc
CFLAGS = -Wall -Wextra -O2 -Ihttp -Irede -Iurl

TARGET = meu_navegador

OBJ = main.o http/http.o rede/rede.o url/url.o


all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)


main.o: main.c
	$(CC) $(CFLAGS) -c main.c


http/http.o: http/http.c http/http.h
	$(CC) $(CFLAGS) -c http/http.c -o http/http.o


rede/rede.o: rede/rede.c rede/rede.h
	$(CC) $(CFLAGS) -c rede/rede.c -o rede/rede.o


url/url.o: url/url.c url/url.h
	$(CC) $(CFLAGS) -c url/url.c -o url/url.o


clean:
	rm -f $(TARGET) index.html $(OBJ)


.PHONY: all clean
