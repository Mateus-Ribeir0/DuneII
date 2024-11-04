#ifndef LOBBY_H
#define LOBBY_H

#include "raylib.h"
#include "menu.h"

// Funções de inicialização e gerenciamento do lobby
void initializeLobby();
void processarEntradaLobby(GameScreen *currentScreen, bool *lobbyInitialized);
void atualizarLobby();
void desenharLobbyDetalhado();
void drawLobby();

#endif
