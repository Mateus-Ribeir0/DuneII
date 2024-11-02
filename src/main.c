#include "raylib.h"
#include "menu.h"
#include "game.h"

int main() {
    GameScreen currentScreen = TITLE;
    int dificuldade = 5;  // Valor padrão para a dificuldade
    bool lobbyInitialized = false;  // Variável para controle de inicialização do lobby

    InitAudioDevice();
    Music titleMusic = LoadMusicStream("static/music/epicversion3.wav");
    PlayMusicStream(titleMusic);

    iniciarMenu(&currentScreen, &dificuldade);

    while (!WindowShouldClose()) {
        UpdateMusicStream(titleMusic);

        switch (currentScreen) {
            case TITLE:
            case RANKINGS:
                atualizarMenu(&currentScreen, &dificuldade);
                desenharMenu(currentScreen);
                if (IsKeyPressed(KEY_ENTER) && currentScreen == TITLE) {
                    currentScreen = CUTSCENE;
                }
                break;
            case CUTSCENE:
                cutsceneArrakis(titleMusic);
                currentScreen = LOBBY;  // Transição para o lobby após a cutscene
                break;
            case LOBBY:
                if (!lobbyInitialized) {
                    initializeLobby();
                    lobbyInitialized = true;  // Marca que o lobby foi inicializado
                }

                BeginDrawing();
                drawLobby();
                EndDrawing();

                // Processamento da entrada do usuário para movimentação baseada em tile
                int dx = 0, dy = 0;
                if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) dx = 1;
                if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) dx = -1;
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) dy = -1;
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) dy = 1;

                movePlayer(dx, dy);  // Assumindo que esta função aceita movimentação baseada em tile

                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = GAME;  // Transição para o estado do jogo
                    lobbyInitialized = false;  // Reseta para garantir re-inicialização na próxima visita ao lobby
                }
                break;
            case GAME:
                playGame(dificuldade);
                currentScreen = TITLE;  // Retorna ao título após o jogo
                break;
        }
    }

    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    CloseAudioDevice();

    finalizarMenu();
    return 0;
}
