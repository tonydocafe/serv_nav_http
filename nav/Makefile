CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = meu_navegador
SRC = meu_navegador.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run:
	./$(TARGET) http://example.com

clean:
	rm -f $(TARGET)
	rm -f index.html
	rm -f *.png *.jpg *.jpeg *.gif *.html *.json *.txt

.PHONY: all run clean
