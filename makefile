TARGET = dune_game

CC = gcc
CFLAGS = -Wall -std=c99

SRC_DIR = src
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S), Darwin)
    LIBS = -L../raylib/src -lraylib -lm -lpthread -ldl -framework OpenGL -framework Cocoa -framework IOKit
    CFLAGS += -I../raylib/src
endif
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $(TARGET) $(LIBS)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $(TARGET) $(LIBS) || ($(MAKE) clean && $(MAKE))

clean:
	rm -f $(TARGET)