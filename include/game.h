#ifndef GAME_H
#define GAME_H

#include "raylib.h"

extern int player_x;
extern int player_y;
extern int mapaAtual;

void playGame();
void movePlayer(int dx, int dy);
void drawGame();
void inicializar_zonas(int mapa);
void initializeItems();

#endif
