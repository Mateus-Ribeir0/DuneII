// include/menu.h
#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum GameScreen { TITLE, RANKINGS, CUTSCENE, GAME } GameScreen;

// Funções do menu
void iniciarMenu(GameScreen *currentScreen, int *dificuldade);
void atualizarMenu(GameScreen *currentScreen, int *dificuldade);
void desenharMenu(GameScreen currentScreen);
void finalizarMenu();

#endif
