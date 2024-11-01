# Nome do executável principal
TARGET = dune_game

# Compilador e flags
CC = gcc
CFLAGS = -Wall -std=c99

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include

# Lista de arquivos .c e .h
SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Bibliotecas necessárias
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Regra para gerar o executável principal
$(TARGET): $(SOURCES) $(HEADERS)
	@echo "Iniciando a compilação..."
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $(TARGET) $(LIBS)

# Limpeza dos arquivos temporários e executáveis
clean:
	rm -f $(TARGET)
