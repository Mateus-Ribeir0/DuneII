TARGET = DuneII

SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib_raylib
RELEASE_DIR = release

SOURCES = $(wildcard $(SRC_DIR)/*.c)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    CC = gcc
    CFLAGS = -Wall -std=c99 -I$(INCLUDE_DIR)
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    EXE_EXT =
else ifeq ($(UNAME_S), Darwin)
    CC = gcc
    CFLAGS = -Wall -std=c99 -I$(INCLUDE_DIR)
    LIBS = -L$(LIB_DIR) -lraylib -lm -lpthread -framework OpenGL -framework Cocoa -framework IOKit
    EXE_EXT =
else
    CC = gcc
    CFLAGS = -Wall -std=c99 -I$(INCLUDE_DIR) -mwindows
    LIBS = -L$(LIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm
    ICON_RC = resource.rc
    ICON_OBJ = resource.o
    EXE_EXT = .exe
endif

$(TARGET)$(EXE_EXT): $(SOURCES) $(ICON_OBJ)
	$(CC) $(CFLAGS) $(SOURCES) $(ICON_OBJ) -o $(TARGET)$(EXE_EXT) $(LIBS)

$(ICON_OBJ): $(ICON_RC)
ifeq ($(UNAME_S), Linux)
	@echo "Ícones não são suportados no Linux."
else ifeq ($(UNAME_S), Darwin)
	@echo "Ícones não são suportados no macOS."
else
	windres $(ICON_RC) -o $(ICON_OBJ)
endif

release: $(TARGET)$(EXE_EXT)
	@echo "Criando release..."
	@mkdir -p $(RELEASE_DIR)
	@cp $(TARGET)$(EXE_EXT) $(RELEASE_DIR)/
	@test -d static && cp -r static $(RELEASE_DIR)/ || { echo "Erro: pasta 'static' não encontrada."; exit 1; }
	@echo "Release criada em: $(RELEASE_DIR)"

clean:
	rm -f $(TARGET)$(EXE_EXT)
	rm -f $(ICON_OBJ)
	@echo "Limpeza concluida!"
