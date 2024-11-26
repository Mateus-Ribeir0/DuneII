#include "lobby.h"

static Texture2D personagemAndando;
static Texture2D monstersTexture;
static Texture2D cerealsTexture;
static Texture2D desertTileset;
static Texture2D bonesTexture;
static Texture2D aguaTexture;


static Texture2D cityTexture;
static Texture2D goldTexture;
static Texture2D luckyTexture;
static Texture2D personagem;
static Texture2D portal;
static Texture2D sombra;
static Music lobbyMusic; 
static Texture2D velho;
static Sound troca;
static Texture2D spaceshipTexture;
static Music warMusic; 


int MAX_ESPECIARIAS = BOLSA_CAPACIDADE_PEQUENA;
static float portalAnimationTimer = 0.0f;
const double MESSAGE_DURATION = 5.0;
static bool isMusicPlaying = false;
int isInteractingWithMerchant = 0;
bool showThankYouMessage = false;
static int portalFrameIndex = 0;
double errorMessageTimer = 0.0;
const int heigthMercador = 150;
bool showErrorMessage = false;
const char *errorMessage = "";
static int easterEggCount = 0;
const int widthMercador = 620;
const char* mensagem = NULL;
static bool spaceshipAnimationPlayed = false;
static float spaceshipPositionX = -128;
static bool isInExtendedLobby = false;
static bool isWarMusicPlaying = false;

void iniciarLobby() {
    velho = LoadTexture("static/image/velho.png");
    cityTexture = LoadTexture("static/image/city.png");
    desertTileset = LoadTexture("static/image/environment.png");
    personagem = LoadTexture("static/image/newstoppedsprites.png");
    personagemAndando = LoadTexture("static/image/newwalkingsprites.png");
    portal = LoadTexture("static/image/portal.png");
    cerealsTexture = LoadTexture("static/image/Cereals.png");
    goldTexture = LoadTexture("static/image/gold.png");
    luckyTexture = LoadTexture("static/image/lucky.png");
    aguaTexture = LoadTexture("static/image/agua.png");
    troca = LoadSound("static/music/trocaDeDinheiro.wav");
    monstersTexture = LoadTexture("static/image/monsters.png");
    bonesTexture = LoadTexture("static/image/bones.png");
    sombra = LoadTexture("static/image/sombras.png");
    spaceshipTexture = LoadTexture("static/image/spaceships.png");
    warMusic = LoadMusicStream("static/music/warMusic.wav");

    if (!isMusicPlaying) {
        lobbyMusic = LoadMusicStream("static/music/Musica_lobby.mp3");
        PlayMusicStream(lobbyMusic);
        isMusicPlaying = true; 
    } else {
        ResumeMusicStream(lobbyMusic);
    }
}

void finalizarLobby() {
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
    UnloadTexture(luckyTexture);
    UnloadTexture(aguaTexture);
    UnloadSound(troca);
    UnloadTexture(spaceshipTexture);
    UnloadMusicStream(warMusic);
    
    if (isMusicPlaying) {
        PauseMusicStream(lobbyMusic);
    }
}

int isPlayerNearMerchant() {
    return (player_x == MERCHANT_X && (player_y == MERCHANT_Y - 1 || player_y == MERCHANT_Y + 1)) ||
            (player_y == MERCHANT_Y && (player_x == MERCHANT_X - 1 || player_x == MERCHANT_X + 1)) ||
            (player_x == MERCHANT_X_LEFT && (player_y == MERCHANT_Y - 1 || player_y == MERCHANT_Y + 1)) ||
            (player_y == MERCHANT_Y && (player_x == MERCHANT_X_LEFT - 1 || player_x == MERCHANT_X_LEFT + 1));
}

void DrawDialogBox(const char *text, int posX, int posY, int width, int height, Color boxColor, Color textColor, bool isPortalDialog) {
    if (isPortalDialog) width = 420;
    else width = 600; 

    DrawRectangleRounded((Rectangle){ posX, posY, width, height }, 0.1f, 16, boxColor);

    int maxCharsPerLine = (width - 20) / MeasureText("A", 20);  
    int maxLines = (height - 20) / 20;                          
    int maxCharsInBox = maxCharsPerLine * maxLines;

    DrawText(TextSubtext(text, 0, maxCharsInBox), posX + 10, posY + 10, 20, textColor);
}

void processarEntradaLobby(GameScreen *currentScreen) {
    mapaAtual = -1;
    bool pertoDePortal = false;
    int dx = 0, dy = 0;

    static bool firstEntry = true;

    // Mover para a posição correta ao retornar da tela vazia
    if (!firstEntry) {
        firstEntry = true;
    } else if (player_y >= 0) {
        firstEntry = false;
    }

    updateWaterLevel(currentScreen);
    UpdateMusicStream(lobbyMusic);

    if (IsKeyPressed(KEY_D)) dx = 1;
    if (IsKeyPressed(KEY_A)) dx = -1;
    if (IsKeyPressed(KEY_W)) dy = -1;
    if (IsKeyPressed(KEY_S)) dy = 1;

    movePlayer(dx, dy);

    // Verificar se o jogador está na borda direita
    if (player_x >= (MAPA_LARGURA - 1) && spaceshipAnimationPlayed) {
        if (dx == 1) { // Pressionou 'D'
            *currentScreen = EMPTY_SCREEN; // Transição para a tela vazia
            return;
        }
    }

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

    if ((player_x >= PORTAL_LOBBY_MAPA2_X - 1 && player_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA2_Y - 1 && player_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA + 1)) {
        
        mensagem = "Você deseja viajar para Bashir'har?\nPressione [P]\n\nDificuldade: Média";
        pertoDePortal = true;
        if (IsKeyPressed(KEY_P)) {
            *currentScreen = GAME;
            mapaAtual = 1;
            mensagem = NULL;
        }
    }

    if ((player_x >= PORTAL_LOBBY_MAPA3_X - 1 && player_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA + 1) &&
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

    if (player_x == 0 && player_y == MAPA_ALTURA - 1) {
        if (IsKeyPressed(KEY_M)) {
            easterEggCount++;
            if (easterEggCount >= 10) {
                playerMoney = 99999;
                easterEggCount = 0;
            }
        }
    } else {
        easterEggCount = 0;
    }

    if (playerWater <= 0.0) {
        *currentScreen = RANKINGS;
        resetarJogo();
        return;
    }
    verificarProximidadePoco(player_x * TILE_SIZE, player_y * TILE_SIZE);
    drawLobby();
}
static double lastWaterAttemptTime = 0; // Marca o tempo da última tentativa

void tentarRecuperarAguaNoPoco() {
    double currentTime = GetTime();
    static double lastWaterAttemptTime = -120.0; // Tempo da última tentativa (-120 para permitir a primeira tentativa)
    static bool lastAttemptSuccessful = true;   // Indica se a última tentativa foi bem-sucedida

    // Atualizar o tempo da nova tentativa
    lastWaterAttemptTime = currentTime;

    // Chance e resultado
    int chance = GetRandomValue(0, 99); // Chance baseada na sorte
    int aguaRecuperada = 0;
    bool sucesso = false;
    char sucessoMensagem[128];

    // Determinar sucesso com base na sorte
    if (playerLucky == 20 && chance < 20) {
        sucesso = true;
        aguaRecuperada = GetRandomValue(1, 5);
    } else if (playerLucky == 30 && chance < 30) {
        sucesso = true;
        aguaRecuperada = GetRandomValue(2, 6);
    } else if (playerLucky == 50 && chance < 50) {
        sucesso = true;
        aguaRecuperada = GetRandomValue(3, 8);
    }

    // Processar sucesso ou falha
    if (sucesso) {
        playerWater = fmin(playerWater + aguaRecuperada, 100.0); // Evitar ultrapassar 100%
        snprintf(sucessoMensagem, sizeof(sucessoMensagem), 
                 "Parabéns! Você recuperou %d%% de água do poço.", aguaRecuperada);
        DrawDialogBox(sucessoMensagem, 100, 550, 600, 100, WHITE, GREEN, false);
        lastAttemptSuccessful = true; // Marca a última tentativa como bem-sucedida
    } else {
        DrawDialogBox("Você não conseguiu dessa vez, tente na próxima...", 100, 550, 600, 100, WHITE, RED, false);
        lastAttemptSuccessful = false; // Marca a última tentativa como falha
    }

    // Manter a mensagem até o jogador pressionar ENTER
    while (!IsKeyPressed(KEY_ENTER)) {
        BeginDrawing();
        UpdateMusicStream(lobbyMusic);
        if (sucesso) {
            DrawDialogBox(sucessoMensagem, 100, 550, 600, 100, WHITE, GREEN, false);
        } else {
            DrawDialogBox("Você não conseguiu dessa vez, tente na próxima...", 100, 550, 600, 100, WHITE, RED, false);
        }
        EndDrawing();
    }
}

void verificarProximidadePoco(float playerX, float playerY) {
    const float areas[][4] = {
        {720, 160, 80, 80}, // Coordenadas do primeiro poço
        {100, 550, 80, 80}  // Coordenadas do segundo poço
    };
    const int numAreas = sizeof(areas) / sizeof(areas[0]);
    static double lastWaterAttemptTime = -120.0; // Tempo da última tentativa (-120 para permitir a primeira tentativa)
    static bool lastAttemptSuccessful = true;   // Indica se a última tentativa foi bem-sucedida

    for (int i = 0; i < numAreas; i++) {
        float areaX = areas[i][0];
        float areaY = areas[i][1];
        float areaWidth = areas[i][2];
        float areaHeight = areas[i][3];

        if (playerX >= areaX - 10 && playerX <= areaX + areaWidth + 10 &&
            playerY >= areaY - 10 && playerY <= areaY + areaHeight + 10) {

            // Se a última tentativa foi falha e ainda está no intervalo de 2 minutos
            if (!lastAttemptSuccessful && GetTime() - lastWaterAttemptTime < 120.0) {
                DrawText("Ainda não é possível tentar, volte depois.", 100, 550, 20, RED);
                return;
            }

            // Exibir a mensagem para tentar recuperar água
            DrawDialogBox("Você deseja tentar recuperar água do poço?\n[1] Sim  [2] Não", 100, 550, 600, 100, WHITE, BLACK, false);

            if (IsKeyPressed(KEY_ONE)) {
                tentarRecuperarAguaNoPoco(); // Chama a função de tentativa
            } else if (IsKeyPressed(KEY_TWO)) {
                DrawDialogBox("Você decidiu não tentar agora.", 100, 550, 600, 100, WHITE, GRAY, false);
                while (!IsKeyPressed(KEY_ENTER)) {
                    BeginDrawing();
                    UpdateMusicStream(lobbyMusic);
                    DrawDialogBox("Você decidiu não tentar agora.", 100, 550, 600, 100, WHITE, GRAY, false);
                    EndDrawing();
                }
            }

            return;
        }
    }
}


void processarTelaVazia(GameScreen *currentScreen) {
    static Texture2D empty_personagem;
    static Texture2D empty_sombra;
    static Texture2D empty_desertTileset;
    static Texture2D empty_villain;
    static Texture2D villain_sombra;
    static Texture2D villain_portrait;
    static Texture2D warningSign;
    static Music windMusic;
    static Music battleMusic;
    static Sound machineLoadingSound;
    static Sound typingSound;

    static bool initialized = false;
    static double startTime = 0;
    static bool dialogTriggered = false;
    static bool windMusicStarted = false;
    static bool battleMusicStarted = false;
    static bool soundPlayed = false;

    static int empty_player_x = 1;
    static int empty_player_y = 10;
    static int lastDirection = 3; // 1 = cima, 2 = esquerda, 3 = baixo, 4 = direita
    static bool isWalking = false;
    static float walkingTimer = 0.0f;

    static const char *dialogTexts[] = {
        "Você não deveria estar aqui...",
        "O príncipe de Arrakis querendo atrapalhar meus planos?",
        "Sua terra não sobreviverá por muito tempo.",
        "A água que Arrakis carrega é fundamental pra prosperidade da nossa civilização de Zarnax.",
        "A guerra pela água já começou, se prepare pra ver essa região sob nosso domínio."
    };
    static const int totalDialogTexts = sizeof(dialogTexts) / sizeof(dialogTexts[0]);
    static int currentDialogIndex = 0;
    static char displayedText[256] = "";
    static int displayedTextLength = 0;
    static double lastTextEndTime = 0;
    static bool hideDialog = false;

    if (!initialized) {
        empty_personagem = LoadTexture("static/image/newstoppedsprites.png");
        empty_sombra = LoadTexture("static/image/sombras.png");
        empty_desertTileset = LoadTexture("static/image/environment.png");
        empty_villain = LoadTexture("static/image/villain.png");
        villain_sombra = LoadTexture("static/image/sombras.png");
        villain_portrait = LoadTexture("static/image/villainPortrait.png");
        warningSign = LoadTexture("static/image/WarningSign05.png");
        windMusic = LoadMusicStream("static/music/wind.wav");
        battleMusic = LoadMusicStream("static/music/battleMusic.wav");
        machineLoadingSound = LoadSound("static/music/machineLoading.wav");
        typingSound = LoadSound("static/music/falas.wav");

        SetSoundVolume(machineLoadingSound, 1.0f);
        SetSoundVolume(typingSound, 0.8f);

        startTime = GetTime();
        initialized = true;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // Desenhar o mapa
    Rectangle tileSourceRec = { 128, 32, 32, 32 };
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
            DrawTextureRec(empty_desertTileset, tileSourceRec, tilePosition, WHITE);
        }
    }

    // Atualizar movimento do personagem
    int dx = 0, dy = 0;

    if (hideDialog && battleMusicStarted) {
        if (IsKeyPressed(KEY_D)) {
            dx = 1;
            lastDirection = 4; // Direita
            isWalking = true;
            walkingTimer = 0.3f;
        }
        if (IsKeyPressed(KEY_A)) {
            dx = -1;
            lastDirection = 2; // Esquerda
            isWalking = true;
            walkingTimer = 0.3f;
        }
        if (IsKeyPressed(KEY_W)) {
            dy = -1;
            lastDirection = 1; // Cima
            isWalking = true;
            walkingTimer = 0.3f;
        }
        if (IsKeyPressed(KEY_S)) {
            dy = 1;
            lastDirection = 3; // Baixo
            isWalking = true;
            walkingTimer = 0.3f;
        }

        if (!(dx == -1 && empty_player_x == 0)) {
            empty_player_x += dx;
            empty_player_y += dy;

            empty_player_x = (empty_player_x < 0) ? 0 : empty_player_x;
            empty_player_x = (empty_player_x >= MAPA_LARGURA) ? MAPA_LARGURA - 1 : empty_player_x;
            empty_player_y = (empty_player_y < 0) ? 0 : empty_player_y;
            empty_player_y = (empty_player_y >= MAPA_ALTURA) ? MAPA_ALTURA - 1 : empty_player_y;
        }
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

    Vector2 positionPersonagem = { empty_player_x * TILE_SIZE, empty_player_y * TILE_SIZE };
    Rectangle destRecPersonagem = { positionPersonagem.x - 32, positionPersonagem.y - 32, 96, 96 };

    DrawTexturePro(empty_sombra, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(empty_personagem, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle sourceRecVillain = { 0, 64, 64, 64 };
    Vector2 villainPosition = { (MAPA_LARGURA - 3) * TILE_SIZE, 10 * TILE_SIZE + 8 };
    Rectangle destRecVillain = { villainPosition.x - 48, villainPosition.y - 48, 96, 96 };

    DrawTexturePro(villain_sombra, sourceRecVillain, destRecVillain, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(empty_villain, sourceRecVillain, destRecVillain, (Vector2){0, 0}, 0.0f, WHITE);

    if (GetTime() - startTime >= 3.0 && !hideDialog) {
        dialogTriggered = true;

        Rectangle villainPortraitDest = { SCREEN_WIDTH - 338, SCREEN_HEIGHT - 418, 288, 288 };
        DrawTexturePro(villain_portrait, (Rectangle){0, 0, villain_portrait.width, villain_portrait.height},
                       villainPortraitDest, (Vector2){0, 0}, 0.0f, WHITE);

        DrawRectangleRounded((Rectangle){ 50, SCREEN_HEIGHT - 150, SCREEN_WIDTH - 100, 100 }, 0.1f, 16, (Color){ 0, 0, 0, 200 });
        DrawRectangleRoundedLines((Rectangle){ 50, SCREEN_HEIGHT - 150, SCREEN_WIDTH - 100, 100 }, 0.1f, 16, WHITE);

        if (displayedTextLength < strlen(dialogTexts[currentDialogIndex])) {
            if (GetTime() - startTime >= 3.0 + displayedTextLength * 0.05) {
                if (!IsSoundPlaying(typingSound)) {
                    PlaySound(typingSound);
                }
                displayedText[displayedTextLength] = dialogTexts[currentDialogIndex][displayedTextLength];
                displayedTextLength++;
                displayedText[displayedTextLength] = '\0';
            }
        } else {
            StopSound(typingSound);
            if (lastTextEndTime == 0) {
                lastTextEndTime = GetTime();
            }
            if (currentDialogIndex < totalDialogTexts - 1 && IsKeyPressed(KEY_ENTER)) {
                currentDialogIndex++;
                displayedTextLength = 0;
                memset(displayedText, 0, sizeof(displayedText));
                lastTextEndTime = 0;
            } else if (currentDialogIndex == totalDialogTexts - 1 && GetTime() - lastTextEndTime >= 4.0) {
                hideDialog = true;
            }
        }
        DrawText(displayedText, 80, SCREEN_HEIGHT - 120, 20, WHITE);

        if (displayedTextLength == strlen(dialogTexts[currentDialogIndex]) && currentDialogIndex < totalDialogTexts - 1) {
            DrawText("Pressione ENTER para continuar...", SCREEN_WIDTH - 300, SCREEN_HEIGHT - 40, 16, GRAY);
        }
    }

    if (hideDialog) {
        if (!soundPlayed) {
            StopMusicStream(windMusic);
            PlaySound(machineLoadingSound);
            soundPlayed = true;
        } else if (!IsSoundPlaying(machineLoadingSound) && !battleMusicStarted) {
            PlayMusicStream(battleMusic);
            battleMusicStarted = true;
        }

        if (battleMusicStarted) {
            UpdateMusicStream(battleMusic);

            for (int y = 0; y < MAPA_ALTURA; y += 2) {
                Vector2 warningPosition = { (MAPA_LARGURA - 1) * TILE_SIZE, y * TILE_SIZE };
                DrawTextureEx(warningSign, warningPosition, 0.0f, 1.0f, WHITE);
            }
        }
    } else {
        if (!windMusicStarted) {
            PlayMusicStream(windMusic);
            windMusicStarted = true;
        }
        UpdateMusicStream(windMusic);
    }

    EndDrawing();
}



void drawLobby() {
    BeginDrawing();

    bool soundPlayed = false;
    static int lastDirection = 3;
    static float walkingTimer = 0.0f;
    static bool isWalking = false;
    int infoBoxX = SCREEN_WIDTH - 230;
    int infoBoxY = 10;
    int infoBoxWidth = 220;
    int infoBoxHeight = 126;
    int merchantMood = 0;

    Rectangle tileSourceRec = { 128, 32, 32, 32 };
    Rectangle hitboxVendinha = { 1389, 330, 123, 120 };
    Rectangle destRecVendinha = { 20, 20, 123* 0.8 , 120* 0.8  };
    Rectangle sourceRecMonster = { 282, 13, 106, 116 };
    Rectangle sourceRecbones1 = { 557, 60, 45, 37 };
    Rectangle sourceRecbones2 = { 557, 60, 45, 37 };
    Rectangle sourceRecbones3 = { 557, 60, 45, 37 };
    Rectangle sourceRecbones4 = { 557, 60, 45, 37 };
    Rectangle sourceRecbones5 = { 666, 125, 23, 19 };
    Rectangle sourceRecbones6 = { 666, 125, 23, 19 };
    Rectangle sourceRecbones7 = { 666, 125, 23, 19 };
    Rectangle sourceRecbones8 = { 784, 72, 29, 24 };
    Rectangle sourceRecbones9 = { 784, 72, 29, 24 };
    Rectangle sourceRecbones10 = { 181, 140, 43, 32 };
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
    Vector2 positionBones = { 300, 50 };
    Vector2 positionBones2 = { 660, 600 }; 
    Vector2 positionBones3 = { 200, 680 }; 
    Vector2 positionBones4 = {760, 300 }; 
    Vector2 positionBones5 = {730, 330 }; 
    Vector2 positionBones6 = {780, 543}; 
    Vector2 positionBones7 = {900, 203}; 
    Vector2 positionBones8 = {1164, 540}; 
    Vector2 positionBones9 = {370, 512}; 
    Vector2 positionBones10 = {560, 134};
    Vector2 positionMonster = { 180, 20 }; 
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
    Vector2 positionCity16 = { 300, 220 };
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
    Rectangle sourceRecPersonagem;
    Vector2 origin = { 0, 0 };
    Color fillColor = (Color){205, 133, 63, 255};
    Color borderColor = (Color){101, 67, 33, 255};
    Vector2 positionPersonagem = { player_x * TILE_SIZE, player_y * TILE_SIZE };
    Rectangle destRecPersonagem = { positionPersonagem.x - 32, positionPersonagem.y - 32, 96, 96 };
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

    portalAnimationTimer += GetFrameTime();
    if (portalAnimationTimer >= 0.1f) {
        portalAnimationTimer = 0.0f;
        portalFrameIndex++;
        if (portalFrameIndex > 3) {
            portalFrameIndex = 0;
        }
    }

    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
            DrawTextureRec(desertTileset, tileSourceRec, tilePosition, WHITE);
        }
    }

    DrawTexturePro(monstersTexture, sourceRecMonster, (Rectangle){ positionMonster.x, positionMonster.y, 106, 116 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones1, (Rectangle){ positionBones.x, positionBones.y, 45, 37 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones2, (Rectangle){ positionBones2.x, positionBones2.y, 45, 37 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones3, (Rectangle){ positionBones3.x, positionBones3.y, 45, 37 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones4, (Rectangle){ positionBones4.x, positionBones4.y, 45, 37 }, (Vector2){ 0, 0 }, 0.0f, WHITE);    
    DrawTexturePro(bonesTexture, sourceRecbones5, (Rectangle){ positionBones5.x, positionBones5.y, 23, 19 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones6, (Rectangle){ positionBones6.x, positionBones6.y, 23, 19 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones7, (Rectangle){ positionBones7.x, positionBones7.y, 23, 19 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones8, (Rectangle){ positionBones8.x, positionBones8.y, 29, 24 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones9, (Rectangle){ positionBones9.x, positionBones9.y, 29, 24 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, sourceRecbones10, (Rectangle){ positionBones10.x, positionBones10.y, 43, 32 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(cityTexture, hitboxVendinha, destRecVendinha, (Vector2){0, 0}, 0.0f, WHITE);

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

    if (isWalking) {
        DrawTexturePro(sombra, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
        DrawTexturePro(personagemAndando, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawTexturePro(sombra, sourceRecPersonagem, destRecPersonagem, (Vector2){0, 0}, 0.0f, WHITE);
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
    DrawTexturePro(portal, portalSourceRec, portalDestRec1, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec2, origin, 0.0f, WHITE);
    DrawTexturePro(portal, portalSourceRec, portalDestRec3, origin, 0.0f, WHITE);

    if (itemsCollected >= 3 && !spaceshipAnimationPlayed) {
        static float spaceshipTimer = 0.0f;
        spaceshipTimer += GetFrameTime();

        Rectangle spaceshipSourceRec = { 0, 0, spaceshipTexture.width, spaceshipTexture.height };
        Rectangle spaceshipDestRec = { spaceshipPositionX, 100, spaceshipTexture.width * 4, spaceshipTexture.height * 4 };
        Vector2 spaceshipOrigin = { spaceshipTexture.width / 2.0f, spaceshipTexture.height / 2.0f };

        DrawTexturePro(spaceshipTexture, spaceshipSourceRec, spaceshipDestRec, spaceshipOrigin, 90.0f, WHITE);

        spaceshipPositionX += 200 * GetFrameTime(); 

        if (!isWarMusicPlaying) {
            StopMusicStream(lobbyMusic); 
            PlayMusicStream(warMusic);
            isWarMusicPlaying = true; 
        }

        if (spaceshipPositionX > SCREEN_WIDTH + 800) {
            spaceshipAnimationPlayed = true;
            spaceshipTimer = 0.0f;
        }
    }

    if (isWarMusicPlaying) {
        UpdateMusicStream(warMusic);
    } else {
        UpdateMusicStream(lobbyMusic);
    }

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

    // Atualize o trecho onde os dados são desenhados
    Vector2 aguaIconPos = { infoBoxX + 18, infoBoxY + 65 };
    Rectangle aguaSourceRec = { 0, 0, aguaTexture.width, aguaTexture.height };
    Rectangle aguaDestRec = { aguaIconPos.x, aguaIconPos.y, 24, 24 };

    DrawTexturePro(aguaTexture, aguaSourceRec, aguaDestRec, origin, 0.0f, WHITE);
    DrawText(TextFormat("Água: %.0f%%", playerWater), infoBoxX + 55, infoBoxY + 70, 18, WHITE);

    Vector2 luckyIconPos = { infoBoxX + 18, infoBoxY + 95 }; // Alinhado ao layout do HUD
    Rectangle luckySourceRec;

    // Verificar o valor de sorte e alterar o ícone conforme necessário
    if (playerLucky == 50) {
        luckySourceRec = (Rectangle){ 164, 44, 107, 108 }; // Coordenadas do ícone para 50% de sorte
    } else if (playerLucky == 30) {
        luckySourceRec = (Rectangle){ 480, 176, 122, 129 }; // Coordenadas do ícone para 30% de sorte
    } else if (playerLucky == 20) {
        luckySourceRec = (Rectangle){ 494, 41, 107, 107 }; // Coordenadas do ícone para 20% de sorte
    } else {
        luckySourceRec = (Rectangle){ 164, 186, 107, 115 }; // Coordenadas do ícone padrão
    }

    Rectangle luckyDestRec = { luckyIconPos.x, luckyIconPos.y, 24, 24 }; // Reduzido para o HUD

    // Desenhar o ícone de sorte
    DrawTexturePro(luckyTexture, luckySourceRec, luckyDestRec, (Vector2){0, 0}, 0.0f, WHITE);

    // Adicionar o texto correspondente
    DrawText(TextFormat("Sorte: %.0f%%", playerLucky), infoBoxX + 50, infoBoxY + 100, 18, WHITE);

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

    if (isPlayerNearMerchant() && !spaceshipAnimationPlayed) {
        Rectangle sourceRecVelho = { 62, 54, 509, 485 };
        Rectangle sourceRecVelhoPuto = { 1084, 108, 510, 484 };
        Rectangle sourceRecVelhoFeliz = { 573, 81, 510, 484 };
        Rectangle destRecVelho = { 150, 370, 256, 256 };
        Vector2 originVelho = { 32, 32 };

        Rectangle currentSprite = (merchantMood == 2) ? sourceRecVelhoPuto :
                                (merchantMood == 1) ? sourceRecVelhoFeliz : sourceRecVelho;


        DrawTexturePro(velho, currentSprite, destRecVelho, originVelho, 0.0f, WHITE);
        if (!spaceshipAnimationPlayed){
            
        }
        if (!isInteractingWithMerchant) {
            merchantMood = 0; 

            DrawDialogBox("Olá viajante, o que podemos negociar hoje?\n\n[1] para vender especiarias\n[2] para comprar uma bolsa nova\n[3] para comprar garrafa de água\n[4] para comprar itens de sorte",
                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

            if (IsKeyPressed(KEY_ONE)) isInteractingWithMerchant = 1;
            else if (IsKeyPressed(KEY_TWO)) isInteractingWithMerchant = 2;
            else if (IsKeyPressed(KEY_THREE)) isInteractingWithMerchant = 3;
            else if (IsKeyPressed(KEY_FOUR)) isInteractingWithMerchant = 4;

        } else {

            switch (isInteractingWithMerchant) {
                case 1: 
                static int merchantMessage = 0;

                if (merchantMessage == 0) {
                    if (itemsCollected > 0) {
                        playerMoney += itemsCollected * 300;
                        itemsCollected = 0;
                        merchantMessage = 1;
                        merchantMood = 1; 
                    } else {
                        merchantMessage = -1;
                        merchantMood = 2; 
                    }
                }

                if (merchantMessage == 1) {
                 
                    while (!IsKeyPressed(KEY_ENTER)) {
            
                        UpdateMusicStream(lobbyMusic);

                 
                        BeginDrawing();
                        DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                        DrawDialogBox("Obrigado pela venda, espero que prospere!\n\n\nAperte[ENTER] para voltar.", 
                                    100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                        EndDrawing();
                    }
                    merchantMessage = 0;
                    merchantMood = 0; 
                    isInteractingWithMerchant = 0;
                } else if (merchantMessage == -1) {
                   
                    while (!IsKeyPressed(KEY_ENTER)) {
              
                        UpdateMusicStream(lobbyMusic);

        
                        BeginDrawing();
                        DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                        DrawDialogBox("Saia daqui, você não tem nenhuma especiaria para\nnegociar!\n\n\nAperte[ENTER] para voltar.", 
                                    100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                        EndDrawing();
                    }
                    merchantMessage = 0;
                    merchantMood = 0; 
                    isInteractingWithMerchant = 0;
                }
                break;


                case 2: 
                DrawDialogBox("Qual bolsa deseja comprar?\n\n[1]  Média (12 especiarias) - 5000\n[2] Grande (24 especiarias) - 10000\n[3] Super (32 especiarias) - 15000",
                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

                if (IsKeyPressed(KEY_ONE)) { 
                    if (playerMoney >= 5000) {
                        MAX_ESPECIARIAS = 12;
                        playerMoney -= 5000;
                        merchantMood = 1; 

                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    } else {
                        merchantMood = 2; 
                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    }
                } else if (IsKeyPressed(KEY_TWO)) { 
                    if (playerMoney >= 10000) {
                        MAX_ESPECIARIAS = 24;
                        playerMoney -= 10000;
                        merchantMood = 1; 

                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0;
                    } else {
                        merchantMood = 2; 
                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0;
                    }
                } else if (IsKeyPressed(KEY_THREE)) { 
                    if (playerMoney >= 15000) {
                        MAX_ESPECIARIAS = 32;
                        playerMoney -= 15000;
                        merchantMood = 1; 

                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                            DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    } else {
                        merchantMood = 2; 
                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0;
                    }
                }
                break;


                case 3: 
                DrawDialogBox("Qual garrafa de água deseja comprar?\n\n[1]  Pequena (10%) - 3000\n[2] Média (20%) - 5000\n[3] Grande (30%) - 7000",
                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

                if (IsKeyPressed(KEY_ONE)) {
                    if (playerMoney >= 3000) {
                        playerWater = fmin(playerWater + GARRAFA_PEQUENA_CAPACIDADE, 100);
                        playerMoney -= 3000;
                        merchantMood = 1; 

                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Obrigado pela compra! Aproveite sua água.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    } else {
                        merchantMood = 2; 
                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    }
                } else if (IsKeyPressed(KEY_TWO)) {
                    if (playerMoney >= 5000) {
                        playerWater = fmin(playerWater + GARRAFA_MEDIA_CAPACIDADE, 100);
                        playerMoney -= 5000;
                        merchantMood = 1; 

                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic);
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Obrigado pela compra! Aproveite sua água.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    } else {
                        merchantMood = 2; 
                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic);
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    }
                } else if (IsKeyPressed(KEY_THREE)) {
                    if (playerMoney >= 7000) {
                        playerWater = fmin(playerWater + GARRAFA_GRANDE_CAPACIDADE, 100);
                        playerMoney -= 7000;
                        merchantMood = 1; 

                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Obrigado pela compra! Aproveite sua água.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    } else {
                        merchantMood = 2; 
                        while (!IsKeyPressed(KEY_ENTER)) {
                            UpdateMusicStream(lobbyMusic); 
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                            DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                            EndDrawing();
                        }
                        isInteractingWithMerchant = 0;
                        merchantMood = 0; 
                    }
                }
                break;

                case 4:
                    DrawDialogBox("Qual item de sorte deseja comprar?\n\n[1] Ferradura (20%) - 3000\n[2] Amuleto (30%) - 7000\n[3] Trevo (50%) - 10000",
                                100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);

                    if (IsKeyPressed(KEY_ONE)) { 
                        if (playerMoney >= FERRADURA_PRECO) {
                            playerLucky = FERRADURA_SORTE;
                            playerMoney -= FERRADURA_PRECO;
                            merchantMood = 1;

                            while (!IsKeyPressed(KEY_ENTER)) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                                DrawDialogBox("Obrigado pela compra! Aproveite sua sorte.\n\n\nAperte[ENTER] para voltar.",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                EndDrawing();
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        } else {
                            merchantMood = 2;
                            while (!IsKeyPressed(KEY_ENTER)) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                EndDrawing();
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    } else if (IsKeyPressed(KEY_TWO)) { 
                        if (playerMoney >= AMULETO_PRECO) {
                            playerLucky = AMULETO_SORTE;
                            playerMoney -= AMULETO_PRECO;
                            merchantMood = 1;

                            while (!IsKeyPressed(KEY_ENTER)) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                                DrawDialogBox("Obrigado pela compra! Aproveite sua sorte.\n\n\nAperte[ENTER] para voltar.",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                EndDrawing();
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        } else {
                            merchantMood = 2;
                            while (!IsKeyPressed(KEY_ENTER)) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                EndDrawing();
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    } else if (IsKeyPressed(KEY_THREE)) { 
                        if (playerMoney >= TREVO_PRECO) {
                            playerLucky = TREVO_SORTE;
                            playerMoney -= TREVO_PRECO;
                            merchantMood = 1;

                            while (!IsKeyPressed(KEY_ENTER)) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                                DrawDialogBox("Obrigado pela compra! Aproveite sua sorte.\n\n\nAperte[ENTER] para voltar.",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                EndDrawing();
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        } else {
                            merchantMood = 2;
                            while (!IsKeyPressed(KEY_ENTER)) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                DrawDialogBox("Você não tem dinheiro suficiente para essa compra.\n\n\nAperte[ENTER] para voltar.",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                EndDrawing();
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    }
                    break;

            }   
        }
    } else if (mensagem != NULL) {
        DrawDialogBox(mensagem, 70, 580, 400, 110, WHITE, BLACK, true);
    } else {
        isInteractingWithMerchant = 0;
    }
    verificarProximidadePoco(player_x * TILE_SIZE, player_y * TILE_SIZE);
    EndDrawing();
}