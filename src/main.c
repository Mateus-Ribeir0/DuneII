// src/main.c
#include "raylib.h"
#include "menu.h"
#include "game.h"

// Declaração da nova função para a cutscene
void cutsceneArrakis();

int main() {
    GameScreen currentScreen = TITLE;
    int dificuldade = 5;  // Valor padrão para a dificuldade

    InitAudioDevice();  // Inicializa o dispositivo de áudio

    Music titleMusic = LoadMusicStream("static/music/epicversion3.wav"); // Carrega a música do menu
    SetMusicVolume(titleMusic, 1.0f);
    PlayMusicStream(titleMusic);

    iniciarMenu(&currentScreen, &dificuldade);

    // Loop principal do programa
    while (!WindowShouldClose()) {

        UpdateMusicStream(titleMusic);

        if (currentScreen == TITLE || currentScreen == RANKINGS) {
            atualizarMenu(&currentScreen, &dificuldade);
            desenharMenu(currentScreen);

            // Verifica se a tecla Enter foi pressionada para iniciar a cutscene
            if (currentScreen == TITLE && IsKeyPressed(KEY_ENTER)) {
                currentScreen = CUTSCENE;  // Transição para o estado de cutscene
            }
        } 
        else if (currentScreen == CUTSCENE) {
            // Chama a cutscene e aguarda a conclusão para iniciar o jogo
            cutsceneArrakis();
            currentScreen = GAME;  // Transição para o modo de jogo após a cutscene
        }
        else if (currentScreen == GAME) {
            // Chama a função principal do jogo com a dificuldade escolhida
            playGame(dificuldade);

            // Retorna ao menu após o término do jogo
            currentScreen = TITLE;
        }
    }

    StopMusicStream(titleMusic);   // Para a música ao sair do programa
    UnloadMusicStream(titleMusic); // Descarrega a música
    CloseAudioDevice();

    finalizarMenu();
    return 0;
}
