TARGET = dune_game

CC = gcc
CFLAGS = -Wall -std=c99 -v

SRC_DIR = src
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S), Darwin)
    LIBS = -lraylib -lm -lpthread -ldl -framework OpenGL -framework Cocoa -framework IOKit
endif

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $(TARGET) $(LIBS) || ($(MAKE) clean && $(MAKE))

clean:
	rm -f $(TARGET)