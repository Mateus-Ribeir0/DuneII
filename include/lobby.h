#ifndef LOBBY_H
#define LOBBY_H

#include "config.h"
#include "game.h"

// Funções de inicialização e gerenciamento do lobby
void processarEntradaLobby(GameScreen *currentScreen, bool *lobbyInitialized);
void desenharLobbyDetalhado();
void drawLobby();
bool isPlayerOnPortal(int new_x, int new_y, int mapaAtual);
void updateWaterLevel(GameScreen *currentScreen);
void resetarJogo();
void iniciarLobby();
void finalizarLobby();

#endif
