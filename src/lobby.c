#include "lobby.h"

// Variável para controlar o estado de interação com o mercador
int isInteractingWithMerchant = 0;

// Função para verificar se o jogador está próximo do mercador
int isPlayerNearMerchant() {
    return (player_x == MERCHANT_X && (player_y == MERCHANT_Y - 1 || player_y == MERCHANT_Y + 1)) ||
           (player_y == MERCHANT_Y && (player_x == MERCHANT_X - 1 || player_x == MERCHANT_X + 1));
}

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
    if (IsKeyPressed(KEY_SEVEN)) mapaAtual = 0;
    if (IsKeyPressed(KEY_EIGHT)) mapaAtual = 1;
    if (IsKeyPressed(KEY_NINE)) mapaAtual = 2;

    movePlayer(dx, dy);

    if (IsKeyPressed(KEY_ENTER)) {
        *currentScreen = GAME;
        *lobbyInitialized = false;
    }
}

void drawLobby() {
    ClearBackground(LIGHTGRAY);  // Define um fundo claro para o lobby

    // Desenha o mapa do lobby
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
        }
    }

    // Desenha o jogador no mapa do lobby
    DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);

    // Desenha o mercador (quadradinho roxo) na posição (5, 5)
    DrawRectangle(MERCHANT_X * TILE_SIZE, MERCHANT_Y * TILE_SIZE, TILE_SIZE, TILE_SIZE, PURPLE);

    // Verifica se o jogador está próximo do mercador
    if (isPlayerNearMerchant() == 1) {
        // Exibe a mensagem inicial de interação se o jogador não tiver iniciado a interação
        if (!isInteractingWithMerchant) {
            DrawText("Deseja vender suas especiarias?", 10, 10, 20, BLACK);
            DrawText("1- Sim", 10, 40, 20, BLACK);
            DrawText("2- Nao", 10, 70, 20, BLACK);

            // Detecta a entrada do jogador
            if (IsKeyPressed(KEY_ONE)) {
                isInteractingWithMerchant = 1;  // Estado para a resposta "Sim"
            } else if (IsKeyPressed(KEY_TWO)) {
                isInteractingWithMerchant = 2;  // Estado para a resposta "Não"
            }
        } else {
            // Exibe a resposta do mercador dependendo da escolha do jogador
            if (isInteractingWithMerchant == 1) {
                DrawText("Obrigado, ate a proxima.", 10, 100, 20, BLACK);
            } else if (isInteractingWithMerchant == 2) {
                DrawText("Volte quando precisar de mim.", 10, 100, 20, BLACK);
            }
        }
    } else {
        // Reseta a interação quando o jogador se afasta do mercador
        isInteractingWithMerchant = 0;
    }
}

void desenharLobbyDetalhado() {
    drawLobby();

    // Calcula a posição X para alinhar o texto à direita
    int screenWidth = GetScreenWidth();
    int textWidth = MeasureText("Pressione 7, 8 ou 9 para selecionar o mapa", 20);
    int xPosition = screenWidth - textWidth - 10;  // Alinha à direita com margem de 10 pixels

    // Desenha o texto na posição calculada à direita da tela
    DrawText("Pressione 7, 8 ou 9 para selecionar o mapa", xPosition, 10, 20, BLACK);
}
