#ifndef RANKING_H
#define RANKING_H

#include "config.h"
#include "menu.h"

typedef struct Player {
    char name[MAX_NAME_LENGTH];
    int score;
    struct Player *next;
} Player;

void carregarRanking();
void atualizarRanking(const char *name, int score);
void exibirRankingScreen(GameScreen *currentScreen);
void liberarRanking();
void inserirNaLista(Player *newPlayer);

#endif
