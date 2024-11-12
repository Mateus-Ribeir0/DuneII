
#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "ranking.h"

void playGame(GameScreen *currentScreen);
void movePlayer(int dx, int dy);
void drawGame();
void inicializar_zonas(int mapa);
void initializeItems();
void limparHistoricoPassos();
void limparColisoesEZonas();
void UnloadAssets();

#endif