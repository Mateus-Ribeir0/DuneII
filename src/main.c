// src/main.c
#include "raylib.h"
#include "menu.h"
#include "game.h"

int main() {
    GameScreen currentScreen = TITLE;
    int dificuldade = 5;  // Valor padrão para a dificuldade

    iniciarMenu(&currentScreen, &dificuldade);

    // Loop principal do programa
    while (!WindowShouldClose()) {
        if (currentScreen == TITLE || currentScreen == RANKINGS) {
            atualizarMenu(&currentScreen, &dificuldade);
            desenharMenu(currentScreen);

            // Verifica se a tecla Enter foi pressionada para iniciar o jogo
            if (currentScreen == TITLE && IsKeyPressed(KEY_ENTER)) {
                currentScreen = GAME;  // Transição para o modo de jogo
            }
        } 
        else if (currentScreen == GAME) {
            // Chama a função principal do jogo com a dificuldade escolhida
            playGame(dificuldade);

            // Retorna ao menu após o término do jogo
            currentScreen = TITLE;
        }
    }

    finalizarMenu();
    return 0;
}
