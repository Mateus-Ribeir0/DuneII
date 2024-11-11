
#include "game.h"  // Inclui a declaração de funções de game.c
#include "lobby.h"
#include "game.h"  // Inclui a declaração de funções de game.c
#include <math.h>  // Para usar a função fmin

void updateWaterLevel(GameScreen *currentScreen);
void resetarJogo();

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

// Função para verificar se o jogador está na posição de um portal, tanto no lobby quanto nos mapas
bool isPlayerOnPortal(int new_x, int new_y, int mapaAtual) {
    if (mapaAtual == -1) { // No lobby
        // Verifica o portal do mapa 1
        if (new_x >= PORTAL_LOBBY_MAPA1_X && new_x < PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA1_Y && new_y < PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA) {
            return true;
        }
        // Verifica o portal do mapa 2
        if (new_x >= PORTAL_LOBBY_MAPA2_X && new_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA2_Y && new_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA) {
            return true;
        }
        // Verifica o portal do mapa 3
        if (new_x >= PORTAL_LOBBY_MAPA3_X && new_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA3_Y && new_y < PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA) {
            return true;
        }
    } else { // Nos mapas, verifica o portal de retorno ao lobby
        if (new_x >= PORTAL_RETORNO_X && new_x < PORTAL_RETORNO_X + PORTAL_RETORNO_LARGURA &&
            new_y >= PORTAL_RETORNO_Y && new_y < PORTAL_RETORNO_Y + PORTAL_RETORNO_ALTURA) {
            return true;
        }
    }
    return false;
}



void processarEntradaLobby(GameScreen *currentScreen, bool *lobbyInitialized) {
    mapaAtual = -1;  // Identifica que o jogador está no lobby
    int dx = 0, dy = 0;

    // Atualiza o nível de água no lobby
    updateWaterLevel(currentScreen);

    // Verifica se o jogador ficou sem água e redireciona para o ranking
    if (playerWater <= 0.0) {
        *currentScreen = RANKINGS;
        resetarJogo();
        return;
    }

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
        
        mensagem = "Você deseja viajar para o Zamirat?\nPressione [P]\n\nDificuldade: ***";
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
        
        mensagem = "Você deseja viajar para o Bashir'har?\nPressione [P]\n\nDificuldade: ****";
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
        
        mensagem = "Você deseja viajar para o Qasr'Rahim?\nPressione [P]\n\nDificuldade: *****";
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
    // Ajusta a largura para 400 quando a caixa é para um portal; caso contrário, mantém 600
    if (isPortalDialog) {
        width = 420;
    } else {
        width = 600;
    }

    DrawRectangleRounded((Rectangle){ posX, posY, width, height }, 0.1f, 16, boxColor);

    static int frameCount = 0;
    static int charactersToShow = 0;

    // Verifica se o diálogo é de um portal
    if (isPortalDialog) {
        // Exibe o texto completo imediatamente
        charactersToShow = strlen(text);
    } else {
        // Controle de exibição gradual para outros diálogos
        if (IsKeyPressed(KEY_SPACE)) {
            charactersToShow = strlen(text);  // Mostra o texto completo
        } else if (charactersToShow < strlen(text)) {
            frameCount++;
            charactersToShow = frameCount / 5;  // Velocidade do texto
            if (charactersToShow > strlen(text)) charactersToShow = strlen(text);
        }
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
bool showThankYouMessage = false;
const double MESSAGE_DURATION = 5.0;
const char *errorMessage = "";
const int widthMercador = 620;
const int heigthMercador = 140;

void drawLobby() {
    ClearBackground((Color){195, 160, 81, 255});  // Define um fundo claro para o lobby

    Texture2D vendinha = LoadTexture("static/image/market_assets.png");

    Rectangle hitboxVendinha = {96, 0, 90, 96};
    Vector2 posicaoVendinha = {20, 20};

    // Desenha o fundo do lobby com tiles de areia
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){195, 160, 81, 255});
        }
    }

    // Desenha a textura da vendinha
    DrawTextureRec(vendinha, hitboxVendinha, posicaoVendinha, WHITE);
    DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);

    // Carrega e desenha a textura do portal nas posições especificadas dos portais do lobby com 3x3 tiles (96x96 pixels)
    Texture2D portal = LoadTexture("static/image/portal.png");

    // Define o recorte na sprite sheet: começa em (0, 0) e tem tamanho 96x96
    Rectangle portalSourceRec = { 0, 0, 32, 32};

    // Define a área de destino onde o portal será desenhado, com escala de 3.0
    Rectangle portalDestRec1 = { PORTAL_LOBBY_MAPA1_X * TILE_SIZE, PORTAL_LOBBY_MAPA1_Y * TILE_SIZE, 32 * 3, 32 * 3 };
    Rectangle portalDestRec2 = { PORTAL_LOBBY_MAPA2_X * TILE_SIZE, PORTAL_LOBBY_MAPA2_Y * TILE_SIZE, 32 * 3, 32 * 3 };
    Rectangle portalDestRec3 = { PORTAL_LOBBY_MAPA3_X * TILE_SIZE, PORTAL_LOBBY_MAPA3_Y * TILE_SIZE, 32 * 3, 32 * 3 };

    // Ponto de origem (pivot) para a rotação, definindo como o canto superior esquerdo
    Vector2 origin = { 0, 0 };

    // Desenha o portal recortado e escalado em 3.0 nas posições dos portais no mapa do lobby
    DrawTexturePro(portal, portalSourceRec, portalDestRec1, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec2, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec3, origin, 0.0f, WHITE);

    // Atualiza e exibe informações da bolsa e status do jogador
    DrawText(TextFormat("Especiarias na bolsa: %d/%d", itemsCollected, MAX_ESPECIARIAS), 10, 10, 20, BLACK);
    DrawText(TextFormat("Dinheiro: %d", playerMoney), 10, 40, 20, BLACK);
    DrawText(TextFormat("Nível de Água: %.0f%%", playerWater), 10, 70, 20, BLUE);

    // Verifica interações com o mercador e exibe diálogos/mensagens
    if (!isPlayerNearMerchant() && showErrorMessage) {
        showErrorMessage = false;
        isInteractingWithMerchant = 0;
    } else if (showErrorMessage && (GetTime() - errorMessageTimer >= MESSAGE_DURATION)) {
        showErrorMessage = false;
        isInteractingWithMerchant = 0;
    }

    if (showErrorMessage) {
        DrawDialogBox(errorMessage, 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
        return;
    }

    if (showThankYouMessage) {
        DrawDialogBox("Obrigado pela venda, espero que prospere!", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
        if (GetTime() - errorMessageTimer >= MESSAGE_DURATION) {
            showThankYouMessage = false;
            isInteractingWithMerchant = 0;
        }
        return;
    }

    // Exibe mensagem de negociação com o mercador se o jogador estiver próximo
    if (isPlayerNearMerchant()) {
        if (!isInteractingWithMerchant) {
            DrawDialogBox("Olá viajante, o que podemos negociar hoje?\n\n[1] para vender especiarias\n[2] para comprar uma bolsa nova\n[3] para comprar garrafa de água", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
            if (IsKeyPressed(KEY_ONE)) {
                isInteractingWithMerchant = 1;  // Opção de venda
            } else if (IsKeyPressed(KEY_TWO)) {
                isInteractingWithMerchant = 2;  // Opção de compra de bolsa
            } else if (IsKeyPressed(KEY_THREE)) {
                isInteractingWithMerchant = 3;  // Opção de compra de garrafa de água
            }
        } else {
            // Lógica de interação com o mercador para venda de especiarias, compra de bolsa e garrafa de água
            if (isInteractingWithMerchant == 1) {
                if (itemsCollected > 0) {
                    playerMoney += itemsCollected * 300;
                    itemsCollected = 0;
                    showThankYouMessage = true;
                    errorMessageTimer = GetTime();
                } else {
                    errorMessage = "Saia daqui, você não tem nenhuma especiaria para negociar!";
                    showErrorMessage = true;
                    errorMessageTimer = GetTime();
                }
            } else if (isInteractingWithMerchant == 2) {
                DrawDialogBox("Qual bolsa deseja comprar?\n\n[1] Média (12 especiarias) - 5000\n[2] Grande (24 especiarias) - 10000\n[3] Super (32 especiarias) - 15000", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

                if (IsKeyPressed(KEY_ONE)) {
                    if (playerMoney >= 5000) {
                        MAX_ESPECIARIAS = 12;
                        playerMoney -= 5000;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra.";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                } else if (IsKeyPressed(KEY_TWO)) {
                    if (playerMoney >= 10000) {
                        MAX_ESPECIARIAS = 24;
                        playerMoney -= 10000;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra.";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                } else if (IsKeyPressed(KEY_THREE)) {
                    if (playerMoney >= 15000) {
                        MAX_ESPECIARIAS = 32;
                        playerMoney -= 15000;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra.";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                }
            } else if (isInteractingWithMerchant == 3) {
                DrawDialogBox("Qual garrafa de água deseja comprar?\n\n[1] Pequena (10%) - 3000\n[2] Média (20%) - 5000\n[3] Grande (30%) - 7000", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

                if (IsKeyPressed(KEY_ONE)) {
                    if (playerMoney >= 3000) {
                        playerWater = fmin(playerWater + GARRAFA_PEQUENA_CAPACIDADE, 100);
                        playerMoney -= 3000;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra.";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                } else if (IsKeyPressed(KEY_TWO)) {
                    if (playerMoney >= 5000) {
                        playerWater = fmin(playerWater + GARRAFA_MEDIA_CAPACIDADE, 100);
                        playerMoney -= 5000;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra.";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                } else if (IsKeyPressed(KEY_THREE)) {
                    if (playerMoney >= 7000) {
                        playerWater = fmin(playerWater + GARRAFA_GRANDE_CAPACIDADE, 100);
                        playerMoney -= 7000;
                    } else {
                        errorMessage = "Você não tem dinheiro suficiente para essa compra.";
                        showErrorMessage = true;
                        errorMessageTimer = GetTime();
                    }
                }
            }
        }
    } else if (mensagem != NULL) {
        DrawDialogBox(mensagem, 70, 580, 400, 110, WHITE, BLACK, true);
    } else {
        isInteractingWithMerchant = 0;
    }
}





void desenharLobbyDetalhado() {
    drawLobby();
}