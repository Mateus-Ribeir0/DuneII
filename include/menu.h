// include/menu.h
#ifndef MENU_H
#define MENU_H

#include "raylib.h"

// Enumeração para as telas do jogo
typedef enum GameScreen { TITLE, RANKINGS, GAME } GameScreen;

// Funções do menu
void iniciarMenu(GameScreen *currentScreen, int *dificuldade);
void atualizarMenu(GameScreen *currentScreen, int *dificuldade);
void desenharMenu(GameScreen currentScreen);
void finalizarMenu();

#endif
