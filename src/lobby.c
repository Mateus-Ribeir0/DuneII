#include "lobby.h"

int MAX_ESPECIARIAS = BOLSA_CAPACIDADE_PEQUENA;
Texture2D vendinha;

// Variável para controlar o estado de interação com o mercador
int isInteractingWithMerchant = 0;

const char* mensagem = NULL;

// Função para verificar se o jogador está próximo do mercador
int isPlayerNearMerchant() {
    return (player_x == MERCHANT_X && (player_y == MERCHANT_Y - 1 || player_y == MERCHANT_Y + 1)) ||
            (player_y == MERCHANT_Y && (player_x == MERCHANT_X - 1 || player_x == MERCHANT_X + 1)) ||
            (player_x == MERCHANT_X_LEFT && (player_y == MERCHANT_Y - 1 || player_y == MERCHANT_Y + 1)) ||
            (player_y == MERCHANT_Y && (player_x == MERCHANT_X_LEFT - 1 || player_x == MERCHANT_X_LEFT + 1));
}

void processarEntradaLobby(GameScreen *currentScreen, bool *lobbyInitialized) {
    mapaAtual = -1;  // Identifica que o jogador está no lobby
    int dx = 0, dy = 0;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) dx = 1;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) dx = -1;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) dy = -1;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) dy = 1;

    movePlayer(dx, dy);

    // Variável para controlar se o jogador está perto de um portal
    bool pertoDePortal = false;

    // Verifica se o jogador está em volta do portal para o mapa 1
    if ((player_x >= PORTAL_LOBBY_MAPA1_X - 1 && player_x <= PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA &&
         player_y >= PORTAL_LOBBY_MAPA1_Y - 1 && player_y <= PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA)) {
        
        mensagem = "Você deseja ir para o mapa 1? Pressione [P]";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 0;
            mensagem = NULL;  // Limpa a mensagem ao entrar no mapa
        }
    }
    // Verifica se o jogador está em volta do portal para o mapa 2
    else if ((player_x >= PORTAL_LOBBY_MAPA2_X - 1 && player_x <= PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA &&
              player_y >= PORTAL_LOBBY_MAPA2_Y - 1 && player_y <= PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA)) {
        
        mensagem = "Você deseja ir para o mapa 2? Pressione [P]";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 1;
            mensagem = NULL;  // Limpa a mensagem ao entrar no mapa
        }
    }
    // Verifica se o jogador está em volta do portal para o mapa 3
    else if ((player_x >= PORTAL_LOBBY_MAPA3_X - 1 && player_x <= PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA &&
              player_y >= PORTAL_LOBBY_MAPA3_Y - 1 && player_y <= PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA)) {
        
        mensagem = "Você deseja ir para o mapa 3? Pressione [P]";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 2;
            mensagem = NULL;  // Limpa a mensagem ao entrar no mapa
        }
    }

    // Se o jogador não estiver perto de nenhum portal, limpa a mensagem
    if (!pertoDePortal) {
        mensagem = NULL;
    }
}


void DrawDialogBox(const char *text, int posX, int posY, int width, int height, Color boxColor, Color textColor, bool isPortalDialog) {
    // Ajusta a largura para 600 quando a caixa é para um portal; caso contrário, mantém 800
    if (isPortalDialog) {
        width = 600;
    } else {
        width = 800;
    }

    DrawRectangleRounded((Rectangle){ posX, posY, width, height }, 0.1f, 16, boxColor);
    
    static int frameCount = 0;
    static int charactersToShow = 0;

    // Verifica se a tecla de espaço foi pressionada
    if (IsKeyPressed(KEY_SPACE)) {
        charactersToShow = strlen(text);  // Mostra o texto completo
    } else if (charactersToShow < strlen(text)) {
        frameCount++;
        charactersToShow = frameCount / 5;  // Velocidade do texto
        if (charactersToShow > strlen(text)) charactersToShow = strlen(text);
    }

    // Calcular o máximo de caracteres que cabem na caixa
    int maxCharsPerLine = (width - 20) / MeasureText("A", 20);  // Ajuste para cada linha
    int maxLines = (height - 20) / 20;                          // Ajuste para a altura da caixa
    int maxCharsInBox = maxCharsPerLine * maxLines;

    // Limita o texto ao máximo que cabe na caixa
    if (charactersToShow > maxCharsInBox) charactersToShow = maxCharsInBox;

    // Desenha o texto até o número atual de caracteres a serem mostrados
    DrawText(TextSubtext(text, 0, charactersToShow), posX + 10, posY + 10, 20, textColor);
}


// Variáveis globais para controlar o temporizador e exibição da mensagem de erro
double errorMessageTimer = 0.0;
bool showErrorMessage = false;
const double ERROR_MESSAGE_DURATION = 5.0;
const char *errorMessage = "";

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

    // Exibe a mensagem de erro se ela estiver ativa e aguarda 5 segundos
    if (showErrorMessage) {
        DrawDialogBox(errorMessage, 100, 500, 600, 120, WHITE, RED, false);
        if (GetTime() - errorMessageTimer >= ERROR_MESSAGE_DURATION) {
            showErrorMessage = false;  // Desativa a exibição da mensagem de erro após 5 segundos
            isInteractingWithMerchant = 0;  // Reseta a interação para permitir novas opções
        }
        return;  // Sai da função para manter a mensagem de erro na tela
    }

    // Verifica se o jogador está próximo do mercador e exibe a interação
    if (isPlayerNearMerchant()) {
        if (!isInteractingWithMerchant) {
            DrawDialogBox("Olá viajante, o que podemos negociar hoje?\n\n[1] para vender especiarias\n[2] para comprar uma bolsa nova", 100, 500, 800, 120, WHITE, BLACK, false);
            if (IsKeyPressed(KEY_ONE)) {
                isInteractingWithMerchant = 1;  // Opção de venda
            } else if (IsKeyPressed(KEY_TWO)) {
                isInteractingWithMerchant = 2;  // Opção de compra
            }
        } else {
            // Realiza ações com base na escolha do jogador
            if (isInteractingWithMerchant == 1) {
                if (itemsCollected > 0) {
                    playerMoney += itemsCollected * 300;
                    itemsCollected = 0;
                    DrawDialogBox("Obrigado pela venda, espero que prospere!", 100, 500, 800, 120, WHITE, BLACK, false);
                    if (IsKeyPressed(KEY_ENTER)) isInteractingWithMerchant = 0;
                } else {
                    errorMessage = "Saia daqui, você não tem nenhuma especiaria para negociar!";
                    showErrorMessage = true;
                    errorMessageTimer = GetTime();
                }
            } else if (isInteractingWithMerchant == 2) {
                DrawDialogBox("Qual delas deseja comprar?\n\n[1] Média (12 especiarias) - 5000\n[2] Grande (24 especiarias) - 10000\n[3] Super (32 especiarias) - 15000", 100, 500, 800, 150, WHITE, BLACK, false);

                if (IsKeyPressed(KEY_ONE)) {
                    if (playerMoney >= 5000) {
                        MAX_ESPECIARIAS = 12;
                        playerMoney -= 5000;
                        DrawDialogBox("Bolsa média adquirida!", 100, 500, 800, 120, WHITE, BLACK, false);
                        if (IsKeyPressed(KEY_ENTER)) isInteractingWithMerchant = 0;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra. Busque mais especiarias e vamos negociar...";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                } else if (IsKeyPressed(KEY_TWO)) {
                    if (playerMoney >= 10000) {
                        MAX_ESPECIARIAS = 24;
                        playerMoney -= 10000;
                        DrawDialogBox("Bolsa grande adquirida!", 100, 500, 800, 120, WHITE, BLACK, false);
                        if (IsKeyPressed(KEY_ENTER)) isInteractingWithMerchant = 0;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra. Busque mais especiarias e vamos negociar...";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                } else if (IsKeyPressed(KEY_THREE)) {
                    if (playerMoney >= 15000) {
                        MAX_ESPECIARIAS = 32;
                        playerMoney -= 15000;
                        DrawDialogBox("Bolsa super adquirida!", 100, 500, 800, 120, WHITE, BLACK, false);
                        if (IsKeyPressed(KEY_ENTER)) isInteractingWithMerchant = 0;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra.\nBusque mais especiarias e vamos negociar...";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                }
            }
        }
    } else if (mensagem != NULL) {
        DrawDialogBox(mensagem, 100, 500, 600, 120, WHITE, BLACK, true);
    } else {
        isInteractingWithMerchant = 0;
    }
}

void desenharLobbyDetalhado() {
    drawLobby();
}
