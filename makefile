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

# Detecta o sistema operacional
UNAME_S := $(shell uname -s)

# Configura as bibliotecas de acordo com o sistema operacional
ifeq ($(UNAME_S), Linux)
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S), Darwin)
    LIBS = -lraylib -lm -lpthread -ldl -framework OpenGL -framework Cocoa -framework IOKit
endif

# Regra para gerar o executável principal
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $(TARGET) $(LIBS) || ($(MAKE) clean && $(MAKE))

# Limpeza dos arquivos temporários e executáveis
clean:
	rm -f $(TARGET)