#include "config.h"

int player_x = MAPA_LARGURA / 2;
int player_y = MAPA_ALTURA / 2;
int mapaAtual = 0;
int itemsCollected = 0;
float playerWater = 100.0;
double lastWaterUpdateTime = 0.0;
char playerName[MAX_NAME_LENGTH] = "";
