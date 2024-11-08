#include "lobby.h"

int MAX_ESPECIARIAS = BOLSA_CAPACIDADE_PEQUENA;
Texture2D vendinha;

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
    ClearBackground((Color){195, 160, 81, 255});  // Define um fundo claro para o lobby

    vendinha = LoadTexture("static/image/market_assets.png");

    Rectangle hitboxVendinha = {96, 0, 90, 96};
    Vector2 posicaoVendinha = {20, 20};

    // Desenha o mapa do lobby
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){195, 160, 81, 255});
        }
    }

    DrawTextureRec(vendinha, hitboxVendinha, posicaoVendinha, WHITE);

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
        if (!isInteractingWithMerchant) {
            // Exibe as opções de interação com o mercador
            DrawText("Deseja:", 10, 100, 20, BLACK);
            DrawText("1- Vender especiarias", 10, 130, 20, BLACK);
            DrawText("2- Comprar bolsa maior", 10, 160, 20, BLACK);
            DrawText("3- Sair", 10, 190, 20, BLACK);

            // Detecta a entrada do jogador
            if (IsKeyPressed(KEY_ONE)) {
                isInteractingWithMerchant = 1;  // Opção de venda
            } else if (IsKeyPressed(KEY_TWO)) {
                isInteractingWithMerchant = 2;  // Opção de compra
            } else if (IsKeyPressed(KEY_THREE)) {
                isInteractingWithMerchant = 3;  // Sair
            }
        } else {
            // Realiza ações com base na escolha do jogador
            if (isInteractingWithMerchant == 1) {
                // Venda de especiarias
                playerMoney += itemsCollected * 300;
                itemsCollected = 0;
                DrawText("Especiarias vendidas!", 10, 220, 20, BLACK);
            } else if (isInteractingWithMerchant == 2) {
                // Exibe opções de compra de bolsas
                DrawText("Escolha a bolsa:", 10, 220, 20, BLACK);
                DrawText("1- Média (12 especiarias) - 5000", 10, 250, 20, BLACK);
                DrawText("2- Grande (24 especiarias) - 8000", 10, 280, 20, BLACK);
                DrawText("3- Super (32 especiarias) - 12000", 10, 310, 20, BLACK);

                // Processa a escolha do jogador para compra de bolsa
                if (IsKeyPressed(KEY_ONE) && playerMoney >= PRECO_BOLSA_MEDIA) {
                    MAX_ESPECIARIAS = BOLSA_CAPACIDADE_MEDIA;
                    playerMoney -= PRECO_BOLSA_MEDIA;
                    DrawText("Bolsa média adquirida!", 10, 340, 20, BLACK);
                } else if (IsKeyPressed(KEY_TWO) && playerMoney >= PRECO_BOLSA_GRANDE) {
                    MAX_ESPECIARIAS = BOLSA_CAPACIDADE_GRANDE;
                    playerMoney -= PRECO_BOLSA_GRANDE;
                    DrawText("Bolsa grande adquirida!", 10, 340, 20, BLACK);
                } else if (IsKeyPressed(KEY_THREE) && playerMoney >= PRECO_BOLSA_SUPER) {
                    MAX_ESPECIARIAS = BOLSA_CAPACIDADE_SUPER;
                    playerMoney -= PRECO_BOLSA_SUPER;
                    DrawText("Bolsa super adquirida!", 10, 340, 20, BLACK);
                } else if ((IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE)) && playerMoney < PRECO_BOLSA_MEDIA) {
                    DrawText("Dinheiro insuficiente!", 10, 340, 20, RED);
                }
            } else if (isInteractingWithMerchant == 3) {
                // Finaliza a interação com o mercador
                DrawText("Até a próxima!", 10, 220, 20, BLACK);
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
