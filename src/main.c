#include "menu.h"
#include "game.h"
#include "lobby.h"

int main() {
    GameScreen currentScreen = TITLE;
    bool lobbyInitialized = false;

    InitAudioDevice();
    Music titleMusic = LoadMusicStream("static/music/epicversion3.wav");
    PlayMusicStream(titleMusic);
    SetMusicVolume(titleMusic, 1.0f);

    iniciarMenu(&currentScreen);

    while (!WindowShouldClose()) {
    UpdateMusicStream(titleMusic);

    switch (currentScreen) {
        case TITLE:
        case RANKINGS:
            atualizarMenu(&currentScreen);
            desenharMenu(currentScreen);
            if (IsKeyPressed(KEY_ENTER) && currentScreen == TITLE) {
                currentScreen = CUTSCENE;
            }
            break;
        case CUTSCENE:
            cutsceneArrakis(titleMusic);
            currentScreen = LOBBY;
            break;
        case LOBBY:
            processarEntradaLobby(&currentScreen, &lobbyInitialized);
            BeginDrawing();
            desenharLobbyDetalhado();
            EndDrawing();
            break;
        case GAME:
            playGame(&currentScreen);
            break;
        case OBJETIVO:
            if (IsKeyPressed(KEY_Q)) {
                currentScreen = TITLE; // Volta ao menu principal
            } else {
                exibirObjetivo(); // Função para desenhar a imagem do objetivo
            }
            break;
    }}

    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    CloseAudioDevice();

    finalizarMenu();
    return 0;
}
