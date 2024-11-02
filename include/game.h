#ifndef GAME_H
#define GAME_H

#include "raylib.h"

void playGame(int dificuldade);  // Função principal do jogo
void movePlayer(int dx, int dy); // Declaração da função para mover o jogador
void initializeLobby();          // Função para inicializar o lobby
void drawLobby();     
#endif
