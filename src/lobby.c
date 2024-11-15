
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
static Texture2D bonesTexture;
static Texture2D monstersTexture;
static Sound troca;
static Music lobbyMusic; 

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
static float portalAnimationTimer = 0.0f;
static int portalFrameIndex = 0;
static bool isMusicPlaying = false;

bool isPlayerOnVendinha(int player_x, int player_y) {
    // Ajuste para a posição e tamanho corretos da vendinha
    int vendinha_x = 20;  // Baseado em posicaoVendinha.x
    int vendinha_y = 20;  // Baseado em posicaoVendinha.y
    int vendinha_largura = 123 * 0.8;  // Ajustado pelo fator de escala
    int vendinha_altura = 120 * 0.8;   // Ajustado pelo fator de escala

    return player_x >= vendinha_x && player_x < vendinha_x + vendinha_largura &&
           player_y >= vendinha_y && player_y < vendinha_y + vendinha_altura;
}


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
    monstersTexture = LoadTexture("static/image/monsters.png");
    bonesTexture = LoadTexture("static/image/bones.png");

    if (!isMusicPlaying) {
        lobbyMusic = LoadMusicStream("static/music/Musica_lobby.mp3");
        PlayMusicStream(lobbyMusic);
        SetMusicVolume(lobbyMusic, 2.0f);
        isMusicPlaying = true; 
    } else {
        ResumeMusicStream(lobbyMusic);
    }
}

void finalizarLobby() {
    if (isMusicPlaying) {
        PauseMusicStream(lobbyMusic);
    }

    UnloadTexture(velho);
    UnloadTexture(cityTexture);
    UnloadTexture(monstersTexture);
    UnloadTexture(bonesTexture);
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
    if (mapaAtual == -1) {
        if (new_x >= PORTAL_LOBBY_MAPA1_X && 
            new_x < PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA1_Y && 
            new_y < PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA) {
            return true;
        }

        if (new_x >= PORTAL_LOBBY_MAPA2_X && 
            new_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA2_Y && 
            new_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA) {
            return true;
        }

        if (new_x >= PORTAL_LOBBY_MAPA3_X && 
            new_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA &&
            new_y >= PORTAL_LOBBY_MAPA3_Y && 
            new_y < PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA) {
            return true;
        }

    } else {
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
    mapaAtual = -1;
    int dx = 0, dy = 0;

    updateWaterLevel(currentScreen);

    if (playerWater <= 0.0) {
        *currentScreen = RANKINGS;
        resetarJogo();
        return;
    }

    UpdateMusicStream(lobbyMusic);

    if (IsKeyPressed(KEY_D)) dx = 1;
    if (IsKeyPressed(KEY_A)) dx = -1;
    if (IsKeyPressed(KEY_W)) dy = -1;
    if (IsKeyPressed(KEY_S)) dy = 1;

    movePlayer(dx, dy);

    bool pertoDePortal = false;

    if ((player_x >= PORTAL_LOBBY_MAPA1_X - 1 && player_x < PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA1_Y - 1 && player_y < PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA + 1)) {
        
        mensagem = "Você deseja viajar para Zamirat?\nPressione [P]\n\nDificuldade: Fácil";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 0;
            mensagem = NULL;
        }
    }
    else if ((player_x >= PORTAL_LOBBY_MAPA2_X - 1 && player_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA2_Y - 1 && player_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA + 1)) {
        
        mensagem = "Você deseja viajar para Bashir'har?\nPressione [P]\n\nDificuldade: Média";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 1;
            mensagem = NULL;
        }
    }
    else if ((player_x >= PORTAL_LOBBY_MAPA3_X - 1 && player_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA + 1) &&
            (player_y >= PORTAL_LOBBY_MAPA3_Y - 1 && player_y < PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA + 1)) {
        
        mensagem = "Você deseja viajar para Qasr'Rahim?\nPressione [P]\n\nDificuldade: Difícil";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 2;
            mensagem = NULL;
        }
    }

    if (!pertoDePortal) {
        mensagem = NULL;
    }
}

void DrawDialogBox(const char *text, int posX, int posY, int width, int height, Color boxColor, Color textColor, bool isPortalDialog) {
    if (isPortalDialog) {
        width = 420;
    } else {
        width = 600;
    }

    DrawRectangleRounded((Rectangle){ posX, posY, width, height }, 0.1f, 16, boxColor);

    static int frameCount = 0;
    static int charactersToShow = 0;

    if (isPortalDialog) {
        charactersToShow = strlen(text);
    } else {
        if (IsKeyPressed(KEY_SPACE)) {
            charactersToShow = strlen(text);
        } else if (charactersToShow < strlen(text)) {
            frameCount++;
            charactersToShow = frameCount / 5; 
            if (charactersToShow > strlen(text)) charactersToShow = strlen(text);
        }
    }

    int maxCharsPerLine = (width - 20) / MeasureText("A", 20);  
    int maxLines = (height - 20) / 20;                          
    int maxCharsInBox = maxCharsPerLine * maxLines;

    if (charactersToShow > maxCharsInBox) charactersToShow = maxCharsInBox;

    DrawText(TextSubtext(text, 0, charactersToShow), posX + 10, posY + 10, 20, textColor);
}

void drawLobby() {
    bool soundPlayed = false;
    Rectangle tileSourceRec = { 128, 32, 32, 32 };
    Rectangle hitboxVendinha = { 1389, 330, 123, 120 };

    Vector2 posicaoVendinha = { 20, 20 };
    Rectangle destRecVendinha = { posicaoVendinha.x, posicaoVendinha.y, 123* 0.8 , 120* 0.8  }; 

    portalAnimationTimer += GetFrameTime();
    if (portalAnimationTimer >= 0.1f) {  // Ajuste a velocidade da animação aqui (0.1f é a velocidade em segundos por frame)
        portalAnimationTimer = 0.0f;
        portalFrameIndex++;
        if (portalFrameIndex > 3) {  // Como temos 4 frames (0, 1, 2, 3)
            portalFrameIndex = 0;
        }
    }


    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
            DrawTextureRec(desertTileset, tileSourceRec, tilePosition, WHITE);
        }
    }

    // Defina a região da imagem que deseja desenhar (ajuste as coordenadas conforme necessário)
    Rectangle sourceRecMonster = { 282, 13, 106, 116 };  // Exemplo de um monstro de 64x64 pixels na posição inicial da spritesheet

    // Defina a posição onde a imagem será exibida no lobby
    Vector2 positionMonster = { 180, 20 };  // Posição x e y no lobby, ajuste conforme desejado

    // Desenhe o monstro no lobby usando a função DrawTexturePro
    DrawTexturePro(monstersTexture, sourceRecMonster, 
                (Rectangle){ positionMonster.x, positionMonster.y, 106, 116 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);

    // Defina a região da imagem que deseja desenhar (ajuste as coordenadas conforme necessário)
    Rectangle sourceRecbones = { 557, 60, 45, 37 };  // Exemplo de um monstro de 64x64 pixels na posição inicial da spritesheet
    Rectangle sourceRecbones2 = { 557, 60, 45, 37 };
    Rectangle sourceRecbones3 = { 557, 60, 45, 37 };
    Rectangle sourceRecbones4 = { 557, 60, 45, 37 };

    Rectangle sourceRecbones5 = { 666, 125, 23, 19 };
    Rectangle sourceRecbones6 = { 666, 125, 23, 19 };
    Rectangle sourceRecbones7 = { 666, 125, 23, 19 };

    Rectangle sourceRecbones8 = { 784, 72, 29, 24 };
    Rectangle sourceRecbones9 = { 784, 72, 29, 24 };
    Rectangle sourceRecbones10 = { 181, 140, 43, 32 };

    // Defina a posição onde a imagem será exibida no lobby
    Vector2 positionBones = { 300, 50 };  // Posição x e y no lobby, ajuste conforme desejado
    Vector2 positionBones2 = { 660, 600 }; 
    Vector2 positionBones3 = { 200, 680 }; 
    Vector2 positionBones4 = {760, 300 }; 

    Vector2 positionBones5 = {730, 330 }; 
    Vector2 positionBones6 = {780, 543}; 
    Vector2 positionBones7 = {900, 203}; 

    Vector2 positionBones8 = {1164, 540}; 
    Vector2 positionBones9 = {370, 512}; 

    Vector2 positionBones10 = {560, 134}; 
    
    // Desenhe o monstro no lobby usando a função DrawTexturePro
    DrawTexturePro(bonesTexture, sourceRecbones, 
                (Rectangle){ positionBones.x, positionBones.y, 45, 37 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones2, 
                (Rectangle){ positionBones2.x, positionBones2.y, 45, 37 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones3, 
                (Rectangle){ positionBones3.x, positionBones3.y, 45, 37 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones4, 
                (Rectangle){ positionBones4.x, positionBones4.y, 45, 37 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);

    
    DrawTexturePro(bonesTexture, sourceRecbones5, 
                (Rectangle){ positionBones5.x, positionBones5.y, 23, 19 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);

    DrawTexturePro(bonesTexture, sourceRecbones6, 
                (Rectangle){ positionBones6.x, positionBones6.y, 23, 19 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones7, 
                (Rectangle){ positionBones7.x, positionBones7.y, 23, 19 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);


    DrawTexturePro(bonesTexture, sourceRecbones8, 
                (Rectangle){ positionBones8.x, positionBones8.y, 29, 24 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones9, 
                (Rectangle){ positionBones9.x, positionBones9.y, 29, 24 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);

    DrawTexturePro(bonesTexture, sourceRecbones10, 
                (Rectangle){ positionBones10.x, positionBones10.y, 43, 32 },  // Tamanho de destino
                (Vector2){ 0, 0 }, 0.0f, WHITE);

    DrawTexturePro(cityTexture, hitboxVendinha, destRecVendinha, (Vector2){0, 0}, 0.0f, WHITE);

    
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
    Rectangle sourceRec16 = { 102, 363, 123, 126 };
    Rectangle sourceRec17 = { 345, 369, 108, 126 };
    Rectangle sourceRec18 = { 1167, 108, 132, 120 };
    


    Vector2 positionCity1 = { 800, 4 };
    Vector2 positionCity2 = { 890, 36 }; 
    Vector2 positionCity3 = { 1000, 36 }; 
    Vector2 positionCity4 = { 1140, 270 };
    Vector2 positionCity5 = { 300, 380 };
    Vector2 positionCity6 = { 200, 376 }; 
    Vector2 positionCity7 = { 100, 354 };
    Vector2 positionCity8 = { 120, 600 }; 
    Vector2 positionCity9 = { 18, 478 };
    Vector2 positionCity10 = { 684, 2 };
    Vector2 positionCity11 = { 760, 200 };
    Vector2 positionCity12 = { 700, 180 };
    Vector2 positionCity13 = { 1000, 280 };
    Vector2 positionCity14 = { 1140, 150 }; 
    Vector2 positionCity15 = { 1, 630 };
    Vector2 positionCity16 = { 950, 400 };
    Vector2 positionCity17 = { 64, 280 };
    Vector2 positionCity18 = { 576, 38 }; 


    Rectangle destRec = { positionCity1.x, positionCity1.y, 108 * 0.8, 162 * 0.8 };
    Rectangle destRec2 = { positionCity2.x, positionCity2.y, 132 * 0.8, 120 * 0.8 };
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
    Rectangle destRec16 = { positionCity16.x, positionCity16.y, 123 * 0.6, 126 * 0.6};
    Rectangle destRec17 = { positionCity17.x, positionCity17.y, 108 * 0.6, 126 * 0.6};
    Rectangle destRec18 = { positionCity18.x, positionCity18.y, 132 * 0.8, 120 * 0.8 };

    static int lastDirection = 3;
    static float walkingTimer = 0.0f;
    static bool isWalking = false;

    if (IsKeyPressed(KEY_W)) {
        lastDirection = 1;
        isWalking = true;
        walkingTimer = 0.3f;
    } else if (IsKeyPressed(KEY_A)) {
        lastDirection = 2;
        isWalking = true;
        walkingTimer = 0.3f;
    } else if (IsKeyPressed(KEY_S)) {
        lastDirection = 3;
        isWalking = true;
        walkingTimer = 0.3f;
    } else if (IsKeyPressed(KEY_D)) {
        lastDirection = 4;
        isWalking = true;
        walkingTimer = 0.3f;
    }

    if (isWalking) {
        walkingTimer -= GetFrameTime();
        if (walkingTimer <= 0) {
            isWalking = false;
        }
    }

    Rectangle sourceRecPersonagem;
    switch (lastDirection) {
        case 1:
            sourceRecPersonagem = (Rectangle){0, 192, 64, 64};
            break;
        case 2: 
            sourceRecPersonagem = (Rectangle){0, 64, 64, 64};
            break;
        case 3:
            sourceRecPersonagem = (Rectangle){0, 0, 64, 64};
            break;
        case 4: 
            sourceRecPersonagem = (Rectangle){0, 128, 64, 64};
            break;
        default:
            sourceRecPersonagem = (Rectangle){0, 0, 64, 64};
            break;
    }

    Vector2 positionPersonagem = { player_x * TILE_SIZE, player_y * TILE_SIZE };

    Rectangle destRecPersonagem = { positionPersonagem.x - 32, positionPersonagem.y - 32, 96, 96 };

    if (isWalking) {
        DrawTexturePro(personagemAndando, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawTexturePro(personagem, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
    }

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
    DrawTexturePro(cityTexture, sourceRec16, destRec16, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec17, destRec17, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(cityTexture, sourceRec18, destRec18, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle portalSourceRec = { 32 * portalFrameIndex, 0, 32, 32 };
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

    DrawTexturePro(portal, portalSourceRec, portalDestRec1, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec2, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec3, origin, 0.0f, WHITE);

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
    // Configurações do sprite do mercador
    Rectangle sourceRecVelho = { 62, 54, 509, 485 };
    Rectangle sourceRecVelhoPuto = { 1084, 108, 510, 484 };
    Rectangle destRecVelho = { 150, 350, 256, 256 };
    Vector2 originVelho = { 32, 32 };

    // Sempre desenha o sprite do mercador
    DrawTexturePro(
        velho, 
        (isInteractingWithMerchant == 2) ? sourceRecVelhoPuto : sourceRecVelho, 
        destRecVelho, originVelho, 0.0f, WHITE
    );

    if (!isInteractingWithMerchant) {
        // Diálogo inicial
        DrawDialogBox("Olá viajante, o que podemos negociar hoje?\n\n[1] para vender especiarias\n[2] para comprar uma bolsa nova\n[3] para comprar garrafa de água",
                      100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

        if (IsKeyPressed(KEY_ONE)) isInteractingWithMerchant = 1;
        else if (IsKeyPressed(KEY_TWO)) isInteractingWithMerchant = 2;
        else if (IsKeyPressed(KEY_THREE)) isInteractingWithMerchant = 3;
    } else {
        // Lógica de interação
        switch (isInteractingWithMerchant) {
            case 1: // Vender especiarias
            static int merchantMessage = 0; // Variável auxiliar para rastrear a mensagem a ser exibida

            // Lógica de venda de especiarias
            if (merchantMessage == 0) { // Verifica se a interação acabou de começar
                if (itemsCollected > 0) {
                    playerMoney += itemsCollected * 300; // Calcula o dinheiro recebido
                    itemsCollected = 0; // Reseta as especiarias coletadas
                    merchantMessage = 1; // Define a mensagem de sucesso
                } else {
                    merchantMessage = -1; // Define a mensagem de erro
                }
            }

            // Exibe a mensagem apropriada
            if (merchantMessage == 1) {
                DrawDialogBox("Obrigado pela venda, espero que prospere!", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
            } else if (merchantMessage == -1) {
                DrawDialogBox("Saia daqui, você não tem nenhuma especiaria para negociar!", 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
            }

            // Aguarda o jogador pressionar qualquer tecla ou WASD para retornar ao menu anterior
            if (GetKeyPressed() != 0 || 
                IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D)) {
                isInteractingWithMerchant = 0; // Sai da interação com o mercador
                merchantMessage = 0; // Reseta a variável para a próxima interação
            }
            break;


            case 2: // Comprar bolsa
                DrawDialogBox("Qual bolsa deseja comprar?\n\n[1] Média (12 especiarias) - 5000\n[2] Grande (24 especiarias) - 10000\n[3] Super (32 especiarias) - 15000",
                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

                if (IsKeyPressed(KEY_ONE)) {
                    if (playerMoney >= 5000) {
                        MAX_ESPECIARIAS = 12;
                        playerMoney -= 5000;
                        DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa.", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    } else {
                        // Mensagem para aguardar a ação do usuário
                        while (!IsKeyPressed(KEY_ENTER)) {
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.", 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing(); // Finaliza o frame atual
                            BeginDrawing(); // Inicia um novo frame
                        }
                        isInteractingWithMerchant = 0; // Finaliza interação após tecla pressionada
                    }
                } else if (IsKeyPressed(KEY_TWO)) {
                    if (playerMoney >= 10000) {
                        MAX_ESPECIARIAS = 24;
                        playerMoney -= 10000;
                        DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa.", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    } else {
                        while (!IsKeyPressed(KEY_ENTER)) {
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.", 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                            BeginDrawing();
                        }
                        isInteractingWithMerchant = 0;
                    }
                } else if (IsKeyPressed(KEY_THREE)) {
                    if (playerMoney >= 15000) {
                        MAX_ESPECIARIAS = 32;
                        playerMoney -= 15000;
                        DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa.", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    } else {
                        while (!IsKeyPressed(KEY_ENTER)) {
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.", 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                            BeginDrawing();
                        }
                        isInteractingWithMerchant = 0;
                    }
                }
                if (IsKeyPressed(KEY_ENTER)) isInteractingWithMerchant = 0;
                break;

            case 3: // Comprar água
                DrawDialogBox("Qual garrafa de água deseja comprar?\n\n[1] Pequena (10%) - 3000\n[2] Média (20%) - 5000\n[3] Grande (30%) - 7000",
                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

                if (IsKeyPressed(KEY_ONE)) {
                    if (playerMoney >= 3000) {
                        playerWater = fmin(playerWater + GARRAFA_PEQUENA_CAPACIDADE, 100);
                        playerMoney -= 3000;
                        DrawDialogBox("Obrigado pela compra! Aproveite sua água.", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    } else {
                        // Mensagem para aguardar a ação do usuário
                        while (!IsKeyPressed(KEY_ENTER)) {
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.", 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                            BeginDrawing();
                        }
                        isInteractingWithMerchant = 0; // Finaliza interação após tecla pressionada
                    }
                } else if (IsKeyPressed(KEY_TWO)) {
                    if (playerMoney >= 5000) {
                        playerWater = fmin(playerWater + GARRAFA_MEDIA_CAPACIDADE, 100);
                        playerMoney -= 5000;
                        DrawDialogBox("Obrigado pela compra! Aproveite sua água.", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    } else {
                        while (!IsKeyPressed(KEY_ENTER)) {
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.", 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                            BeginDrawing();
                        }
                        isInteractingWithMerchant = 0;
                    }
                } else if (IsKeyPressed(KEY_THREE)) {
                    if (playerMoney >= 7000) {
                        playerWater = fmin(playerWater + GARRAFA_GRANDE_CAPACIDADE, 100);
                        playerMoney -= 7000;
                        DrawDialogBox("Obrigado pela compra! Aproveite sua água.", 100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    } else {
                        while (!IsKeyPressed(KEY_ENTER)) {
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.", 100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                            BeginDrawing();
                        }
                        isInteractingWithMerchant = 0;
                    }
                }
                if (IsKeyPressed(KEY_ENTER)) isInteractingWithMerchant = 0;
                break;

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