#ifndef CONFIG_H
#define CONFIG_H

#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"

typedef enum GameScreen { TITLE, NAME_INPUT, CUTSCENE, LOBBY, GAME, OBJETIVO, RANKINGS, EMPTY_SCREEN } GameScreen;

#define TILE_SIZE 32
#define MAPA_LARGURA 40
#define MAPA_ALTURA 22
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 704

#define MAX_NAME_LENGTH 25

#define MERCHANT_X 2
#define MERCHANT_Y 2
#define MERCHANT_X_LEFT (MERCHANT_X - 1)

#define BOLSA_CAPACIDADE_PEQUENA 6
#define BOLSA_CAPACIDADE_MEDIA 12
#define BOLSA_CAPACIDADE_GRANDE 24
#define BOLSA_CAPACIDADE_SUPER 32

#define PRECO_BOLSA_MEDIA 5000
#define PRECO_BOLSA_GRANDE 8000
#define PRECO_BOLSA_SUPER 12000

#define PORTAL_HORIZONTAL_LARGURA 3
#define PORTAL_HORIZONTAL_ALTURA 3
#define PORTAL_VERTICAL_LARGURA 3
#define PORTAL_VERTICAL_ALTURA 3

#define PORTAL_LOBBY_MAPA1_X 22
#define PORTAL_LOBBY_MAPA1_Y 14
#define PORTAL_LOBBY_MAPA2_X 28
#define PORTAL_LOBBY_MAPA2_Y 14
#define PORTAL_LOBBY_MAPA3_X 34
#define PORTAL_LOBBY_MAPA3_Y 14

#define PORTAL_RETORNO_LARGURA 3
#define PORTAL_RETORNO_ALTURA 3
#define PORTAL_RETORNO_X 18
#define PORTAL_RETORNO_Y 0

#define LOBBY_WATER_LOSS_INTERVAL 30.0
#define MAP1_WATER_LOSS_INTERVAL 20.0
#define MAP2_WATER_LOSS_INTERVAL 15.0
#define MAP3_WATER_LOSS_INTERVAL 10.0

#define GARRAFA_PEQUENA_CAPACIDADE 10 
#define GARRAFA_MEDIA_CAPACIDADE 20
#define GARRAFA_GRANDE_CAPACIDADE 30

#define PRECO_GARRAFA_PEQUENA 3000
#define PRECO_GARRAFA_MEDIA 5000
#define PRECO_GARRAFA_GRANDE 7000

#define FERRADURA_PRECO 3000
#define AMULETO_PRECO 7000
#define TREVO_PRECO 10000

#define FERRADURA_SORTE 20.0f
#define AMULETO_SORTE 30.0f
#define TREVO_SORTE 50.0f


extern int player_x;
extern int player_y;
extern int mapaAtual;
extern int itemsCollected;
extern int playerMoney;
extern const char* mensagem;
extern int MAX_ESPECIARIAS;
extern float playerWater;
extern float playerLucky;
extern double lastWaterUpdateTime;
extern char playerName[MAX_NAME_LENGTH];
extern bool usandoControle;
extern bool telaVaziaBloqueada;
extern bool spaceshipAnimationPlayed;
extern bool contFinal;


void comandoJogador(bool *usandoControle);
void DrawDialogBoxWithButtons(const char *text, int posX, int posY, int width, int height, Color boxColor, Color textColor, bool usandoControle, Texture2D texture, Rectangle *botaoAreas, float scale);
void DrawMenuOptionsWithButtons(const char *text, int posX, int posY, int fontSize, float buttonScale, Color textColor, bool usandoControle, Texture2D texture, Rectangle *botaoAreas);
void portable_sleep(int seconds);

#endif