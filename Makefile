CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
LDFLAGS = -lm

CFLAGS = -Wall -Wextra -std=c11 -pedantic -g -D_XOPEN_SOURCE=700

TARGET = chamados_ti
SOURCES = main.c arvore.c pilha.c persistencia.c utils.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✓ Compilação concluída: $(TARGET)"

%.o: %.c chamados.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "✓ Arquivos de compilação removidos"

help:
	@echo "Targets disponíveis:"
	@echo "  make all   - Compilar o projeto"
	@echo "  make run   - Compilar e executar"
	@echo "  make clean - Remover arquivos compilados"
	@echo "  make help  - Mostrar esta mensagem"
