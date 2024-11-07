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

// Definição da posição do mercador
#define MERCHANT_X 5
#define MERCHANT_Y 5
#define MAX_ESPECIARIAS 6

// Dimensões para os portais
#define PORTAL_HORIZONTAL_LARGURA 4
#define PORTAL_HORIZONTAL_ALTURA 2
#define PORTAL_VERTICAL_LARGURA 2
#define PORTAL_VERTICAL_ALTURA 4

// Coordenadas para os três portais no lobby
#define PORTAL_LOBBY_MAPA1_X 25
#define PORTAL_LOBBY_MAPA1_Y 1
#define PORTAL_LOBBY_MAPA2_X 37
#define PORTAL_LOBBY_MAPA2_Y 9
#define PORTAL_LOBBY_MAPA3_X 25
#define PORTAL_LOBBY_MAPA3_Y 19

// Coordenadas para o portal de retorno ao lobby
#define PORTAL_RETORNO_LARGURA 2
#define PORTAL_RETORNO_ALTURA 4
#define PORTAL_RETORNO_X 37
#define PORTAL_RETORNO_Y 9

// Variáveis globais mutáveis
extern int player_x;
extern int player_y;
extern int mapaAtual;
extern int itemsCollected;
extern int playerMoney;
extern const char* mensagem;

#endif
