#include "config.h"

// Variáveis globais mutáveis, com valores iniciais
int player_x = MAPA_LARGURA / 2;
int player_y = MAPA_ALTURA / 2;
int mapaAtual = 0;
int itemsCollected = 0;
float playerWater = 100.0;  // Nível de água inicial
double lastWaterUpdateTime = 0.0;  // Última vez que a água foi atualizada

