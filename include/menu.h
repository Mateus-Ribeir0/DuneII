#ifndef MENU_H
#define MENU_H

#include "config.h"

void iniciarMenu(GameScreen *currentScreen);
void finalizarMenu();
void atualizarMenu(GameScreen *currentScreen);
void drawMenu();
void cutsceneArrakis();
void exibirObjetivo(GameScreen *currentScreen);
void desenharBackgroundComLogo();
void recebeNomeDoPlayer(GameScreen *currentScreen);
void showFadingImage(Texture2D image, float duration);
void displayCutscene(Texture2D image, const char* text, Music titleMusic, float speed, float scale, float duration);

#endif
