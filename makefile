TARGET = dune_game

CC = gcc
CFLAGS = -Wall -std=c99

SRC_DIR = src
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $(TARGET) $(LIBS) || ($(MAKE) clean && $(MAKE))


clean:
	rm -f $(TARGET)