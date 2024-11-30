#include "lobby.h"
#include "game.h"

static Texture2D personagemAndando;
static Texture2D monstersTexture;
static Texture2D cerealsTexture;
static Texture2D desertTileset;
static Texture2D bonesTexture;
static Texture2D aguaTexture;
static Texture2D npcslobbyTexture;
static Texture2D homemTexture;
static Texture2D mulherTexture;
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
static Texture2D controlesTexture;
Rectangle botaoAreas[4];

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
//static bool isInExtendedLobby = false;
static bool isWarMusicPlaying = false;

static bool telaVaziaBloqueada = false; // Variável para bloquear a telaVazia após a sequência final


Rectangle npcslobbyCollisionBox;
Rectangle npcslobby2CollisionBox;

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
    npcslobbyTexture = LoadTexture("static/image/npcslobby.png");
    homemTexture = LoadTexture("static/image/homem.png");
    mulherTexture = LoadTexture("static/image/mulher.png");
    troca = LoadSound("static/music/trocaDeDinheiro.wav");
    monstersTexture = LoadTexture("static/image/monsters.png");
    bonesTexture = LoadTexture("static/image/bones.png");
    sombra = LoadTexture("static/image/sombras.png");
    spaceshipTexture = LoadTexture("static/image/spaceships.png");
    warMusic = LoadMusicStream("static/music/warMusic.wav");
    controlesTexture = LoadTexture("static/image/Xbox_one.png");

    botaoAreas[0] = (Rectangle){0, 80, 16, 16};  // Botão Y
    botaoAreas[1] = (Rectangle){0, 16, 16, 16};  // Botão A
    botaoAreas[2] = (Rectangle){0, 112, 16, 16}; // Botão B
    botaoAreas[3] = (Rectangle){0, 48, 16, 16};  // Botão X

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
    UnloadTexture(npcslobbyTexture);
    UnloadTexture(homemTexture);
    UnloadTexture(mulherTexture);
    UnloadSound(troca);
    UnloadTexture(spaceshipTexture);
    UnloadMusicStream(warMusic);
    UnloadTexture(controlesTexture);
    
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
    comandoJogador(&usandoControle);

    if (IsGamepadAvailable(0)) {
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
            dy = -1;
        }
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
            dy = 1;
        }
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
            dx = -1;
        }
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            dx = 1;
        }
    }

    // Garantir que a música do lobby toque
    if (IsMusicStreamPlaying(warMusic)) {
        StopMusicStream(lobbyMusic); // Garante que lobbyMusic pare enquanto warMusic toca
        UpdateMusicStream(warMusic);
    } else {
        if (!IsMusicStreamPlaying(lobbyMusic)) {
            PlayMusicStream(lobbyMusic);
        }
        UpdateMusicStream(lobbyMusic);
    }

    if (IsKeyPressed(KEY_D)) dx = 1;
    if (IsKeyPressed(KEY_A)) dx = -1;
    if (IsKeyPressed(KEY_W)) dy = -1;
    if (IsKeyPressed(KEY_S)) dy = 1;

    movePlayer(dx, dy);

    // Condição para acessar o chefe (boss)
    if (player_x >= MAPA_LARGURA - 1) { // Jogador tenta sair pelo limite direito do mapa
        if (itemsCollected >= 3 && !spaceshipAnimationPlayed) { // Verifica se o jogador tem mais de 4 especiarias
            *currentScreen = EMPTY_SCREEN; // Transição para a tela do chefe
            return;
        } else {
            // Impede o jogador de sair e exibe uma mensagem
            player_x = MAPA_LARGURA - 2; // Reposiciona o jogador antes do limite
            mensagem = "Você precisa de mais especiarias para enfrentar o chefe!";
        }
    }

    if ((player_x >= PORTAL_LOBBY_MAPA1_X - 1 && player_x < PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA1_Y - 1 && player_y < PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA + 1)) {
        
        if (usandoControle) {
            mensagem = "Deseja viajar para Zamirat?\nDificuldade: Facil\n\nPressione: [Y]";
        } else {
            mensagem = "Deseja viajar para Zamirat?\nDificuldade: Facil\n\nPressione: [P]";
        }

        pertoDePortal = true;
        if (IsKeyPressed(KEY_P) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
            *currentScreen = GAME;
            mapaAtual = 0;
            mensagem = NULL;
        }
    }

    if ((player_x >= PORTAL_LOBBY_MAPA2_X - 1 && player_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA2_Y - 1 && player_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA + 1)) {
        
        if (usandoControle) {
            mensagem = "Deseja viajar para Bashir'har?\nDificuldade: Media\n\nPressione: [Y]";
        } else {
            mensagem = "Deseja viajar para Bashir'har?\nDificuldade: Media\n\nPressione: [P]";
        }

        pertoDePortal = true;
        if (IsKeyPressed(KEY_P) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
            *currentScreen = GAME;
            mapaAtual = 1;
            mensagem = NULL;
        }
    }

    if ((player_x >= PORTAL_LOBBY_MAPA3_X - 1 && player_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA + 1) &&
        (player_y >= PORTAL_LOBBY_MAPA3_Y - 1 && player_y < PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA + 1)) {
        
        if (usandoControle) {
            mensagem = "Deseja viajar para Qasr'Rahim?\nDificuldade: Dificil\n\nPressione: [Y]";
        } else {
            mensagem = "Deseja viajar para Qasr'Rahim?\nDificuldade: Dificil\n\nPressione: [P]";
        }

        pertoDePortal = true;
        if (IsKeyPressed(KEY_P) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
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

static double lastWaterAttemptTime = -120.0; // Tempo da última tentativa
static bool lastAttemptSuccessful = true;   // Indica se a última tentativa foi bem-sucedida

void tentarRecuperarAguaNoPoco() {
    double currentTime = GetTime();

    int chance = GetRandomValue(0, 99); 
    int aguaRecuperada = 0;
    bool sucesso = false;
    char sucessoMensagem[128];

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

    if (sucesso) {
        while (true) {
            UpdateMusicStream(lobbyMusic);
            BeginDrawing();
            if (usandoControle) {
                snprintf(sucessoMensagem, sizeof(sucessoMensagem), "Parabens! Voce recuperou %d%% de agua do poco.\n\nAperte [B]para voltar.", aguaRecuperada);
                DrawDialogBoxWithButtons(sucessoMensagem, 110, 550, 600, 100, WHITE, GREEN, usandoControle, controlesTexture, botaoAreas, 1.50);
            } else {
                snprintf(sucessoMensagem, sizeof(sucessoMensagem), "Parabéns! Você recuperou %d%% de água do poço.\n\nAperte [ENTER] para voltar.", aguaRecuperada);
                DrawDialogBox(sucessoMensagem, 100, 550, 600, 100, WHITE, GREEN, false);
            }
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
        }
        playerWater = fmin(playerWater + aguaRecuperada, 100.0);
        lastAttemptSuccessful = true;
    } else {
        while (true) {
            UpdateMusicStream(lobbyMusic);
            BeginDrawing();
            if (usandoControle) {
                DrawDialogBoxWithButtons("Parece que esta sem sorte, voce nao conseguiu nada\ndessa vez, tente na proxima...\n\nAperte [B]para voltar", 110, 550, 600, 100, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
            } else {
                DrawDialogBox("Parece que está sem sorte, você não conseguiu nada\ndessa vez, tente na próxima...\n\nAperte[ENTER] para voltar", 100, 550, 600, 100, WHITE, RED, false);
            }
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
        }
        lastAttemptSuccessful = false;
        lastWaterAttemptTime = currentTime;
    }
}

void verificarProximidadePoco(float playerX, float playerY) {
    const float areas[][4] = {
        {720, 160, 80, 80},
        {100, 550, 80, 80} 
    };
    const int numAreas = sizeof(areas) / sizeof(areas[0]);
    double currentTime = GetTime();

    for (int i = 0; i < numAreas; i++) {
        float areaX = areas[i][0];
        float areaY = areas[i][1];
        float areaWidth = areas[i][2];
        float areaHeight = areas[i][3];

        if (playerX >= areaX - 10 && playerX <= areaX + areaWidth + 10 &&
            playerY >= areaY - 10 && playerY <= areaY + areaHeight + 10) {

            if (!lastAttemptSuccessful && (currentTime - lastWaterAttemptTime < 10.0)) {
                DrawDialogBox("Ainda não é possível tentar novamente. Volte depois.", 
                              100, 550, 600, 100, WHITE, RED, false);
                return;
            }
            
            if (usandoControle) {
                DrawDialogBoxWithButtons("Voce deseja tentar recuperar agua do poco?\n\n[A]Sim\n[X]Nao", 100, 550, 600, 100, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.30);
            } else {
                DrawDialogBox("Você deseja tentar recuperar água do poço?\n\n[1] Sim\n[2] Não", 100, 550, 600, 100, WHITE, BLACK, false);
            }

            if (IsKeyPressed(KEY_ONE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                tentarRecuperarAguaNoPoco();
            } else if (IsKeyPressed(KEY_TWO) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
                while (true) {
                    BeginDrawing();
                    UpdateMusicStream(lobbyMusic);
                    if (usandoControle) {
                        DrawDialogBoxWithButtons("Voce decidiu nao tentar agora\n\nAperte [B]para voltar", 100, 550, 600, 100, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                    } else {
                        DrawDialogBox("Você decidiu não tentar agora\n\nAperte [ENTER] para voltar", 100, 550, 600, 100, WHITE, GRAY, false);
                    }
                    EndDrawing();
                    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
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
    static Texture2D fireballSprite;
    static Music windMusic;
    static Music battleMusic;
    static Sound machineLoadingSound;
    static Sound typingSound;
    static Sound gameOverSoundWar;

    #define MAX_OCCURRENCES 15
    //static Vector2 warningPositions[MAX_OCCURRENCES]; // Posições dos Warnings
    //static double occurrenceTimers[MAX_OCCURRENCES];  // Timers para cada Warning/Explosion
    //static bool warningsActive[MAX_OCCURRENCES];     // Status de Warning ativo
    //static bool explosionsActive[MAX_OCCURRENCES];   // Status de Explosion ativo
    #define TOTAL_EXPLOSIONS 20

    // Configurações para as explosões
    #define TOTAL_EXPLOSIONS 20

    // Configurações para as explosões
    static Vector2 explosionPositions[TOTAL_EXPLOSIONS]; // Posições das explosões
    static double explosionTimers[TOTAL_EXPLOSIONS];     // Timers para cada explosão
    static bool explosionWarnings[TOTAL_EXPLOSIONS];    // Ativação dos warnings
    static bool explosionActiveStates[TOTAL_EXPLOSIONS];// Ativação das explosões
    static bool explosionHitboxes[TOTAL_EXPLOSIONS];

    static bool initializedWarnings = false;         // Marca inicialização
    static Sound explosionSound; 

    static bool initialized = false;
    static double startTime = 0;
    //static bool dialogTriggered = false;
    static bool windMusicStarted = false;
    static bool battleMusicStarted = false;
    static bool soundPlayed = false;
    static float fireballPositionX; // Posição inicial da Fireball
    static float fireballSpeed;    // Velocidade da Fireball
    static bool fireballActive = false; // Indica se a Fireball está ativa
    static int fireballWave = 1; // Controla qual onda está ativa: 1 ou 2
    static bool waveInProgress = true; // Indica se a onda atual está em progresso
    static Sound cannonSound;
    static Texture2D explosionSprite;
                    

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
    //static double explosionStartTime = 0; // Tempo em que a explosão começou

    if (!initialized) {
        empty_personagem = LoadTexture("static/image/newstoppedsprites.png");
        empty_sombra = LoadTexture("static/image/sombras.png");
        empty_desertTileset = LoadTexture("static/image/environment.png");
        empty_villain = LoadTexture("static/image/villain.png");
        villain_sombra = LoadTexture("static/image/sombras.png");
        villain_portrait = LoadTexture("static/image/villainPortrait.png");
        warningSign = LoadTexture("static/image/WarningSign05.png");
        fireballSprite = LoadTexture("static/image/Fireball68x9.png");
        windMusic = LoadMusicStream("static/music/wind.wav");
        battleMusic = LoadMusicStream("static/music/battleMusic.wav");
        machineLoadingSound = LoadSound("static/music/machineLoading.wav");
        typingSound = LoadSound("static/music/falas.wav");
        gameOverSoundWar = LoadSound("static/music/deathsound.wav");
        cannonSound = LoadSound("static/music/cannon.wav");
        explosionSprite = LoadTexture("static/image/Explosion.png");

        SetSoundVolume(machineLoadingSound, 1.0f);
        SetSoundVolume(typingSound, 0.8f);
        SetSoundVolume(cannonSound, 1.0f);

        // Configurar posição e velocidade da Fireball
        fireballPositionX = MAPA_LARGURA * TILE_SIZE; // Começa à direita
        fireballSpeed = (float)(MAPA_LARGURA * TILE_SIZE) / 1.5f; // Velocidade para 1,5 segundos

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

    // Lógica de movimento do personagem
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

    // Adicionar a verificação de proximidade com o vilão aqui
    int distanceX = abs(empty_player_x - (MAPA_LARGURA - 3)); // Posição X do vilão
    int distanceY = abs(empty_player_y - 10);                // Posição Y do vilão

    if ((distanceX <= 1 && distanceY == 0) || (distanceY <= 1 && distanceX == 0)) {
        // O jogador está a 1 tile de distância do vilão

        // Parar músicas e sons
        StopMusicStream(windMusic);
        StopMusicStream(battleMusic);
        StopSound(machineLoadingSound);
        StopSound(typingSound);
        StopSound(gameOverSoundWar);

        // Mostrar tela preta
        double blackStartTime = GetTime();
        while (GetTime() - blackStartTime < 1.5) {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        // Tocar o som "laugh.wav" após 1.5 segundos
        Sound laughSound = LoadSound("static/music/laugh.wav");
        PlaySound(laughSound);

        // Esperar 1 segundo após "laugh.wav" começar a tocar
        double laughStartTime = GetTime();
        while (GetTime() - laughStartTime < 1.0) {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        // Configurar textos da textbox
        const char *dialogTexts[] = {
            "HahahHahHAhAhA!",
            "Depois de todo caos que você jurou instaurar em Arrakis, eu vou poder salvá-la do seu mal.",
            "Foi tão fácil, sua infantaria é tão ridícula...",
            "Suas últimas palavras...?"
        };
        const int dialogCount = sizeof(dialogTexts) / sizeof(dialogTexts[0]);
        int currentDialogIndex = 0;

        // Variáveis para efeito de digitação
        char displayedText[256] = "";
        int displayedTextLength = 0;
        double textStartTime1 = GetTime();
        bool dialogComplete = false;

        // Carregar som de falas
        Sound falasSound = LoadSound("static/music/falas.wav");

        // Mostrar os diálogos sequencialmente
        while (currentDialogIndex < dialogCount) {
            BeginDrawing();
            ClearBackground(BLACK);

            // Caixa de texto
            DrawRectangle(50, GetScreenHeight() - 150, GetScreenWidth() - 100, 100, (Color){0, 0, 0, 200});
            DrawRectangleLines(50, GetScreenHeight() - 150, GetScreenWidth() - 100, 100, WHITE);

            // Efeito de digitação
            if (displayedTextLength < strlen(dialogTexts[currentDialogIndex])) {
                if (GetTime() - textStartTime1 >= displayedTextLength * 0.05) { // Controle de velocidade de digitação
                    if (displayedTextLength == 0) {
                        // Iniciar som de falas no início do efeito de digitação
                        PlaySound(falasSound);
                    }
                    displayedText[displayedTextLength] = dialogTexts[currentDialogIndex][displayedTextLength];
                    displayedTextLength++;
                    displayedText[displayedTextLength] = '\0';
                }
            } else {
                dialogComplete = true; // Todo o texto foi exibido
                StopSound(falasSound); // Parar o som de falas
            }

            // Exibir o texto
            DrawText(displayedText, 70, GetScreenHeight() - 130, 20, WHITE);

            EndDrawing();

            // Após o texto ser completamente exibido
            if (dialogComplete && (GetTime() - textStartTime1 >= displayedTextLength * 0.05 + 2.0)) {
                // Esperar 2 segundos antes de exibir o próximo texto
                double waitStartTime = GetTime();
                while (GetTime() - waitStartTime < 2.0) {
                    BeginDrawing();
                    ClearBackground(BLACK);

                    // Reexibir o texto atual durante a pausa de 2 segundos
                    DrawRectangle(50, GetScreenHeight() - 150, GetScreenWidth() - 100, 100, (Color){0, 0, 0, 200});
                    DrawRectangleLines(50, GetScreenHeight() - 150, GetScreenWidth() - 100, 100, WHITE);
                    DrawText(displayedText, 70, GetScreenHeight() - 130, 20, WHITE);

                    EndDrawing();
                }

                // Passar para o próximo texto
                currentDialogIndex++;
                displayedTextLength = 0;
                displayedText[0] = '\0';
                textStartTime1 = GetTime();
                dialogComplete = false;
            }
        }

        // Tela preta por 2 segundos após o último diálogo
        double finalBlackTime = GetTime();
        while (GetTime() - finalBlackTime < 2.0) {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        // Exibir texto "Eu só queria prosperar minha casa..." com efeito de digitação mais lento
        const char *finalText = "Eu só queria prosperar minha casa...";
        char displayedFinalText[256] = "";
        int finalTextLength = 0;
        double finalTextStartTime = GetTime();

        while (finalTextLength < strlen(finalText)) {
            BeginDrawing();
            ClearBackground(BLACK);

            // Efeito de digitação mais lento para o texto final
            if (GetTime() - finalTextStartTime >= finalTextLength * 0.1) { // Controle de velocidade mais lento
                if (finalTextLength == 0) {
                    // Iniciar som de falas no início do efeito de digitação
                    PlaySound(falasSound);
                }
                displayedFinalText[finalTextLength] = finalText[finalTextLength];
                finalTextLength++;
                displayedFinalText[finalTextLength] = '\0';
            }

            // Exibir o texto
            DrawText(
                displayedFinalText,
                (GetScreenWidth() - MeasureText(displayedFinalText, 30)) / 2,
                GetScreenHeight() / 2 - 15,
                30,
                WHITE
            );

            EndDrawing();
        }

        // Parar sons ao terminar o texto final
        StopSound(falasSound);
        StopSound(laughSound);

        // Manter o texto final por 10 segundos
        double finalTextDisplayTime = GetTime();
        while (GetTime() - finalTextDisplayTime < 10.0) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText(
                finalText,
                (GetScreenWidth() - MeasureText(finalText, 30)) / 2,
                GetScreenHeight() / 2 - 15,
                30,
                WHITE
            );
            EndDrawing();
        }

        // Exibir novo texto "Ele... Ele não é tão diferente de mim..." com efeito de digitação
        const char *newText1 = "Ele... Ele não é tão diferente de mim...";
        const char *newText2 = "Será que vale a pena matá-lo, ou posso me unir a ele?";

        displayedTextLength = 0;
        memset(displayedText, 0, sizeof(displayedText));
        double newTextStartTime = GetTime();

        // Efeito de digitação para "Ele... Ele não é tão diferente de mim..."
        while (displayedTextLength < strlen(newText1)) {
            BeginDrawing();
            ClearBackground(BLACK);

            if (GetTime() - newTextStartTime >= displayedTextLength * 0.05) {
                displayedText[displayedTextLength] = newText1[displayedTextLength];
                displayedTextLength++;
                displayedText[displayedTextLength] = '\0';
            }

            DrawText(displayedText, 70, GetScreenHeight() - 130, 20, WHITE);
            EndDrawing();
        }

        StopSound(falasSound);

        // Esperar 3 segundos após "Ele... Ele não é tão diferente de mim..."
        double pauseAfterNewText1 = GetTime();
        while (GetTime() - pauseAfterNewText1 < 3.0) {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        // Efeito de digitação para "Será que vale a pena matá-lo, ou posso me unir a ele?"
        displayedTextLength = 0;
        memset(displayedText, 0, sizeof(displayedText));
        newTextStartTime = GetTime();

        while (displayedTextLength < strlen(newText2)) {
            BeginDrawing();
            ClearBackground(BLACK);

            if (GetTime() - newTextStartTime >= displayedTextLength * 0.05) {
                displayedText[displayedTextLength] = newText2[displayedTextLength];
                displayedTextLength++;
                displayedText[displayedTextLength] = '\0';
            }

            DrawText(displayedText, 70, GetScreenHeight() - 130, 20, WHITE);
            EndDrawing();
        }

        StopSound(falasSound);

        // Esperar 3 segundos após "Será que vale a pena matá-lo..."
        double pauseAfterNewText2 = GetTime();
        while (GetTime() - pauseAfterNewText2 < 3.0) {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        const char *choiceText = "Poupar? [Aperte P] | Matar? [Aperte K]";

        BeginDrawing();
        ClearBackground(BLACK);

        // Exibir o texto das escolhas centralizado na tela
        DrawText(
            choiceText,
            (GetScreenWidth() - MeasureText(choiceText, 30)) / 2, // Centralizar horizontalmente
            GetScreenHeight() / 2 - 15,                          // Posicionar verticalmente
            30,                                                  // Tamanho da fonte
            WHITE                                                // Cor do texto
        );

        Sound breathingSound = LoadSound("static/music/breathing.wav");
        Sound suspenseSound = LoadSound("static/music/suspenseMusic.wav");
        Texture2D slashSprite = LoadTexture("static/image/slash.png"); // Sprite do slash

        SetSoundVolume(breathingSound, 1.0f); // Ajuste o volume do som
        SetSoundVolume(suspenseSound, 0.7f);  // Ajuste o volume da música
        PlaySound(breathingSound);            // Inicia o som de respiração
        PlaySound(suspenseSound);             // Inicia a música de suspense

        bool waitingForInput = true;
        while (waitingForInput) {
            BeginDrawing();
            ClearBackground(BLACK);

            // Exibir o texto das escolhas centralizado na tela
            const char *choiceText = "Poupar? [Aperte P] | Matar? [Aperte K]";
            DrawText(
                choiceText,
                (GetScreenWidth() - MeasureText(choiceText, 30)) / 2, // Centralizar horizontalmente
                GetScreenHeight() / 2 - 15,                          // Posicionar verticalmente
                30,                                                  // Tamanho da fonte
                WHITE                                                // Cor do texto
            );

            EndDrawing();

            // Verificar entrada do jogador
            if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_K)) {
                StopSound(breathingSound); // Para o som ao pressionar qualquer tecla válida
                StopSound(suspenseSound); // Para a música de suspense
                waitingForInput = false;

                if (IsKeyPressed(KEY_P)) {
                    // Ação para poupar
                    // Adicione sua lógica aqui
                    break;
                }
                if (IsKeyPressed(KEY_K)) {
                    StopSound(breathingSound); // Parar o som de respiração
                    StopSound(suspenseSound);  // Parar o som de suspense

                    Texture2D portraitProtagonist = LoadTexture("static/image/portraitProtagonist.png");
                    Texture2D portraitProtagonist2 = LoadTexture("static/image/portraitProtagonist2.png");
                    Texture2D portraitProtagonist3 = LoadTexture("static/image/portraitProtagonist3.png");
                    Sound deathSound = LoadSound("static/sound/deathSound.wav");
                    Sound typingSound = LoadSound("static/sound/falas.wav");

                    // Configurações de opacidade e escala
                    float opacities[] = { 0.3f, 0.7f, 1.0f };
                    float scales[] = { 1.0f, 1.5f, 2.0f };
                    double durations[] = { 0.5, 0.5, 1.0 }; // Duração reduzida para transições mais rápidas

                    // Posição base do sprite
                    float positionX, positionY;

                    // Exibir `portraitProtagonist` com fases de opacidade e escala
                    for (int i = 0; i < 3; i++) {
                        float scale = scales[i];
                        positionX = GetScreenWidth() - portraitProtagonist.width * scale;
                        positionY = GetScreenHeight() - portraitProtagonist.height * scale;

                        if (i > 0) {
                            PlaySound(deathSound); // Tocar som entre as fases
                        }

                        double startTime = GetTime();
                        while (GetTime() - startTime < durations[i]) {
                            BeginDrawing();
                            ClearBackground(BLACK);

                            Rectangle sourceRec = { 0, 0, portraitProtagonist.width, portraitProtagonist.height };
                            Rectangle destRec = {
                                positionX,
                                positionY,
                                portraitProtagonist.width * scale,
                                portraitProtagonist.height * scale
                            };
                            Vector2 origin = { 0, 0 };

                            DrawTexturePro(portraitProtagonist, sourceRec, destRec, origin, 0.0f, Fade(WHITE, opacities[i]));

                            EndDrawing();
                        }
                    }

                    // Exibir `portraitProtagonist2` por 1.5 segundos
                    double secondPortraitStartTime = GetTime();
                    while (GetTime() - secondPortraitStartTime < 1.5) {
                        BeginDrawing();
                        ClearBackground(BLACK);

                        Rectangle sourceRec = { 0, 0, portraitProtagonist2.width, portraitProtagonist2.height };
                        Rectangle destRec = {
                            positionX,
                            positionY,
                            portraitProtagonist2.width * scales[2],
                            portraitProtagonist2.height * scales[2]
                        };
                        Vector2 origin = { 0, 0 };

                        DrawTexturePro(portraitProtagonist2, sourceRec, destRec, origin, 0.0f, WHITE);

                        EndDrawing();
                    }

                    // Exibir `portraitProtagonist3` com caixa de texto simultânea
                    const char *text = "Eu já queria fazer isso há um tempo.";
                    char displayedText[256] = "";
                    int totalChars = strlen(text);
                    int displayedChars = 0;
                    double textStartTime2 = GetTime();
                    bool dialogComplete = false;
                    double textboxEndTime = 0;

                    while (!dialogComplete || (textboxEndTime != 0 && GetTime() - textboxEndTime < 2.0)) {
                        BeginDrawing();
                        ClearBackground(BLACK);

                        // Desenhar o `portraitProtagonist3`
                        Rectangle sourceRec = { 0, 0, portraitProtagonist3.width, portraitProtagonist3.height };
                        Rectangle destRec = {
                            positionX,
                            positionY,
                            portraitProtagonist3.width * scales[2],
                            portraitProtagonist3.height * scales[2]
                        };
                        Vector2 origin = { 0, 0 };
                        DrawTexturePro(portraitProtagonist3, sourceRec, destRec, origin, 0.0f, WHITE);

                        // Configuração dinâmica da caixa de texto
                        int textWidth = MeasureText(text, 20); // Largura do texto na fonte escolhida
                        int boxWidth = textWidth + 20; // Largura da caixa (texto + margem)
                        int boxHeight = 60; // Altura fixa
                        float boxX = positionX - boxWidth - 20; // Caixa posicionada à esquerda do sprite
                        float boxY = positionY + (portraitProtagonist3.height * scales[2] / 4); // Centralizada na altura do sprite

                        DrawRectangle(boxX, boxY, boxWidth, boxHeight, WHITE); // Fundo da caixa
                        DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, BLACK); // Contorno da caixa

                        // Efeito de digitação
                        if (!dialogComplete) {
                            double elapsed = GetTime() - textStartTime2;
                            displayedChars = (int)((elapsed / 0.5) * totalChars); // Exibir caracteres proporcionalmente ao tempo
                            if (displayedChars > totalChars) {
                                displayedChars = totalChars;
                                dialogComplete = true; // Todo o texto foi exibido
                                textboxEndTime = GetTime(); // Marcar o tempo final da exibição
                            }
                            strncpy(displayedText, text, displayedChars);
                            displayedText[displayedChars] = '\0';

                            // Tocar som enquanto o texto é exibido
                            if (displayedChars > 0 && displayedChars < totalChars && !IsSoundPlaying(typingSound)) {
                                PlaySound(typingSound);
                            }
                        }

                        // Exibir o texto na caixa
                        DrawText(displayedText, boxX + 10, boxY + 20, 20, BLACK);

                        EndDrawing();
                    }

                    // Parte do "slashing"
                    // Carregar o sprite para a animação de "slash"
                    Texture2D slashSprite = LoadTexture("static/image/slash.png");

                    // Carregar o som "slashing.wav"
                    Sound slashingSound = LoadSound("static/music/slashing.wav");
                    SetSoundVolume(slashingSound, 1.0f); // Ajustar o volume, se necessário

                    // Configuração da animação
                    const int frameWidth = 128;  // Largura de cada quadro na spritesheet
                    const int frameHeight = 128; // Altura de cada quadro na spritesheet
                    const int numFrames = 10;    // Total de quadros
                    const float frameTime = 1.0f / numFrames; // Tempo por quadro (1 segundo para todos os quadros)

                    // Dimensão proporcional do sprite desenhado
                    const int drawWidth = 896;
                    const int drawHeight = 896;

                    // Posição do sprite na tela
                    Vector2 slashPosition = {
                        (GetScreenWidth() - drawWidth) / 2,
                        (GetScreenHeight() - drawHeight) / 2
                    };

                    // Lógica de animação
                    float animationTimer = 0.0f;
                    int currentFrame = 0;
                    int animationCycles = 0;
                    const int maxCycles = 3;

                    // Tocar o som no início da animação
                    PlaySound(slashingSound);

                    // Executar a animação 3 vezes
                    while (animationCycles < maxCycles) {
                        BeginDrawing();
                        ClearBackground(BLACK);

                        // Calcular o quadro atual
                        animationTimer += GetFrameTime();
                        if (animationTimer >= frameTime) {
                            animationTimer = 0.0f;
                            currentFrame++;
                            if (currentFrame >= numFrames) {
                                currentFrame = 0;
                                animationCycles++;
                            }
                        }

                        // Calcular as coordenadas do quadro na spritesheet
                        int frameX = (currentFrame % 2) * frameWidth; // Alterna entre 0 e 128 para colunas
                        int frameY = (currentFrame / 2) * frameHeight; // Alterna entre linhas
                        Rectangle sourceRec = { frameX, frameY, frameWidth, frameHeight };
                        Rectangle destRec = { slashPosition.x, slashPosition.y, drawWidth, drawHeight };
                        Vector2 origin = { 0, 0 };

                        // Desenhar o quadro atual
                        DrawTexturePro(slashSprite, sourceRec, destRec, origin, 0.0f, WHITE);

                        EndDrawing();
                    }

                    Sound creditosMusicRun = LoadSound("static/music/creditosMusic.wav");
                    PlaySound(creditosMusicRun);

                    double startTimeRun1 = GetTime();
                    while (GetTime() - startTimeRun1 < 3.0) {

                        BeginDrawing();
                        ClearBackground(BLACK);
                        EndDrawing();
                    }
                        Texture2D mountainsSprite = LoadTexture("static/image/mountains.png");
                        Texture2D farMountainsSprite = LoadTexture("static/image/far-mountains.png");
                        Texture2D personagemRunningRun = LoadTexture("static/image/personagemRunning.png");
                        Texture2D platformsSpriteRun = LoadTexture("static/image/platforms2.png");

                        // Configurações de escala
                        float runningSpriteScaleRun = 6.0f;
                        float spriteWidthRun = 64 * runningSpriteScaleRun;
                        float spriteHeightRun = 64 * runningSpriteScaleRun;

                        float platformsScaleRun = 10.0f;
                        float platformsSpriteWidthRun = platformsSpriteRun.width * platformsScaleRun;
                        float platformsSpriteHeightRun = platformsSpriteRun.height * platformsScaleRun;

                        float mountainsScale = 3.0f;
                        float mountainsSpriteWidth = mountainsSprite.width * mountainsScale;
                        float mountainsSpriteHeight = mountainsSprite.height * mountainsScale;

                        float farMountainsScale = 2.0f;
                        float farMountainsSpriteWidth = farMountainsSprite.width * farMountainsScale;
                        float farMountainsSpriteHeight = farMountainsSprite.height * farMountainsScale;

                        // Configurações de posição
                        float runningSpriteXRun = -spriteWidthRun;
                        float runningSpriteYRun = GetScreenHeight() - platformsSpriteHeightRun - (spriteHeightRun * 0.5f); // Descer um pouco

                        float platformsSpriteYRun = GetScreenHeight() - (platformsSpriteHeightRun * 0.9f);
                        float mountainsSpriteY = platformsSpriteYRun - mountainsSpriteHeight * 0.8f - 20; // Subir um pouco
                        float farMountainsSpriteY = (GetScreenHeight() - farMountainsSpriteHeight) / 2;   // Centralizar na tela

                        float finalPositionXRun = (GetScreenWidth() - spriteWidthRun) / 2;
                        float runningSpeedRun = (finalPositionXRun - runningSpriteXRun) / 15.0f;

                        const int frameWidthRun = 64;
                        const int frameHeightRun = 64;
                        const int totalFramesRun = 8;
                        const float frameTimeRun = 0.1f;

                        float animationTimerRun = 0.0f;
                        int currentFrameRun = 0;

                        double startRunningTimeRun = GetTime();
                        bool runningAnimationCompleteRun = false;

                        float platformsScrollOffset = 0.0f;
                        float platformsScrollSpeed = 150.0f;

                        float mountainsScrollOffset = 0.0f;
                        float mountainsScrollSpeed = 50.0f;

                        float farMountainsScrollOffset = 0.0f;
                        float farMountainsScrollSpeed = 20.0f;

                        float rectangleOpacity = 1.0f;

                        // Textos sequenciais
                        const char *texts[] = {
                            "CRÉDITOS",
                            "Agradecemos por termos ido à amostra Tech Design 2024.2",
                            "Somos gratos também a...",
                            "JP reizinho de Orobó e",
                            "Jeraross",
                            "pela competição saudável...",
                            "ficamos felizes em crescer junto com vocês.",
                            "Temos que agradecer especialmente a...",
                            "Micucci do Borel",
                            "Por nos apoiar sempre de perto...",
                            "e se entusiasmar tanto com nosso jogo",
                            "...esse jogo é especial por você também.",
                            "Também agradecer a todos os devs do projeto pelo empenho.",
                            "Mateus Ribeiro - @mateus_ribeir0",
                            "João Pedro Mamede - @jpmamededs",
                            "Carlos Espósito - @carlosesposito22",
                            "Foi muito divertido desenvolver esse jogo...",
                            "E, certamente, ele foi muito importante pra nós...",
                            "Créditos a GiveHeartRecords...",
                            "Samuel Kim Music",
                            "todos os sites de spritesheets...",
                            "e todos os canais de SFX do YouTube...",
                            "Obrigado por jogarem DUNE II"
                        };
                        const int totalTexts = sizeof(texts) / sizeof(texts[0]);
                        int currentTextIndex = 0;
                        double textStartTime3 = GetTime();
                        float textOpacity = 1.0f;
                        const float textDisplayTime = 2.5f;
                        const float textFadeSpeed = 1.0f;

                        // Controlar quando o fundo RGB(210, 83, 76) e sprites devem ser exibidos
                        bool elementsVisible = false;

                        // Retângulo branco
                        float whiteRectangleOpacity = 1.0f;    // Opacidade inicial do retângulo branco
                        double rectangleStartTime = 0.0;

                        // Animação do personagem até o centro
                        while (!runningAnimationCompleteRun) {
                            double elapsedRun = GetTime() - startRunningTimeRun;

                            if (elapsedRun < 15.0) {
                                runningSpriteXRun += runningSpeedRun * GetFrameTime();
                            } else {
                                runningAnimationCompleteRun = true;
                                runningSpriteXRun = finalPositionXRun;
                            }

                            animationTimerRun += GetFrameTime();
                            if (animationTimerRun >= frameTimeRun) {
                                animationTimerRun = 0.0f;
                                currentFrameRun = (currentFrameRun + 1) % totalFramesRun;
                            }

                            BeginDrawing();
                            ClearBackground(BLACK);

                            int frameXRun = currentFrameRun * frameWidthRun;
                            int frameYRun = 128;

                            Rectangle sourceRecRun = { frameXRun, frameYRun, frameWidthRun, frameHeightRun };
                            Rectangle destRecRun = {
                                runningSpriteXRun,
                                runningSpriteYRun,
                                spriteWidthRun,
                                spriteHeightRun
                            };
                            Vector2 originRun = { 0, 0 };

                            DrawTexturePro(personagemRunningRun, sourceRecRun, destRecRun, originRun, 0.0f, WHITE);

                            EndDrawing();
                        }

                        // Controlar o tempo para exibir os elementos adicionais
                        double delayStartTime = GetTime();
                        while (GetTime() - delayStartTime < 0.0) {
                            animationTimerRun += GetFrameTime();
                            if (animationTimerRun >= frameTimeRun) {
                                animationTimerRun = 0.0f;
                                currentFrameRun = (currentFrameRun + 1) % totalFramesRun;
                            }

                            BeginDrawing();
                            ClearBackground(BLACK);

                            int frameXRun = currentFrameRun * frameWidthRun;
                            int frameYRun = 128;

                            Rectangle sourceRecRun = { frameXRun, frameYRun, frameWidthRun, frameHeightRun };
                            Rectangle destRecRun = {
                                finalPositionXRun,
                                runningSpriteYRun,
                                spriteWidthRun,
                                spriteHeightRun
                            };
                            Vector2 originRun = { 0, 0 };

                            DrawTexturePro(personagemRunningRun, sourceRecRun, destRecRun, originRun, 0.0f, WHITE);

                            EndDrawing();
                        }

                        elementsVisible = true;

                        // Loop principal
                        while (!WindowShouldClose()) {
                            // Atualizar offsets e animações
                            farMountainsScrollOffset += farMountainsScrollSpeed * GetFrameTime();
                            farMountainsScrollOffset = fmod(farMountainsScrollOffset, farMountainsSpriteWidth);

                            mountainsScrollOffset += mountainsScrollSpeed * GetFrameTime();
                            mountainsScrollOffset = fmod(mountainsScrollOffset, mountainsSpriteWidth);

                            platformsScrollOffset += platformsScrollSpeed * GetFrameTime();
                            platformsScrollOffset = fmod(platformsScrollOffset, platformsSpriteWidthRun);

                            animationTimerRun += GetFrameTime();
                            if (animationTimerRun >= frameTimeRun) {
                                animationTimerRun = 0.0f;
                                currentFrameRun = (currentFrameRun + 1) % totalFramesRun;
                            }

                            // Atualizar exibição dos textos
                            double elapsedTextTime = GetTime() - textStartTime3;
                            if (elapsedTextTime > textDisplayTime) {
                                textOpacity -= textFadeSpeed * GetFrameTime(); // Diminuir opacidade gradualmente

                                if (textOpacity <= 0.0f) {
                                    currentTextIndex++;
                                    if (currentTextIndex < totalTexts) {
                                        textStartTime3 = GetTime(); // Reiniciar o tempo para o próximo texto
                                        textOpacity = 1.0f;        // Restaurar opacidade
                                    }
                                }
                            }

                            BeginDrawing();
                            ClearBackground((Color){210, 83, 76, 255}); // Fundo na cor definida

                            // Desenhar os elementos do fundo e plataformas
                            for (float x = -farMountainsScrollOffset; x < GetScreenWidth(); x += farMountainsSpriteWidth) {
                                Rectangle sourceRec = {0, 0, farMountainsSprite.width, farMountainsSprite.height};
                                Rectangle destRec = {x, farMountainsSpriteY, farMountainsSpriteWidth, farMountainsSpriteHeight};
                                Vector2 origin = {0, 0};
                                DrawTexturePro(farMountainsSprite, sourceRec, destRec, origin, 0.0f, WHITE);
                            }

                            for (float x = -mountainsScrollOffset; x < GetScreenWidth(); x += mountainsSpriteWidth) {
                                Rectangle sourceRec = {0, 0, mountainsSprite.width, mountainsSprite.height};
                                Rectangle destRec = {x, mountainsSpriteY, mountainsSpriteWidth, mountainsSpriteHeight};
                                Vector2 origin = {0, 0};
                                DrawTexturePro(mountainsSprite, sourceRec, destRec, origin, 0.0f, WHITE);
                            }

                            for (float x = -platformsScrollOffset; x < GetScreenWidth(); x += platformsSpriteWidthRun) {
                                Rectangle sourceRecPlatformsRun = {0, 0, platformsSpriteRun.width, platformsSpriteRun.height};
                                Rectangle destRecPlatformsRun = {x, platformsSpriteYRun, platformsSpriteWidthRun, platformsSpriteHeightRun};
                                Vector2 originPlatformsRun = {0, 0};
                                DrawTexturePro(platformsSpriteRun, sourceRecPlatformsRun, destRecPlatformsRun, originPlatformsRun, 0.0f, WHITE);
                            }

                            // Desenhar personagem correndo
                            Rectangle sourceRecRun = {currentFrameRun * frameWidthRun, 128, frameWidthRun, frameHeightRun};
                            Rectangle destRecRun = {finalPositionXRun, runningSpriteYRun, spriteWidthRun, spriteHeightRun};
                            Vector2 originRun = {0, 0};
                            DrawTexturePro(personagemRunningRun, sourceRecRun, destRecRun, originRun, 0.0f, WHITE);

                            // Exibir o texto dos créditos na camada superior
                            if (currentTextIndex < totalTexts) {
                                const char *currentText = texts[currentTextIndex];
                                int fontSize = 40; // Tamanho da fonte aumentado
                                int textWidth = MeasureText(currentText, fontSize);
                                float textX = (GetScreenWidth() - textWidth) / 2;
                                float textY = (GetScreenHeight() / 2) - 100; // Posição ajustada para ficar acima do meio da tela
                                DrawText(currentText, textX, textY, fontSize, Fade(WHITE, textOpacity));
                            }

                            // Desenhar o retângulo branco (fade-out)
                            if (GetTime() - delayStartTime > 0.0) {
                                if (rectangleStartTime == 0.0) {
                                    rectangleStartTime = GetTime();
                                }
                                double elapsedRectangleTime = GetTime() - rectangleStartTime;
                                whiteRectangleOpacity = 1.0f - (float)(elapsedRectangleTime / 1.0f);
                                if (whiteRectangleOpacity < 0.0f) whiteRectangleOpacity = 0.0f;

                                DrawRectangle(0, 0, 1280, 720, Fade(WHITE, whiteRectangleOpacity));
                            }

                            EndDrawing();

                            // Verificar se o som `creditosMusicRun` terminou
                            if (!IsSoundPlaying(creditosMusicRun)) {
                                UnloadTexture(personagemRunningRun);
                                UnloadTexture(platformsSpriteRun);
                                UnloadTexture(mountainsSprite);
                                UnloadTexture(farMountainsSprite);
                                UnloadSound(creditosMusicRun);
                                UnloadTexture(empty_villain);
                                UnloadTexture(villain_sombra);

                                telaVaziaBloqueada = true;
                                *currentScreen = LOBBY;   
                                break;                    
                            }
                        }
                        
                }
            }
        }

        EndDrawing();

    }

    if (isWalking) {
        walkingTimer -= GetFrameTime();
        if (walkingTimer <= 0) {
            isWalking = false;
        }
    }

    // Sprite do personagem
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

    // Sprite do vilão
    Rectangle sourceRecVillain = { 0, 64, 64, 64 };
    Vector2 villainPosition = { (MAPA_LARGURA - 3) * TILE_SIZE, 10 * TILE_SIZE + 8 };
    Rectangle destRecVillain = { villainPosition.x - 48, villainPosition.y - 48, 96, 96 };

    DrawTexturePro(villain_sombra, sourceRecVillain, destRecVillain, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(empty_villain, sourceRecVillain, destRecVillain, (Vector2){0, 0}, 0.0f, WHITE);

    // Mostrar diálogo e retrato
    if (GetTime() - startTime >= 3.0 && !hideDialog) {
        //dialogTriggered = true;

        // Retrato do vilão
        Rectangle villainPortraitDest = { SCREEN_WIDTH - 338, SCREEN_HEIGHT - 418, 288, 288 };
        DrawTexturePro(villain_portrait, (Rectangle){0, 0, villain_portrait.width, villain_portrait.height},
                       villainPortraitDest, (Vector2){0, 0}, 0.0f, WHITE);

        // Caixa de texto
        DrawRectangleRounded((Rectangle){ 50, SCREEN_HEIGHT - 150, SCREEN_WIDTH - 100, 100 }, 0.1f, 16, (Color){ 0, 0, 0, 200 });
        DrawRectangleRoundedLines((Rectangle){ 50, SCREEN_HEIGHT - 150, SCREEN_WIDTH - 100, 100 }, 0.1f, 16, WHITE);

        // Mostrar texto com efeito de digitação
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
                fireballActive = true; // Ativar a Fireball após o diálogo
            }
        }
        DrawText(displayedText, 80, SCREEN_HEIGHT - 120, 20, WHITE);

        if (displayedTextLength == strlen(dialogTexts[currentDialogIndex]) && currentDialogIndex < totalDialogTexts - 1) {
            DrawText("Pressione ENTER para continuar...", SCREEN_WIDTH - 300, SCREEN_HEIGHT - 40, 16, GRAY);
        }
    }

    // Quando o diálogo terminar e a música de batalha começar
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

            // Movimento da Fireball
            if (fireballActive) {
                fireballPositionX -= fireballSpeed * GetFrameTime();
                if (fireballPositionX < -2 * TILE_SIZE) {
                    fireballActive = false; // Desativar a Fireball quando sair do mapa
                    waveInProgress = false; // Finalizar a onda atual
                }
            }

            // Verificar se devemos iniciar a próxima onda
            if (!fireballActive && !waveInProgress) {
                fireballWave++; // Passar para a próxima onda
                if (fireballWave > 2) fireballWave = 1; // Voltar para a primeira onda após a segunda
                fireballPositionX = MAPA_LARGURA * TILE_SIZE; // Reiniciar posição da Fireball
                fireballActive = true; // Ativar a próxima onda
                waveInProgress = true; // A onda está em progresso novamente

                PlaySound(cannonSound);
            }

            // Desenhar WarningSign
            for (int y = 0; y < MAPA_ALTURA; y++) {
                bool shouldDrawWarning = false;

                // Ajustar lógica do WarningSign de acordo com a wave
                if ((fireballWave == 1 && y % 2 == 0) || // Wave 1: Warnings nos tiles pares
                    (fireballWave == 2 && y % 2 != 0)) { // Wave 2: Warnings nos tiles ímpares
                    shouldDrawWarning = true;
                }

                if (shouldDrawWarning) {
                    Vector2 warningPosition = { (MAPA_LARGURA - 1) * TILE_SIZE, y * TILE_SIZE };
                    DrawTextureEx(warningSign, warningPosition, 0.0f, 1.0f, WHITE);
                }
            

                // Desenhar Fireball nos tiles correspondentes à onda
                if (fireballActive) {
                    if ((fireballWave == 1 && y % 2 == 0) || (fireballWave == 2 && y % 2 != 0)) {
                        Rectangle sourceRecFireball = { 0, 0, 68, 9 };
                        Vector2 fireballPosition = { fireballPositionX, y * TILE_SIZE + (TILE_SIZE - 32) / 2 }; // Centralizar na tile
                        Rectangle destRecFireball = { fireballPosition.x, fireballPosition.y, 32 * (68.0f / 9.0f), 32 }; // Proporção de 32 pixels de altura
                        DrawTexturePro(fireballSprite, sourceRecFireball, destRecFireball, (Vector2){0, 0}, 0.0f, WHITE);
                    }
                }
            }
        }
    } else {
        if (!windMusicStarted) {
            PlayMusicStream(windMusic);
            windMusicStarted = true;
        }
        UpdateMusicStream(windMusic);
    }

    if (!initializedWarnings) {
        for (int i = 0; i < TOTAL_EXPLOSIONS; i++) {
            explosionPositions[i] = (Vector2){
                (MAPA_LARGURA / 2 + GetRandomValue(3, 8)) * TILE_SIZE, // Posição mais à direita
                (MAPA_ALTURA / 2 + GetRandomValue(-4, 4)) * TILE_SIZE  // Posição próxima ao centro vertical
            };
            explosionTimers[i] = 0;
            explosionWarnings[i] = false;
            explosionActiveStates[i] = false;
            explosionHitboxes[i] = false;
        }
        explosionSound = LoadSound("static/music/explosionsfx.wav");
        initializedWarnings = true;
    }
    BeginDrawing();
    ClearBackground(BLACK);

    // Fireball e colisão
    for (int y = 0; y < MAPA_ALTURA; y++) {
        if (fireballActive) {
            // Lógica para desenhar a fireball apenas nas posições relevantes
            bool fireballInTile = false;

            if ((fireballWave == 1 && y % 2 == 0) || // Onda 1: Fireball nos tiles pares
                (fireballWave == 2 && y % 2 != 0)) { // Onda 2: Fireball nos tiles ímpares
                fireballInTile = true;
            }

            if (fireballInTile) {
                Rectangle sourceRecFireball = {0, 0, 68, 9};
                Vector2 fireballPosition = {fireballPositionX, y * TILE_SIZE + (TILE_SIZE - 32) / 2};
                Rectangle destRecFireball = {fireballPosition.x, fireballPosition.y, 32 * (68.0f / 9.0f), 32};

                // Verificar colisão com o jogador
                Rectangle playerRec = {
                    empty_player_x * TILE_SIZE,
                    empty_player_y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };

                if (CheckCollisionRecs(playerRec, destRecFireball)) {
                    // Parar músicas e tocar som de game over
                    // Parar músicas e tocar som de game over
                    StopMusicStream(windMusic);
                    StopMusicStream(battleMusic);
                    PlaySound(gameOverSoundWar);

                    // Sequência de morte com fade-in do texto "GAME OVER"
                    double startBlackTime = GetTime();
                    float opacity = 0.0f; // Opacidade inicial do texto

                    while (opacity < 1.0f) {
                        double elapsed = GetTime() - startBlackTime;

                        // Incrementar opacidade gradualmente
                        opacity = elapsed / 2.0f; // 2 segundos para atingir opacidade máxima
                        if (opacity > 1.0f) opacity = 1.0f;

                        BeginDrawing();
                        ClearBackground(BLACK);

                        sleep(1);

                        // Desenhar texto "GAME OVER" com opacidade
                        DrawTextEx(
                            GetFontDefault(),
                            "GAME OVER",
                            (Vector2){
                                (GetScreenWidth() - MeasureText("GAME OVER", 80)) / 2,
                                GetScreenHeight() / 2 - 40
                            },
                            80, // Tamanho da fonte
                            2,  // Espaçamento
                            Fade(WHITE, opacity) // Aplicar opacidade ao texto
                        );

                        EndDrawing();
                    }

                    // Esperar 1 segundo com o texto visível antes de continuar
                    sleep(1);

                    // Reiniciar jogo
                    atualizarRanking(playerName, playerMoney);
                    zerarMonetaria();
                    resetarJogo();
                    *currentScreen = RANKINGS;
                    return;
                }
            }
        }
    }

    if (battleMusicStarted) { // Garantir que as ocorrências só comecem após a música
        for (int i = 0; i < TOTAL_EXPLOSIONS; i++) {
            double currentTime = GetTime();

            // Ativar Warning após intervalo de 4 segundos
            if (!explosionWarnings[i] && !explosionActiveStates[i]) {
                if (explosionTimers[i] == 0) {
                    explosionTimers[i] = currentTime + i * 4.0; // Intervalo de 4 segundos entre os warnings
                }
                if (currentTime >= explosionTimers[i]) {
                    explosionWarnings[i] = true;
                }
            }

            // Gerenciar Warnings
            if (explosionWarnings[i]) {
                if (currentTime - explosionTimers[i] <= 1.5) {
                    // Desenhar Warning
                    DrawTextureEx(warningSign, explosionPositions[i], 0.0f, 2.0f, WHITE);
                } else if (!explosionActiveStates[i]) {
                    // Após 1.5 segundos, ativar a explosão
                    explosionWarnings[i] = false;
                    explosionActiveStates[i] = true;
                    explosionTimers[i] = currentTime; // Registrar o tempo inicial da explosão
                }
            }

            // Gerenciar Explosões
            if (explosionActiveStates[i]) {
                double elapsedTime = currentTime - explosionTimers[i];
                int frameWidth = 96, frameHeight = 96, totalFrames = 1152 / frameWidth;
                int currentFrame = (int)((elapsedTime / 1.5) * totalFrames);

                if (currentFrame < totalFrames) {
                    if (currentFrame == 0 && !IsSoundPlaying(explosionSound)) {
                        PlaySound(explosionSound);
                    }

                    Rectangle sourceRecExplosion = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
                    Rectangle destRecExplosion = {
                        explosionPositions[i].x + TILE_SIZE - 96 / 2,
                        explosionPositions[i].y + TILE_SIZE - 96 / 2,
                        192,
                        192
                    };
                    DrawTexturePro(explosionSprite, sourceRecExplosion, destRecExplosion, (Vector2){0, 0}, 0.0f, WHITE);

                    // Ativar a hitbox 0.2 segundos após o início da explosão
                    if (elapsedTime >= 0.2) {
                        explosionHitboxes[i] = true;
                    }
                } else {
                    // Finalizar explosão e reiniciar ciclo
                    explosionActiveStates[i] = false;
                    explosionHitboxes[i] = false; // Desativar hitbox definitivamente
                    explosionTimers[i] = currentTime + TOTAL_EXPLOSIONS * 4.0; // Agendar próximo ciclo
                }
            }

            // Verificar colisão com a hitbox da explosão
            if (explosionHitboxes[i]) {
                Rectangle explosionRec = {
                    explosionPositions[i].x + TILE_SIZE - 96 / 2,
                    explosionPositions[i].y + TILE_SIZE - 96 / 2,
                    192,
                    192
                };

                Rectangle playerRec = {
                    empty_player_x * TILE_SIZE,
                    empty_player_y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };

                if (CheckCollisionRecs(playerRec, explosionRec)) {
                    // Parar músicas e tocar som de game over
                    StopMusicStream(windMusic);
                    StopMusicStream(battleMusic);
                    PlaySound(gameOverSoundWar);

                    // Sequência de morte com fade-in do texto "GAME OVER"
                    double startBlackTime = GetTime();
                    float opacity = 0.0f; // Opacidade inicial do texto

                    while (opacity < 1.0f) {
                        double elapsed = GetTime() - startBlackTime;

                        // Incrementar opacidade gradualmente
                        opacity = elapsed / 2.0f; // 2 segundos para atingir opacidade máxima
                        if (opacity > 1.0f) opacity = 1.0f;

                        BeginDrawing();
                        ClearBackground(BLACK);

                        // Desenhar texto "GAME OVER" com opacidade
                        DrawTextEx(
                            GetFontDefault(),
                            "GAME OVER",
                            (Vector2){
                                (GetScreenWidth() - MeasureText("GAME OVER", 80)) / 2,
                                GetScreenHeight() / 2 - 40
                            },
                            80, // Tamanho da fonte
                            2,  // Espaçamento
                            Fade(WHITE, opacity) // Aplicar opacidade ao texto
                        );

                        EndDrawing();
                    }

                    // Esperar 1 segundo com o texto visível antes de continuar
                    sleep(1);

                    // Reiniciar jogo
                    atualizarRanking(playerName, playerMoney);
                    zerarMonetaria();
                    resetarJogo();
                    *currentScreen = RANKINGS;
                    return;
                }
            }
        }
    }

    EndDrawing();
}


void drawLobby() {
    BeginDrawing();


    static bool isNearNpc1 = false;               // Indica se o jogador está próximo ao NPC1
    static bool isNearNpc2 = false;               // Indica se o jogador está próximo ao NPC2
    static char displayedMessage1[256] = "";      // Mensagem atualmente exibida para NPC1
    static char displayedMessage2[256] = "";      // Mensagem atualmente exibida para NPC2
    static int messageLength1 = 0;                // Número de caracteres exibidos para NPC1
    static int messageLength2 = 0;                // Número de caracteres exibidos para NPC2
    static double messageStartTime1 = 0;          // Tempo inicial da mensagem para NPC1
    static double messageStartTime2 = 0;          // Tempo inicial da mensagem para NPC2

    const char *npcMessage1 = "Parece que você anda coletando muitas\nespeciarias, cuidado, você pode acabar\ngerando uma guerra com os Zarnax's...";
    const char *npcMessage2 = "Ola principe de Arrakis, você não\nparece estar com uma boa sorte hoje..."; // Mensagem para o segundo NPC

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
    Rectangle npcslobby = { 0, 0, 64, 64 };
    Rectangle npcslobby2 = { 0, 128, 64, 64 };
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

    if (IsMusicStreamPlaying(warMusic)) {
        StopMusicStream(lobbyMusic); // Garantir que lobbyMusic não toque enquanto warMusic toca
    } else if (!IsMusicStreamPlaying(lobbyMusic)) {
        PlayMusicStream(lobbyMusic);
    }
    UpdateMusicStream(lobbyMusic);

    // Desenhar a sprite do NPC no centro do mapa com tamanho igual ao sprite do jogador
    Vector2 npcCenterPosition = { 
        (MAPA_LARGURA * TILE_SIZE) / 2 - 48,  // Centralizar horizontalmente, ajustando para tamanho 96x96
        (MAPA_ALTURA * TILE_SIZE) / 2 - 48   // Centralizar verticalmente
    };
    Rectangle npcSourceRec = { 0, 0, 64, 64 }; // Fonte da textura
    Rectangle npcDestRec = { npcCenterPosition.x+300, npcCenterPosition.y-120, 96, 96 }; // Destino com tamanho do jogador

    DrawTexturePro(npcslobbyTexture, npcSourceRec, npcDestRec, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle npcSourceRec2 = { 0, 128, 64, 64 }; // Fonte da textura
    Rectangle npcDestRec2 = { npcCenterPosition.x-320, npcCenterPosition.y+180, 96, 96 }; // Destino com tamanho do jogador

    DrawTexturePro(npcslobbyTexture, npcSourceRec2, npcDestRec2, (Vector2){0, 0}, 0.0f, WHITE);



    npcslobbyCollisionBox = (Rectangle){npcCenterPosition.x + 348, npcCenterPosition.y - 64, 4, 0};
    npcslobby2CollisionBox = (Rectangle){npcCenterPosition.x - 300, npcCenterPosition.y + 220, 2, 0};

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

    if (IsKeyPressed(KEY_W) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
        lastDirection = 1;
        isWalking = true;
        walkingTimer = 0.3f;
    }
    else if (IsKeyPressed(KEY_A) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
        lastDirection = 2;
        isWalking = true;
        walkingTimer = 0.3f;
    }
    else if (IsKeyPressed(KEY_S) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
        lastDirection = 3;
        isWalking = true;
        walkingTimer = 0.3f;
    }
    else if (IsKeyPressed(KEY_D) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
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

    if (isPlayerNearMerchant() && IsMusicStreamPlaying(lobbyMusic)) {
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

            if (usandoControle) {
                DrawDialogBoxWithButtons("Ola viajante, o que podemos negociar hoje?\n\n[X] para vender especiarias\n[Y] para comprar uma bolsa nova\n[B] para comprar garrafa de agua\n[A] para comprar itens de sorte",
                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.30);
            } else {
                DrawDialogBox("Olá viajante, o que podemos negociar hoje?\n\n[1]  para vender especiarias\n[2] para comprar uma bolsa nova\n[3] para comprar garrafa de água\n[4] para comprar itens de sorte",
                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
            }

            if (IsKeyPressed(KEY_ONE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) isInteractingWithMerchant = 1;
            else if (IsKeyPressed(KEY_TWO) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) isInteractingWithMerchant = 2;
            else if (IsKeyPressed(KEY_THREE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) isInteractingWithMerchant = 3;
            else if (IsKeyPressed(KEY_FOUR) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) isInteractingWithMerchant = 4;

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
                        while (true) {
                            UpdateMusicStream(lobbyMusic);
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);

                            if (usandoControle) {
                                DrawDialogBoxWithButtons("Obrigado pela venda, espero que prospere!\n\n\n\nAperte [B]para voltar",
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                            } else {
                                DrawDialogBox("Obrigado pela venda, espero que prospere!\n\n\n\nAperte [ENTER] para voltar", 
                                        100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);    
                            }

                            EndDrawing();

                            if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                        }
                        merchantMessage = 0;
                        merchantMood = 0; 
                        isInteractingWithMerchant = 0;
                    } else if (merchantMessage == -1) {
                        while (true) {
                            UpdateMusicStream(lobbyMusic);
                            BeginDrawing();
                            DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);

                            if (usandoControle) {
                                DrawDialogBoxWithButtons("Saia daqui, voce nao tem nenhuma especiaria para\nnegociar!\n\n\nAperte [B]para voltar",
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                            } else {
                                DrawDialogBox("Saia daqui, você não tem nenhuma especiaria para\nnegociar!\n\n\nAperte [ENTER] para voltar", 
                                        100, 550, widthMercador, heigthMercador, WHITE, RED, false);   
                            }
                            
                            EndDrawing();

                            if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                        }
                        merchantMessage = 0;
                        merchantMood = 0; 
                        isInteractingWithMerchant = 0;
                    }
                break;

                case 2:
                    if (usandoControle) {
                        DrawDialogBoxWithButtons("Qual bolsa deseja comprar?\n\n[A]Media (12 especiarias) - 5000\n[X]Grande (24 especiarias) - 10000\n[Y]Super (32 especiarias) - 15000",
                                100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.30);
                    } else {
                        DrawDialogBox("Qual bolsa deseja comprar?\n\n[1]  Média (12 especiarias) - 5000\n[2] Grande (24 especiarias) - 10000\n[3] Super (32 especiarias) - 15000",
                                100, 550, widthMercador, heigthMercador, WHITE, BLACK, false); 
                    }

                    if (IsKeyPressed(KEY_ONE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) { 
                        if (playerMoney >= 5000) {
                            MAX_ESPECIARIAS = 12;
                            playerMoney -= 5000;
                            merchantMood = 1; 

                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua nova bolsa\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte[ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) { 
                        if (playerMoney >= 10000) {
                            MAX_ESPECIARIAS = 24;
                            playerMoney -= 10000;
                            merchantMood = 1; 

                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua nova bolsa\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte[ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) { 
                        if (playerMoney >= 15000) {
                            MAX_ESPECIARIAS = 32;
                            playerMoney -= 15000;
                            merchantMood = 1; 

                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua nova bolsa\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua nova bolsa\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte[ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    }
                break;

                case 3:
                    if (usandoControle) {
                        DrawDialogBoxWithButtons("Qual garrafa de agua deseja comprar?\n\n[A]Pequena (10%) - 3000\n[X]Media (20%) - 5000\n[Y]Grande (30%) - 7000",
                                100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.30);
                    } else {
                        DrawDialogBox("Qual garrafa de água deseja comprar?\n\n[1]  Pequena (10%) - 3000\n[2] Média (20%) - 5000\n[3] Grande (30%) - 7000",
                                100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    }

                    if (IsKeyPressed(KEY_ONE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                        if (playerMoney >= 3000) {
                            playerWater = fmin(playerWater + GARRAFA_PEQUENA_CAPACIDADE, 100);
                            playerMoney -= 3000;
                            merchantMood = 1; 

                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua agua.\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua água.\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) { 
                        if (playerMoney >= 5000) {
                            playerWater = fmin(playerWater + GARRAFA_MEDIA_CAPACIDADE, 100);
                            playerMoney -= 5000;
                            merchantMood = 1; 
                            
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua agua\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua água\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
                        if (playerMoney >= 7000) {
                            playerWater = fmin(playerWater + GARRAFA_GRANDE_CAPACIDADE, 100);
                            playerMoney -= 7000;
                            merchantMood = 1; 

                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE); 
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua agua\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua água\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0; 
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    }
                break;

                case 4:
                    if (usandoControle) {
                        DrawDialogBoxWithButtons("Qual item de sorte deseja comprar?\n\n[A] Ferradura (20%) - 3000\n[X] Amuleto (30%) - 7000\n[Y] Trevo (50%) - 10000",
                                100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.30);
                    } else {
                        DrawDialogBox("Qual item de sorte deseja comprar?\n\n[1] Ferradura (20%) - 3000\n[2] Amuleto (30%) - 7000\n[3] Trevo (50%) - 10000",
                                100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                    }
                    
                    if (IsKeyPressed(KEY_ONE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                        if (playerMoney >= FERRADURA_PRECO) {
                            playerLucky = FERRADURA_SORTE;
                            playerMoney -= FERRADURA_PRECO;
                            merchantMood = 1;

                            while (true) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua sorte\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua sorte\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) { 
                        if (playerMoney >= AMULETO_PRECO) {
                            playerLucky = AMULETO_SORTE;
                            playerMoney -= AMULETO_PRECO;
                            merchantMood = 1;

                            while (true) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua sorte\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua sorte\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) { 
                        if (playerMoney >= TREVO_PRECO) {
                            playerLucky = TREVO_SORTE;
                            playerMoney -= TREVO_PRECO;
                            merchantMood = 1;

                            while (true) {
                                UpdateMusicStream(lobbyMusic);
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoFeliz, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Obrigado pela compra! Aproveite sua sorte\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Obrigado pela compra! Aproveite sua sorte\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, BLACK, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        } else {
                            merchantMood = 2; 
                            while (true) {
                                UpdateMusicStream(lobbyMusic); 
                                BeginDrawing();
                                DrawTexturePro(velho, sourceRecVelhoPuto, destRecVelho, originVelho, 0.0f, WHITE);
                                if (usandoControle) {
                                    DrawDialogBoxWithButtons("Voce nao tem dinheiro suficiente para essa compra\n\n\n\nAperte [B]para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, usandoControle, controlesTexture, botaoAreas, 1.50);
                                } else {
                                    DrawDialogBox("Você não tem dinheiro suficiente para essa compra\n\n\n\nAperte [ENTER] para voltar",
                                            100, 550, widthMercador, heigthMercador, WHITE, RED, false);
                                }
                                EndDrawing();
                                if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) break;
                            }
                            isInteractingWithMerchant = 0;
                            merchantMood = 0;
                        }
                    }
                break;
            }   
        }
    } else if (mensagem != NULL) {
        DrawDialogBoxWithButtons(mensagem, 110, 550, 400, 110, WHITE, BLACK, usandoControle, controlesTexture, botaoAreas, 1.50);
    } else if (telaVaziaBloqueada){
        isInteractingWithMerchant = 0;
    } else {
        isInteractingWithMerchant = 0;
    }
    
    verificarProximidadePoco(player_x * TILE_SIZE, player_y * TILE_SIZE);

    Vector2 playerPosition = { player_x * TILE_SIZE, player_y * TILE_SIZE };

    // Define a área de proximidade para NPC1
    Rectangle npcProximityBox1 = {
        npcslobbyCollisionBox.x - TILE_SIZE,
        npcslobbyCollisionBox.y - TILE_SIZE,
        npcslobbyCollisionBox.width + (TILE_SIZE * 2),
        npcslobbyCollisionBox.height + (TILE_SIZE * 2)
    };

    // Define a área de proximidade para NPC2
    Rectangle npcProximityBox2 = {
        npcslobby2CollisionBox.x - TILE_SIZE,
        npcslobby2CollisionBox.y - TILE_SIZE,
        npcslobby2CollisionBox.width + (TILE_SIZE * 2),
        npcslobby2CollisionBox.height + (TILE_SIZE * 2)
    };

    // Verificar proximidade do jogador com NPC1
    if (CheckCollisionRecs((Rectangle){playerPosition.x, playerPosition.y, TILE_SIZE, TILE_SIZE}, npcProximityBox1)) {
        if (!isNearNpc1) {
            isNearNpc1 = true;
            messageLength1 = 0;
            messageStartTime1 = GetTime();
        }

        if (IsKeyPressed(KEY_SPACE)) {
            messageLength1 = strlen(npcMessage1);
            strncpy(displayedMessage1, npcMessage1, messageLength1);
            displayedMessage1[messageLength1] = '\0';
        } else if (messageLength1 < strlen(npcMessage1)) {
            if (GetTime() - messageStartTime1 >= 0.1) {
                messageLength1++;
                messageStartTime1 = GetTime();
            }
            strncpy(displayedMessage1, npcMessage1, messageLength1);
            displayedMessage1[messageLength1] = '\0';
        }

        // Adicionar a sprite "homem.png" ao lado da caixa de diálogo para NPC1
        Rectangle spriteSourceRec = { 0, 0, homemTexture.width, homemTexture.height };
        Rectangle spriteDestRec = { 88, 390, homemTexture.width * 0.5, homemTexture.height * 0.5 };
        DrawTexturePro(homemTexture, spriteSourceRec, spriteDestRec, (Vector2){0, 0}, 0.0f, WHITE);

        // Exibir mensagem para NPC1
        DrawDialogBox(displayedMessage1, 100, 550, 400, 110, WHITE, BLACK, true);

    } else {
        isNearNpc1 = false;
        messageLength1 = 0;
        displayedMessage1[0] = '\0';
    }

    // Verificar proximidade do jogador com NPC2
    if (CheckCollisionRecs((Rectangle){playerPosition.x, playerPosition.y, TILE_SIZE, TILE_SIZE}, npcProximityBox2)) {
        if (!isNearNpc2) {
            isNearNpc2 = true;
            messageLength2 = 0;
            messageStartTime2 = GetTime();
        }

        if (IsKeyPressed(KEY_SPACE)) {
            messageLength2 = strlen(npcMessage2);
            strncpy(displayedMessage2, npcMessage2, messageLength2);
            displayedMessage2[messageLength2] = '\0';
        } else if (messageLength2 < strlen(npcMessage2)) {
            if (GetTime() - messageStartTime2 >= 0.1) {
                messageLength2++;
                messageStartTime2 = GetTime();
            }
            strncpy(displayedMessage2, npcMessage2, messageLength2);
            displayedMessage2[messageLength2] = '\0';
        }

        // Adicionar a sprite "mulher.png" ao lado da caixa de diálogo para NPC2
        Rectangle spriteSourceRec = { 0, 0, mulherTexture.width, mulherTexture.height };
        Rectangle spriteDestRec = { 88, 390, mulherTexture.width * 0.5, mulherTexture.height * 0.5 };
        DrawTexturePro(mulherTexture, spriteSourceRec, spriteDestRec, (Vector2){0, 0}, 0.0f, WHITE);

        // Exibir mensagem para NPC2
        DrawDialogBox(displayedMessage2, 100, 550, 400, 110, WHITE, BLACK, true);

    } else {
        isNearNpc2 = false;
        messageLength2 = 0;
        displayedMessage2[0] = '\0';
    }

    EndDrawing();
}