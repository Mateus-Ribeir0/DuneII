
#include "lobby.h"
#include <math.h>

// Variáveis de textura e som declaradas como static
static Texture2D velho;
static Texture2D desertTileset;
static Texture2D personagem;
static Texture2D personagemAndando;
static Texture2D portal;
static Texture2D cerealsTexture;
static Texture2D goldTexture;
static Texture2D aguaTexture;
static Texture2D cityTexture;
static Sound troca;

// Variáveis Globias
int isInteractingWithMerchant = 0;
int MAX_ESPECIARIAS = BOLSA_CAPACIDADE_PEQUENA;
const char* mensagem = NULL;
double errorMessageTimer = 0.0;
bool showErrorMessage = false;
bool showThankYouMessage = false;
const double MESSAGE_DURATION = 5.0;
const char *errorMessage = "";
const int widthMercador = 620;
const int heigthMercador = 140;

void iniciarLobby() {
    velho = LoadTexture("static/image/velho.png");
    cityTexture = LoadTexture("static/image/city.png");
    desertTileset = LoadTexture("static/image/environment.png");
    personagem = LoadTexture("static/image/newstoppedsprites.png");
    personagemAndando = LoadTexture("static/image/newwalkingsprites.png");
    portal = LoadTexture("static/image/portal.png");
    cerealsTexture = LoadTexture("static/image/Cereals.png");
    goldTexture = LoadTexture("static/image/gold.png");
    aguaTexture = LoadTexture("static/image/agua.png");
    troca = LoadSound("static/music/trocaDeDinheiro.wav");
}

void finalizarLobby() {
    UnloadTexture(velho);
    UnloadTexture(cityTexture);
    UnloadTexture(desertTileset);
    UnloadTexture(personagem);
    UnloadTexture(personagemAndando);
    UnloadTexture(portal);
    UnloadTexture(cerealsTexture);
    UnloadTexture(goldTexture);
    UnloadTexture(aguaTexture);
    UnloadSound(troca);
}

int isPlayerNearMerchant() {
    return (player_x == MERCHANT_X && (player_y == MERCHANT_Y - 1 || player_y == MERCHANT_Y + 1)) ||
            (player_y == MERCHANT_Y && (player_x == MERCHANT_X - 1 || player_x == MERCHANT_X + 1)) ||
            (player_x == MERCHANT_X_LEFT && (player_y == MERCHANT_Y - 1 || player_y == MERCHANT_Y + 1)) ||
            (player_y == MERCHANT_Y && (player_x == MERCHANT_X_LEFT - 1 || player_x == MERCHANT_X_LEFT + 1));
}

bool isPlayerOnPortal(int new_x, int new_y, int mapaAtual) {
    if (mapaAtual == -1) { // No lobby
        // Verifica o portal do mapa 1 no lobby
        if (new_x >= PORTAL_LOBBY_MAPA1_X && 
            new_x < PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA1_Y && 
            new_y < PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA) {
            return true;
        }

        // Verifica o portal do mapa 2 no lobby
        if (new_x >= PORTAL_LOBBY_MAPA2_X && 
            new_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA2_Y && 
            new_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA) {
            return true;
        }

        // Verifica o portal do mapa 3 no lobby
        if (new_x >= PORTAL_LOBBY_MAPA3_X && 
            new_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA3_Y && 
            new_y < PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA) {
            return true;
        }

    } else { // Nos mapas, verifica o portal de retorno ao lobby
        if (new_x >= PORTAL_RETORNO_X && 
            new_x < PORTAL_RETORNO_X + PORTAL_RETORNO_LARGURA &&
            new_y >= PORTAL_RETORNO_Y && 
            new_y < PORTAL_RETORNO_Y + PORTAL_RETORNO_ALTURA) {
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

    // Verifica proximidade com o portal para o mapa 1 com uma linha extra abaixo
    if ((player_x >= PORTAL_LOBBY_MAPA1_X - 1 && player_x < PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA1_Y - 1 && player_y < PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA + 1)) {
        
        mensagem = "Você deseja viajar para Zamirat?\nPressione [P]\n\nDificuldade: ***";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 0;
            mensagem = NULL;  // Limpa a mensagem ao entrar no mapa
        }
    }
    // Verifica proximidade com o portal para o mapa 2
    else if ((player_x >= PORTAL_LOBBY_MAPA2_X - 1 && player_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA2_Y - 1 && player_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA + 1)) {
        
        mensagem = "Você deseja viajar para Bashir'har?\nPressione [P]\n\nDificuldade: ****";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 1;
            mensagem = NULL;  // Limpa a mensagem ao entrar no mapa
        }
    }
    // Verifica proximidade com o portal para o mapa 3
    else if ((player_x >= PORTAL_LOBBY_MAPA3_X - 1 && player_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA + 1) &&
            (player_y >= PORTAL_LOBBY_MAPA3_Y - 1 && player_y < PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA + 1)) {
        
        mensagem = "Você deseja viajar para Qasr'Rahim?\nPressione [P]\n\nDificuldade: *****";
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

void drawLobby() {
    bool soundPlayed = false;
    Rectangle tileSourceRec = { 128, 32, 32, 32 };
    // Define a área de recorte da nova sprite da vendinha
    Rectangle hitboxVendinha = { 1389, 330, 123, 120 };

    // Define a posição e o tamanho de destino onde deseja desenhar a nova sprite no lobby
    Vector2 posicaoVendinha = { 20, 20 };
    Rectangle destRecVendinha = { posicaoVendinha.x, posicaoVendinha.y, 123* 0.8 , 120* 0.8  }; // Tamanho da vendinha conforme especificado

    // Desenha o fundo do lobby com tiles de areia
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
            DrawTextureRec(desertTileset, tileSourceRec, tilePosition, WHITE);
        }
    }

    // Desenha a nova textura da vendinha
    DrawTexturePro(cityTexture, hitboxVendinha, destRecVendinha, (Vector2){0, 0}, 0.0f, WHITE);

    
    // Define a área de recorte da sprite `cityTexture`
        // Define as áreas de recorte das sprites `cityTexture`
    Rectangle sourceRec = { 831, 66, 108, 162 };
    Rectangle sourceRec2 = { 1167, 108, 132, 120 };
    Rectangle sourceRec3 = { 978, 102, 150, 126 };
    Rectangle sourceRec4 = { 582, 354, 84, 135 };
    Rectangle sourceRec5 = { 1068, 309, 90, 138 };
    Rectangle sourceRec6 = { 936, 303, 96, 144 };
    Rectangle sourceRec7 = { 801, 279, 96, 168 };
    Rectangle sourceRec8 = { 120, 261, 81, 51 };
    Rectangle sourceRec9 = { 459, 240, 84, 72 };
    Rectangle sourceRec10 = { 1338, 60, 138, 168 };
    Rectangle sourceRec11 = { 216, 246, 90, 63 };
    Rectangle sourceRec12 = { 222, 147, 33, 51 };
    Rectangle sourceRec13 = { 222, 147, 33, 51 };
    Rectangle sourceRec14 = { 324, 264, 126, 48 };
    Rectangle sourceRec15 = { 102, 363, 123, 126 };
    


    // Define as posições onde deseja desenhar as sprites `cityTexture` no lobby
    Vector2 positionCity1 = { 800, 4 }; // Posição da primeira sprite
    Vector2 positionCity2 = { 890, 36 }; // Posição da segunda sprite
    Vector2 positionCity3 = { 1000, 36 }; // Posição da segunda sprite
    Vector2 positionCity4 = { 1140, 270 }; // Posição da segunda sprite
    Vector2 positionCity5 = { 300, 380 }; // Posição da segunda sprite
    Vector2 positionCity6 = { 200, 376 }; // Posição da segunda sprite
    Vector2 positionCity7 = { 100, 354 }; // Posição da segunda sprite
    Vector2 positionCity8 = { 120, 600 }; // Posição da segunda sprite
    Vector2 positionCity9 = { 18, 478 }; // Posição da segunda sprite
    Vector2 positionCity10 = { 684, 2 }; // Posição da segunda sprite
    Vector2 positionCity11 = { 760, 200 }; // Posição da segunda sprite
    Vector2 positionCity12 = { 700, 180 }; // Posição da segunda sprite
    Vector2 positionCity13 = { 1000, 280 }; // Posição da segunda sprite
    Vector2 positionCity14 = { 1140, 150 }; // Posição da segunda sprite
    Vector2 positionCity15 = { 1, 630 }; // Posição da segunda sprite


    // Define os retângulos de destino com escala reduzida
    Rectangle destRec = { positionCity1.x, positionCity1.y, 108 * 0.8, 162 * 0.8 };  // Reduz a escala da primeira sprite em 80%
    Rectangle destRec2 = { positionCity2.x, positionCity2.y, 132 * 0.8, 120 * 0.8 }; // Reduz a escala da segunda sprite em 80%
    Rectangle destRec3 = { positionCity3.x, positionCity3.y, 150 * 0.8, 126 * 0.8 };
    Rectangle destRec4 = { positionCity4.x, positionCity4.y, 84 * 0.5, 135 * 0.5 };
    Rectangle destRec5 = { positionCity5.x, positionCity5.y, 90 , 138 };
    Rectangle destRec6 = { positionCity6.x, positionCity6.y, 96 , 144 };
    Rectangle destRec7 = { positionCity7.x, positionCity7.y, 96 , 168 };
    Rectangle destRec8 = { positionCity8.x, positionCity8.y, 81 * 0.8, 51 *0.8};
    Rectangle destRec9 = { positionCity9.x, positionCity9.y, 81 , 51};
    Rectangle destRec10 = { positionCity10.x, positionCity10.y, 138 * 0.8, 168 * 0.8};
    Rectangle destRec11 = { positionCity11.x, positionCity11.y, 90 * 0.7, 63 * 0.7};
    Rectangle destRec12 = { positionCity12.x, positionCity12.y, 33 * 0.5, 51 * 0.5};
    Rectangle destRec13 = { positionCity13.x, positionCity13.y, 33 * 0.5, 51 * 0.5};
    Rectangle destRec14 = { positionCity14.x, positionCity14.y, 126 * 0.8, 48 * 0.8};
    Rectangle destRec15 = { positionCity15.x, positionCity15.y, 123 * 0.6, 126 * 0.6};

    // Desenha as sprites `cityTexture` no lobby
    DrawTexturePro(cityTexture, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec2, destRec2, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec3, destRec3, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec4, destRec4, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec5, destRec5, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec6, destRec6, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec7, destRec7, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec8, destRec8, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec9, destRec9, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec10, destRec10, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec11, destRec11, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec12, destRec12, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec13, destRec13, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec14, destRec14, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec15, destRec15, (Vector2){0, 0}, 0.0f, WHITE);

    // Atualize a direção com base na tecla pressionada
    static int lastDirection = 3;  // Começa apontando para "baixo" (S)
    static float walkingTimer = 0.0f; // Temporizador para a animação de "andando"
    static bool isWalking = false;    // Controle se está na animação de "andando"

    // Detecta qual tecla foi pressionada e atualiza a direção e o temporizador
    if (IsKeyPressed(KEY_W)) {
        lastDirection = 1;  // Cima
        isWalking = true;
        walkingTimer = 0.3f; // Iniciar o temporizador
    } else if (IsKeyPressed(KEY_A)) {
        lastDirection = 2;  // Esquerda
        isWalking = true;
        walkingTimer = 0.3f;
    } else if (IsKeyPressed(KEY_S)) {
        lastDirection = 3;  // Baixo
        isWalking = true;
        walkingTimer = 0.3f;
    } else if (IsKeyPressed(KEY_D)) {
        lastDirection = 4;  // Direita
        isWalking = true;
        walkingTimer = 0.3f;
    }

    // Atualiza o temporizador se estiver "andando"
    if (isWalking) {
        walkingTimer -= GetFrameTime();
        if (walkingTimer <= 0) {
            isWalking = false;  // Termina a animação de "andando"
        }
    }

    // Usa switch para definir o retângulo de origem com base na última direção
    Rectangle sourceRecPersonagem;
    switch (lastDirection) {
        case 1: // Cima
            sourceRecPersonagem = (Rectangle){0, 192, 64, 64};
            break;
        case 2: // Esquerda
            sourceRecPersonagem = (Rectangle){0, 64, 64, 64};
            break;
        case 3: // Baixo
            sourceRecPersonagem = (Rectangle){0, 0, 64, 64};
            break;
        case 4: // Direita
            sourceRecPersonagem = (Rectangle){0, 128, 64, 64};
            break;
        default:
            sourceRecPersonagem = (Rectangle){0, 0, 64, 64}; // Posição padrão (parado)
            break;
    }

    // Define a posição do personagem na tela
    Vector2 positionPersonagem = { player_x * TILE_SIZE, player_y * TILE_SIZE };

    // Desenha o sprite redimensionado para 96x96, centralizando sobre a posição de 32x32
    Rectangle destRecPersonagem = { positionPersonagem.x - 32, positionPersonagem.y - 32, 96, 96 };

    // Se estiver "andando", desenha o sprite de "andando"; caso contrário, desenha o sprite de direção final
    if (isWalking) {
        DrawTexturePro(personagemAndando, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawTexturePro(personagem, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
    }

    // Carrega e desenha a textura do portal nas posições especificadas dos portais do lobby
    Rectangle portalSourceRec = { 0, 0, 32, 32 };
    Rectangle portalDestRec1 = { 
        PORTAL_LOBBY_MAPA1_X * TILE_SIZE, 
        PORTAL_LOBBY_MAPA1_Y * TILE_SIZE, 
        TILE_SIZE * PORTAL_HORIZONTAL_LARGURA,  
        TILE_SIZE * PORTAL_HORIZONTAL_ALTURA 
    };

    Rectangle portalDestRec2 = { 
        PORTAL_LOBBY_MAPA2_X * TILE_SIZE, 
        PORTAL_LOBBY_MAPA2_Y * TILE_SIZE, 
        TILE_SIZE * PORTAL_VERTICAL_LARGURA,  
        TILE_SIZE * PORTAL_VERTICAL_ALTURA 
    };

    Rectangle portalDestRec3 = { 
        PORTAL_LOBBY_MAPA3_X * TILE_SIZE, 
        PORTAL_LOBBY_MAPA3_Y * TILE_SIZE, 
        TILE_SIZE * PORTAL_HORIZONTAL_LARGURA,  
        TILE_SIZE * PORTAL_HORIZONTAL_ALTURA 
    };

    Vector2 origin = { 0, 0 };

    // Desenha os portais
    DrawTexturePro(portal, portalSourceRec, portalDestRec1, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec2, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec3, origin, 0.0f, WHITE);

    // Define a área de informações no canto direito da tela
    int infoBoxX = SCREEN_WIDTH - 230;
    int infoBoxY = 10;
    int infoBoxWidth = 220;
    int infoBoxHeight = 100;

    Color fillColor = (Color){205, 133, 63, 255};
    Color borderColor = (Color){101, 67, 33, 255};

    DrawRectangleRounded((Rectangle){infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight}, 0.1f, 16, fillColor);
    DrawRectangleRoundedLines((Rectangle){infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight}, 0.1f, 16, borderColor);
    DrawRectangleRoundedLines((Rectangle){infoBoxX + 1, infoBoxY + 1, infoBoxWidth - 2, infoBoxHeight - 2}, 0.1f, 16, borderColor);
    DrawRectangleRoundedLines((Rectangle){infoBoxX + 2, infoBoxY + 2, infoBoxWidth - 4, infoBoxHeight - 4}, 0.1f, 16, borderColor);

    Vector2 especiariaIconPos = { infoBoxX + 10, infoBoxY + 1 };
    DrawTextureRec(cerealsTexture, (Rectangle){64, 64, 32, 32}, especiariaIconPos, WHITE);
    DrawText("Especiarias:", infoBoxX + 50, infoBoxY + 10, 18, WHITE);
    DrawText(TextFormat("%d/%d", itemsCollected, MAX_ESPECIARIAS), infoBoxX + 160, infoBoxY + 10, 18, WHITE);

    Vector2 goldIconPos = { infoBoxX + 15, infoBoxY + 35 };
    Rectangle goldSourceRec = { 0, 0, 16, 16 };
    Rectangle goldDestRec = { goldIconPos.x, goldIconPos.y, 24, 24 };

    DrawTexturePro(goldTexture, goldSourceRec, goldDestRec, origin, 0.0f, WHITE);
    DrawText(TextFormat("Dinheiro: %d", playerMoney), infoBoxX + 50, infoBoxY + 40, 18, WHITE);

    Vector2 aguaIconPos = { infoBoxX + 18, infoBoxY + 65 };
    Rectangle aguaSourceRec = { 0, 0, aguaTexture.width, aguaTexture.height };
    Rectangle aguaDestRec = { aguaIconPos.x, aguaIconPos.y, 24, 24 };

    DrawTexturePro(aguaTexture, aguaSourceRec, aguaDestRec, origin, 0.0f, WHITE);
    DrawText(TextFormat("Água: %.0f%%", playerWater), infoBoxX + 55, infoBoxY + 70, 18, WHITE);

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
        if (!soundPlayed) {
            PlaySound(troca);
            soundPlayed = true;
        }
        DrawDialogBox("Obrigado pela venda, espero que prospere!", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

        if (GetTime() - errorMessageTimer >= MESSAGE_DURATION) {
            showThankYouMessage = false;
            isInteractingWithMerchant = 0;
            soundPlayed = false;
        }
        return;
    }

    if (!showThankYouMessage) {
        soundPlayed = false;
    }

    if (isPlayerNearMerchant()) {
        if (!isInteractingWithMerchant) {
            Rectangle sourceRecVelho = { 0, 0, 550, 550 };
            Rectangle destRecVelho = { 105, 350, 256, 256 };
            Vector2 originVelho = { 32, 32 };

            DrawTexturePro(velho, sourceRecVelho, destRecVelho, originVelho, 0.0f, WHITE);
            DrawDialogBox("Olá viajante, o que podemos negociar hoje?\n\n[1] para vender especiarias\n[2] para comprar uma bolsa nova\n[3] para comprar garrafa de água", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
            
            if (IsKeyPressed(KEY_ONE)) {
                isInteractingWithMerchant = 1;
            } else if (IsKeyPressed(KEY_TWO)) {
                isInteractingWithMerchant = 2;
            } else if (IsKeyPressed(KEY_THREE)) {
                isInteractingWithMerchant = 3;
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