#include "menu.h"
#include "game.h"
#include "lobby.h"
#include "ranking.h"

int main() {
    GameScreen currentScreen = TITLE;
    bool lobbyInitialized = false;
    bool gameInitialized = false;

    InitAudioDevice();
    Music titleMusic = LoadMusicStream("static/music/epicversion3.wav");
    PlayMusicStream(titleMusic);
    SetMusicVolume(titleMusic, 1.0f);

    iniciarMenu(&currentScreen);

    while (!WindowShouldClose()) {
    UpdateMusicStream(titleMusic);

        switch (currentScreen) {
            case TITLE:
                atualizarMenu(&currentScreen);
                desenharMenu(currentScreen);
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = NAME_INPUT;
                }
                break;
            case NAME_INPUT:
                recebeNomeDoPlayer(&currentScreen);
                break;
            case OBJETIVO:
                if (IsKeyPressed(KEY_Q)) {
                    currentScreen = TITLE;
                } else {
                    exibirObjetivo();
                }
                break;
            case CUTSCENE:
                cutsceneArrakis(titleMusic);
                currentScreen = LOBBY;
                break;
            case LOBBY:
                if (!lobbyInitialized) {
                    iniciarLobby();
                    lobbyInitialized = true;
                }
                processarEntradaLobby(&currentScreen, &lobbyInitialized);
                BeginDrawing();
                desenharLobbyDetalhado();
                EndDrawing();

                // Verifica se o currentScreen mudou para fora do lobby
                if (currentScreen != LOBBY && lobbyInitialized) {
                    finalizarLobby();
                    lobbyInitialized = false;
                }
                break;
            case GAME:
                if (!gameInitialized) {
                    iniciarGame();
                    gameInitialized = true;
                }

                playGame(&currentScreen);

                // Se o currentScreen mudou para fora do jogo, finalize os recursos
                if (currentScreen != GAME && gameInitialized) {
                    finalizarGame();
                    gameInitialized = false;
                }
                break;
            case RANKINGS:
                exibirRankingScreen(&currentScreen);
                break;
        }
    }

    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    CloseAudioDevice();

    finalizarMenu();
    return 0;
}
