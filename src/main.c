#include "menu.h"
#include "game.h"
#include "lobby.h"
#include "ranking.h"

int main() {
    GameScreen currentScreen = TITLE;
    bool lobbyInitialized = false;
    bool gameInitialized = false;

    InitAudioDevice();
    iniciarMenu(&currentScreen);

    while (!WindowShouldClose()) {
        switch (currentScreen) {
            case TITLE:
                atualizarMenu(&currentScreen);
                drawMenu();
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = NAME_INPUT;
                }
                break;
            case NAME_INPUT:
                recebeNomeDoPlayer(&currentScreen);
                break;
            case OBJETIVO:
                exibirObjetivo(&currentScreen);
                break;
            case CUTSCENE:
                cutsceneArrakis();
                currentScreen = LOBBY;
                break;
            case LOBBY:
                if (!lobbyInitialized) {
                    iniciarLobby();
                    lobbyInitialized = true;
                }

                processarEntradaLobby(&currentScreen);

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

                if (currentScreen != GAME && gameInitialized) {
                    finalizarGame();
                    gameInitialized = false;
                }
                break;
            case RANKINGS:
                exibirRankingScreen(&currentScreen);
                break;
            case EMPTY_SCREEN:
                processarTelaVazia(&currentScreen);
                break;
        }
    }

    CloseAudioDevice();
    finalizarMenu();
    return 0;
}
