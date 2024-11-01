// src/main.c
#include "raylib.h"
#include "menu.h"
#include "game.h"
#include <stdlib.h>

int main() {
    // Inicializa o dispositivo de áudio
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_ERROR, "Falha ao inicializar o dispositivo de áudio.");
        return 1; // Sai do programa se o áudio não estiver pronto
    }

    // Carrega a música
    Music titleMusic = LoadMusicStream("static/music/epicversion3.wav");
    if (titleMusic.stream.buffer == NULL) {
        TraceLog(LOG_ERROR, "Falha ao carregar o arquivo de música.");
        CloseAudioDevice(); // Fecha o dispositivo de áudio se a música falhar
        return 1;
    }

    // Ajuste de volume e inicialização da reprodução
    SetMusicVolume(titleMusic, 1.0f);
    PlayMusicStream(titleMusic);

    // Define a tela inicial
    GameScreen currentScreen = TITLE;
    int dificuldade = 5;

    iniciarMenu(&currentScreen, &dificuldade);

    // Loop principal do programa
    while (!WindowShouldClose()) {
        // Atualiza o áudio somente se o dispositivo de áudio estiver pronto
        if (IsAudioDeviceReady()) {
            UpdateMusicStream(titleMusic);
        }

        if (currentScreen == TITLE || currentScreen == RANKINGS) {
            atualizarMenu(&currentScreen, &dificuldade);
            desenharMenu(currentScreen);

            if (currentScreen == TITLE && IsKeyPressed(KEY_ENTER)) {
                currentScreen = CUTSCENE;  // Transição para a cutscene
            }
        } else if (currentScreen == CUTSCENE) {
            cutsceneArrakis(titleMusic);  // Passa a música para a cutscene
            currentScreen = GAME;  // Transição para o jogo após a cutscene
        } else if (currentScreen == GAME) {
            playGame(dificuldade);
            currentScreen = TITLE;  // Retorna ao menu após o jogo
        }
    }

    // Finaliza a música e fecha o dispositivo de áudio
    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    CloseAudioDevice();

    finalizarMenu();
    return 0;
}