#include "lobby.h"

// Variável para controlar o estado de interação com o mercador
int isInteractingWithMerchant = 0;

const char* mensagem = NULL;

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
    mapaAtual = -1;  // Identifica que o jogador está no lobby
    limparColisoesEZonas();
    int dx = 0, dy = 0;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) dx = 1;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) dx = -1;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) dy = -1;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) dy = 1;

    movePlayer(dx, dy);
  
    // Verifica se o jogador está em volta do portal para o mapa 1
    if ((player_x >= PORTAL_LOBBY_MAPA1_X - 1 && player_x <= PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA &&
         player_y >= PORTAL_LOBBY_MAPA1_Y - 1 && player_y <= PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA)) {
        
        mensagem = "Você deseja ir para o mapa 1? Pressione [P]";
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 0;
        }
    }
    // Verifica se o jogador está em volta do portal para o mapa 2
    else if ((player_x >= PORTAL_LOBBY_MAPA2_X - 1 && player_x <= PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA &&
              player_y >= PORTAL_LOBBY_MAPA2_Y - 1 && player_y <= PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA)) {
        
        mensagem = "Você deseja ir para o mapa 2? Pressione [P]";
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 1;
        }
    }
    // Verifica se o jogador está em volta do portal para o mapa 3
    else if ((player_x >= PORTAL_LOBBY_MAPA3_X - 1 && player_x <= PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA &&
              player_y >= PORTAL_LOBBY_MAPA3_Y - 1 && player_y <= PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA)) {
        
        mensagem = "Você deseja ir para o mapa 3? Pressione [P]";
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 2;
        }
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

    // Desenha os portais
    DrawRectangle(PORTAL_LOBBY_MAPA1_X * TILE_SIZE, PORTAL_LOBBY_MAPA1_Y * TILE_SIZE,
                  TILE_SIZE * PORTAL_HORIZONTAL_LARGURA, TILE_SIZE * PORTAL_HORIZONTAL_ALTURA, ORANGE);

    DrawRectangle(PORTAL_LOBBY_MAPA2_X * TILE_SIZE, PORTAL_LOBBY_MAPA2_Y * TILE_SIZE,
                  TILE_SIZE * PORTAL_VERTICAL_LARGURA, TILE_SIZE * PORTAL_VERTICAL_ALTURA, ORANGE);

    DrawRectangle(PORTAL_LOBBY_MAPA3_X * TILE_SIZE, PORTAL_LOBBY_MAPA3_Y * TILE_SIZE,
                  TILE_SIZE * PORTAL_HORIZONTAL_LARGURA, TILE_SIZE * PORTAL_HORIZONTAL_ALTURA, ORANGE);

    // Mostra a quantidade de especiarias coletadas no lobby
    DrawText(TextFormat("Especiarias na bolsa: %d/%d", itemsCollected, MAX_ESPECIARIAS), 10, 10, 20, BLACK);
    DrawText(TextFormat("Dinheiro: %d", playerMoney), 10, 40, 20, BLACK);

    // Desenha o mercador (quadradinho roxo) na posição (5, 5)
    DrawRectangle(MERCHANT_X * TILE_SIZE, MERCHANT_Y * TILE_SIZE, TILE_SIZE, TILE_SIZE, PURPLE);

    // Verifica se o jogador está próximo do mercador
    if (isPlayerNearMerchant()) {
        // Exibe a mensagem inicial de interação se o jogador não tiver iniciado a interação
        if (!isInteractingWithMerchant) {
            DrawText("Deseja vender suas especiarias?", 10, 100, 20, BLACK);
            DrawText("1- Sim", 10, 130, 20, BLACK);
            DrawText("2- Nao", 10, 160, 20, BLACK);

            // Detecta a entrada do jogador
            if (IsKeyPressed(KEY_ONE)) {
                isInteractingWithMerchant = 1;  // Estado para a resposta "Sim"
            } else if (IsKeyPressed(KEY_TWO)) {
                isInteractingWithMerchant = 2;  // Estado para a resposta "Não"
            }
        } else {
            // Exibe a resposta do mercador dependendo da escolha do jogador
            if (isInteractingWithMerchant == 1) {
                // Calcula o valor da venda e atualiza o dinheiro do jogador
                playerMoney += itemsCollected * 300;  // Multiplica o valor por 3
                itemsCollected = 0;  // Zera as especiarias na bolsa
                DrawText("Obrigado, até a próxima.", 10, 190, 20, BLACK);
            } else if (isInteractingWithMerchant == 2) {
                DrawText("Volte quando precisar de mim.", 10, 190, 20, BLACK);
            }
        }
    } else {
        // Reseta a interação quando o jogador se afasta do mercador
        isInteractingWithMerchant = 0;
    }

    // Exibe a mensagem no centro da tela, se o jogador estiver em volta de um portal
    if (mensagem != NULL) {
        int screenWidth = GetScreenWidth();
        int textWidth = MeasureText(mensagem, 20);
        int xPosition = (screenWidth - textWidth) / 2;
        DrawText(mensagem, xPosition, GetScreenHeight() / 2, 20, BLACK);
    }
    mensagem = NULL;  
}

void desenharLobbyDetalhado() {
    drawLobby();
}
