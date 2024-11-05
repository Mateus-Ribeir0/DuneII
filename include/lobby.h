#ifndef LOBBY_H
#define LOBBY_H

#include "config.h"
#include "game.h" //O lobby está herdando o game.h, pois talvez a gente precise de informações do game.c futuramente!

// Funções de inicialização e gerenciamento do lobby
void initializeLobby();
void processarEntradaLobby(GameScreen *currentScreen, bool *lobbyInitialized);
void atualizarLobby();
void desenharLobbyDetalhado();
void drawLobby();

#endif
