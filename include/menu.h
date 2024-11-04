// include/menu.h
#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum GameScreen { TITLE, RANKINGS, CUTSCENE, LOBBY, GAME } GameScreen;


// Funções do menu
void iniciarMenu(GameScreen *currentScreen);
void atualizarMenu(GameScreen *currentScreen);
void desenharMenu(GameScreen currentScreen);
void finalizarMenu();
void cutsceneArrakis(Music music);

#endif
