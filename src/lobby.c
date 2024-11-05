#include "lobby.h"

void initializeLobby() {
    // Adicione outras inicializações, como NPCs e interações
}

void atualizarLobby() {
    // Lógica de atualização contínua do lobby, se necessário
}

void processarEntradaLobby(GameScreen *currentScreen, bool *lobbyInitialized) {
    int dx = 0, dy = 0;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) dx = 1;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) dx = -1;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) dy = -1;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) dy = 1;

    // Seleção de mapa no lobby
    if (IsKeyPressed(KEY_ONE)) mapaAtual = 0;
    if (IsKeyPressed(KEY_TWO)) mapaAtual = 1;
    if (IsKeyPressed(KEY_THREE)) mapaAtual = 2;

    movePlayer(dx, dy);

    if (IsKeyPressed(KEY_ENTER)) {
        *currentScreen = GAME;
        *lobbyInitialized = false;
    }
}

void drawLobby() {
    ClearBackground(WHITE);

    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, WHITE);
        }
    }

    // Desenha o jogador no mapa do lobby
    DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
}

void desenharLobbyDetalhado() {
    drawLobby();
    DrawText("Pressione 1, 2 ou 3 para selecionar o mapa", 10, 10, 20, BLACK);
}