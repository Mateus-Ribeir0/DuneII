#ifndef CONFIG_H
#define CONFIG_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

typedef enum GameScreen { TITLE, RANKINGS, CUTSCENE, LOBBY, GAME } GameScreen;

// Constantes de configuração, disponíveis para todos os arquivos
#define TILE_SIZE 32
#define MAPA_LARGURA 40
#define MAPA_ALTURA 22
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 704

// Variáveis globais mutáveis
extern int player_x;
extern int player_y;
extern int mapaAtual;
extern int itemsCollected;

#endif
