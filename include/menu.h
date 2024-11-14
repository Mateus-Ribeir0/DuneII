#ifndef MENU_H
#define MENU_H

#include "config.h"

void iniciarMenu(GameScreen *currentScreen);
void atualizarMenu(GameScreen *currentScreen);
void desenharMenu(GameScreen currentScreen);
void finalizarMenu();
void cutsceneArrakis(Music music);
void exibirObjetivo();
void desenharBackgroundComLogo();
void recebeNomeDoPlayer(GameScreen *currentScreen);

#endif
