// include/menu.h
#ifndef MENU_H
#define MENU_H

#include "config.h"

// Funções do menu
void iniciarMenu(GameScreen *currentScreen);
void atualizarMenu(GameScreen *currentScreen);
void desenharMenu(GameScreen currentScreen);
void finalizarMenu();
void cutsceneArrakis(Music music);

#endif
