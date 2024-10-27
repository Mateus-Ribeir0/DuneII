# Nome do executável
TARGET = duneII

# Compilador e flags
CC = gcc
CFLAGS = -Wall

# Lista de arquivos .c e .h
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)

# Regra para gerar o executável
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -Iinclude -o $(TARGET)

# Limpeza dos arquivos temporários
clean:
	rm -f $(TARGET) src/*.o