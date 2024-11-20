#include "game.h"
#include "lobby.h"

static Texture2D personagem;
static Texture2D sandRuins2;
static Texture2D sandRuins3;
static Texture2D sandRuins4;
static Texture2D personagemAndando;
static Texture2D cerealsTexture;
static Texture2D ruinasDeAreiaGrandes;
static Texture2D environment1_1;
static Texture2D environment1_2;
static Texture2D environment2_1;
static Texture2D environment2_2;
static Texture2D environment3_1;
static Texture2D environment3_2;
static Texture2D safezone;
static Texture2D bonesTexture;
static Texture2D ruinasDeAreiaPequenas;
static Texture2D goldTexture;
static Texture2D aguaTexture;
static Texture2D characterBack;
static Texture2D sandworm;
static Sound musicaMapa0;
static Sound musicaMapa1;
static Sound musicaMapa2;
static Sound spellCastSound;
static Sound gameOverSound;
static Sound barulhoMonstro;
static Sound deathEmotiva;
static Texture2D portal;
static Texture2D sombra;
static Texture2D map0;
static Sound monsterGrowl2;
static bool isMonsterActive = false;
static double monsterStartTime = 0.0;
static double lastEPressTime = 0.0;
static int ePressCount = 0;
static Texture2D EpressSprite;
static float spriteAnimationTimer = 0.0f;
static int spriteFrameIndex = 0;
static double mapaEntradaTime = 0.0;
static Texture2D monsters;
static double nextMonsterCheckTime = 0.0;
static int lastMap = -1;
static Sound heartbeatSound;
static bool isHeartbeatPlaying = false;

#define NUM_ITEMS 5
#define MAX_HISTORICO 1000
#define MAX_PADRAO 9
#define TOTAL_ESPECIARIAS 15

int playerMoney = 0;
int deathEmotivaTocando;

Rectangle cerealsSourceRec = { 64, 64, 32, 32 };

Texture2D loadingImagesMap0[4];
Texture2D loadingImagesMap1[4];
Texture2D loadingImagesMap2[4];
Texture2D loadingImagesLobby[4];

const float loadingImageDisplayTimes[4] = {2.5f, 3.0f, 3.5f, 4.0f};  

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point position;
    bool collected;
} Item;

Item items[NUM_ITEMS];
char historico[MAX_HISTORICO] = "";
int passosRepetidosMax = 3;
static float portalAnimationTimer = 0.0f;
static int portalFrameIndex = 0;

int frameAtual = 0;
float tempoAnimacao = 0;
const float duracaoFrame = 0.2f;

Point posicoesEspeciarias[TOTAL_ESPECIARIAS] = {
    {20, 4}, {8, 6}, {12, 11}, {17, 8}, {23, 12}, {28, 5}, {32, 14}, 
    {36, 17}, {6, 15}, {15, 16}, {20, 7}, {26, 18}, {30, 10}, {33, 6}, {37, 13}
};

int indiceEspeciariaAtual = 0;

void initializeLoadingScreen() {
    loadingImagesMap0[0] = LoadTexture("static/image/mapa0.1.png");
    loadingImagesMap0[1] = LoadTexture("static/image/mapa0.2.png");
    loadingImagesMap0[2] = LoadTexture("static/image/mapa0.3.png");
    loadingImagesMap0[3] = LoadTexture("static/image/mapa0.4.png");

    loadingImagesMap1[0] = LoadTexture("static/image/mapa1.1.png");
    loadingImagesMap1[1] = LoadTexture("static/image/mapa1.2.png");
    loadingImagesMap1[2] = LoadTexture("static/image/mapa1.3.png");
    loadingImagesMap1[3] = LoadTexture("static/image/mapa1.4.png");

    loadingImagesMap2[0] = LoadTexture("static/image/mapa2.1.png");
    loadingImagesMap2[1] = LoadTexture("static/image/mapa2.2.png");
    loadingImagesMap2[2] = LoadTexture("static/image/mapa2.3.png");
    loadingImagesMap2[3] = LoadTexture("static/image/mapa2.4.png");

    loadingImagesLobby[0] = LoadTexture("static/image/cutsceneLobbyColored1.png");
    loadingImagesLobby[1] = LoadTexture("static/image/cutsceneLobbyColored2.png");
    loadingImagesLobby[2] = LoadTexture("static/image/cutsceneLobbyColored3.png");
    loadingImagesLobby[3] = LoadTexture("static/image/cutsceneLobbyColored4.png");

}

void iniciarGame() {
    personagem = LoadTexture("static/image/newstoppedsprites.png");
    sandRuins2 = LoadTexture("static/image/Sand_ruins2.png");
    sandRuins3 = LoadTexture("static/image/Sand_ruins3.png");
    bonesTexture = LoadTexture("static/image/bones.png");
    sandRuins4 = LoadTexture("static/image/Sand_ruins4.png");
    personagemAndando = LoadTexture("static/image/newwalkingsprites.png");
    cerealsTexture = LoadTexture("static/image/Cereals.png");
    ruinasDeAreiaGrandes = LoadTexture("static/image/Sand_ruins1.png");
    environment1_1 = LoadTexture("static/image/Rock6_1.png");
    environment1_2 = LoadTexture("static/image/Rock6_3.png");
    environment2_1 = LoadTexture("static/image/Rock2_1.png");
    environment2_2 = LoadTexture("static/image/Rock2_3.png");
    environment3_1 = LoadTexture("static/image/finallevel.png");
    environment3_2 = LoadTexture("static/image/Rock8_3.png");
    safezone = LoadTexture ("static/image/desert_tileset2.png");
    ruinasDeAreiaPequenas = LoadTexture("static/image/Sand_ruins5.png");
    goldTexture = LoadTexture("static/image/gold.png");
    aguaTexture = LoadTexture("static/image/agua.png");
    characterBack = LoadTexture("static/image/characterback.png");
    sandworm = LoadTexture("static/image/sandworm.png");
    portal = LoadTexture("static/image/portal.png");
    sombra = LoadTexture("static/image/sombras.png");
    map0 = LoadTexture("static/image/map0.png");
    EpressSprite = LoadTexture("static/image/Epress.png");
    monsters = LoadTexture("static/image/monsters.png");

    musicaMapa0 = LoadSound("static/music/mapa0musica.mp3");
    SetSoundVolume(musicaMapa0, 0.7f);
    musicaMapa1 = LoadSound("static/music/mapa1musica.mp3");
    SetSoundVolume(musicaMapa1, 0.6f);
    musicaMapa2 = LoadSound("static/music/mapa2musica.mp3");
    SetSoundVolume(musicaMapa2, 0.6f);
    spellCastSound = LoadSound("static/music/spellcast.mp3");
    gameOverSound = LoadSound("static/music/deathsfx2.wav");
    barulhoMonstro = LoadSound("static/music/monster.mp3");
    deathEmotiva = LoadSound("static/music/deathemotiva.mp3");
    SetSoundVolume(musicaMapa2, 0.8f);
    monsterGrowl2 = LoadSound("static/music/monsterGrowl2.wav");
    heartbeatSound = LoadSound("static/music/heartbeat.wav");
    SetSoundVolume(heartbeatSound, 1.0f);


    initializeLoadingScreen();
}

void finalizarGame() {
    UnloadTexture(personagem);
    UnloadTexture(sandRuins2);
    UnloadTexture(sandRuins3);
    UnloadTexture(sandRuins4);
    UnloadTexture(personagemAndando);
    UnloadTexture(cerealsTexture);
    UnloadTexture(ruinasDeAreiaGrandes);
    UnloadTexture(environment1_1);
    UnloadTexture(environment1_2);
    UnloadTexture(environment2_1);
    UnloadTexture(environment2_2);
    UnloadTexture(environment3_1);
    UnloadTexture(environment3_2);
    UnloadTexture(ruinasDeAreiaPequenas);
    UnloadTexture(goldTexture);
    UnloadTexture(aguaTexture);
    UnloadTexture(characterBack);
    UnloadTexture(sandworm);
    UnloadTexture(portal);
    UnloadTexture(map0);
    UnloadTexture(EpressSprite);

    UnloadSound(musicaMapa0);
    UnloadSound(musicaMapa1);
    UnloadSound(musicaMapa2);
    UnloadSound(spellCastSound);
    UnloadSound(gameOverSound);
    UnloadSound(barulhoMonstro);
    UnloadSound(deathEmotiva);
    UnloadSound(monsterGrowl2);
    UnloadSound(heartbeatSound);

    for (int i = 0; i < 4; i++) {
        UnloadTexture(loadingImagesMap0[i]);
        UnloadTexture(loadingImagesMap1[i]);
        UnloadTexture(loadingImagesMap2[i]);
        UnloadTexture(loadingImagesLobby[i]);
    }
}

void showLoadingScreen(Texture2D* loadingImages) {
    for (int i = 0; i < 4; i++) {
        ClearBackground(BLACK);
        DrawTexture(loadingImages[i], 0, 0, WHITE);
        EndDrawing();

        float waitTime = loadingImageDisplayTimes[i];
        float startTime = GetTime();

        while (GetTime() - startTime < waitTime) {
            BeginDrawing();
            DrawTexture(loadingImages[i], 0, 0, WHITE);
            EndDrawing();
        }
    }
}

void inicializarEspeciaria() {
    if (indiceEspeciariaAtual < TOTAL_ESPECIARIAS) {
        items[0].position = posicoesEspeciarias[indiceEspeciariaAtual];
        items[0].collected = false;
    }
}

bool faltamDoisParaSequencia(const char *historico, size_t tamanho_padrao) {
    size_t historico_len = strlen(historico);
    return (historico_len >= tamanho_padrao - 2 && historico_len < tamanho_padrao);
}

void updateWaterLevel(GameScreen *currentScreen) {
    double currentTime = GetTime();
    double interval = LOBBY_WATER_LOSS_INTERVAL;

    if (mapaAtual == 0) {
        interval = MAP1_WATER_LOSS_INTERVAL;
    } else if (mapaAtual == 1) {
        interval = MAP2_WATER_LOSS_INTERVAL;
    } else if (mapaAtual == 2) {
        interval = MAP3_WATER_LOSS_INTERVAL;
    }

    if (currentTime - lastWaterUpdateTime >= interval) {
        playerWater -= 1.0;
        lastWaterUpdateTime = currentTime;

        if (playerWater <= 0.0) {
            playerWater = 0.0;
            *currentScreen = RANKINGS;
        }
    }
}

void checkItemCollection() {
    if (!items[0].collected && items[0].position.x == player_x && items[0].position.y == player_y) {
        if (itemsCollected < MAX_ESPECIARIAS) {
            items[0].collected = true;
            int especiariasGanhas = 0;

            switch (mapaAtual) {
                case 0: especiariasGanhas = 1; break;
                case 1: especiariasGanhas = 2; break;
                case 2: especiariasGanhas = 4; break;
            }

            itemsCollected += especiariasGanhas;
            if (itemsCollected > MAX_ESPECIARIAS) {
                itemsCollected = MAX_ESPECIARIAS;
            }

            indiceEspeciariaAtual++;
            if (indiceEspeciariaAtual < TOTAL_ESPECIARIAS) {
                items[0].position = posicoesEspeciarias[indiceEspeciariaAtual];
                items[0].collected = false;
            }
        } else {
            DrawText("Bolsa cheia! Não é possível coletar mais especiarias.", 10, 30, 20, RED);
        }
    }
}


#define NUM_PEDRAS 12

Vector2 posicoesPedras[NUM_PEDRAS] = {
    {5, 5}, {10, 8}, {15, 12}, {20, 3},
    {25, 18}, {30, 10}, {35, 15}, {40, 5},
    {8, 20}, {12, 25}, {18, 30}, {22, 35}
};

#define DUNAS_MAPA1 5
#define DUNAS_MAPA2 7
#define DUNAS_MAPA3 10

Point posicoesDunasMapa1[DUNAS_MAPA1] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5} };
Point posicoesDunasMapa2[DUNAS_MAPA2] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5}};
Point posicoesDunasMapa3[DUNAS_MAPA3] = {
    {8, 8}, 
    {14, 12},
    {22, 16},
    {4, 18},
    {28, 6},
    {18, 10},
    {10, 14},
    {26, 4},
    {6, 20},
    {16, 2} 
};

Rectangle vendinhaCollisionBox = {20 + (123 * 0.8) / 4, 20 + (120 * 0.8) / 4, (123 * 0.8) / 2, (120 * 0.8) / 2};
Rectangle cityCollisionBoxes[] = {
    {800 + 50, 4 + 30, 120 * 0.5, 200 * 0.4},      
    {890 + 5, 36 + 10, 132 * 0.5, 200 * 0.4},
    {572 + 5, 38 + 10, 132 * 0.5, 200 * 0.4},         
    {1000 + 10, 36 + 10, 150 * 0.7, 200 * 0.4},    
    {300 + 5, 380 + 5 + (138 * 0.8) * 0.85, 90 * 0.7, (138 * 0.8) * 0.1},
    {200 + 5, 376 + 10 + (144 * 0.6) * 0.85, 96 * 0.6, (144 * 0.7) * 0.3},
    {100 + 5, 354 + 10 + (168 * 0.6) * 0.85, 96 * 0.6, (168 * 0.7) * 0.3},
    {120 + 10 + (55 * 0.7) * 0.1, 600 + 5 + (55 * 0.7) * 0.1, (55 * 0.7) * 0.6, (55 * 0.7) * 0.6},
    {18 + 15, 478 + 5, 84 * 0.7, 72 * 0.4},
    {684 + 15, 2 + 30, 138 * 0.8, 200 * 0.4},
    {760 + 10, 200 + 5, 90 * 0.3, 63 * 0.3},


};


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

void movePlayer(int dx, int dy) {
    int new_x = player_x + dx;
    int new_y = player_y + dy;

    Rectangle playerRect = { new_x * TILE_SIZE, new_y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

    if (new_x >= 0 && new_x < MAPA_LARGURA && new_y >= 0 && new_y < MAPA_ALTURA) {
        bool colidiuComDuna = false;
        bool colidiuComPedra = false;
        bool colidiuComCidade = false;

        Rectangle ruinasCollisionBox = { 30, 40, 190, 180 };
        if (mapaAtual == 1 && CheckCollisionRecs(playerRect, ruinasCollisionBox)) {
            return;
        }

        if (isPlayerOnPortal(new_x, new_y, mapaAtual)) {
            return;
        }

        if (mapaAtual == -1) {
            if (CheckCollisionRecs(playerRect, vendinhaCollisionBox)) {
                return;
            }

            for (int i = 0; i < sizeof(cityCollisionBoxes) / sizeof(cityCollisionBoxes[0]); i++) {
                if (CheckCollisionRecs(playerRect, cityCollisionBoxes[i])) {
                    colidiuComCidade = true;
                    break;
                }
            }
        }

        if (mapaAtual == 0) {
            for (int i = 0; i < NUM_PEDRAS; i++) {
                if (new_x == (int)posicoesPedras[i].x && new_y == (int)posicoesPedras[i].y) {
                    colidiuComPedra = true;
                    break;
                }
            }

            for (int i = 0; i < DUNAS_MAPA1; i++) {
                if ((new_x >= posicoesDunasMapa1[i].x && new_x < posicoesDunasMapa1[i].x + 3) &&
                    (new_y >= posicoesDunasMapa1[i].y && new_y < posicoesDunasMapa1[i].y + 2)) {
                    colidiuComDuna = true;
                    break;
                }
            }
        } else if (mapaAtual == 1) {
            for (int i = 0; i < DUNAS_MAPA2; i++) {
                if ((new_x >= posicoesDunasMapa2[i].x && new_x < posicoesDunasMapa2[i].x + 3) &&
                    (new_y >= posicoesDunasMapa2[i].y && new_y < posicoesDunasMapa2[i].y + 2)) {
                    colidiuComDuna = true;
                    break;
                }
            }
        } else if (mapaAtual == 2) {
            for (int i = 0; i < DUNAS_MAPA3; i++) {
                if ((new_x >= posicoesDunasMapa3[i].x && new_x < posicoesDunasMapa3[i].x + 3) &&
                    (new_y >= posicoesDunasMapa3[i].y && new_y < posicoesDunasMapa3[i].y + 2)) {
                    colidiuComDuna = true;
                    break;
                }
            }
        }

        if (!colidiuComDuna && !colidiuComPedra && !colidiuComCidade) {
            player_x = new_x;
            player_y = new_y;
        }
    }
}

void desenharAnimacaoMorte(Texture2D personagem, Texture2D personagemMorto) {
    Rectangle framesMorte[] = {
        {128, 0, 64, 64}, {192, 0, 64, 64},
        {256, 0, 64, 64}, {320, 0, 64, 64}, {384, 0, 64, 64}
    };
    int numFramesMorte = sizeof(framesMorte) / sizeof(framesMorte[0]);

    Vector2 posicao = {player_x * TILE_SIZE, player_y * TILE_SIZE};
    Rectangle destRec = {posicao.x - 32, posicao.y - 32, 96, 96};

    const float duracaoFramePrimeiro = 0.5f;
    const float duracaoFrame = 0.1f; 
    float tempoInicial = GetTime();

    for (int i = 0; i < numFramesMorte; i++) {
        float tempoFrameAtual = (i == 0) ? duracaoFramePrimeiro : duracaoFrame;

        while (GetTime() - tempoInicial < tempoFrameAtual) {
            ClearBackground(BLACK);
            BeginDrawing();
            DrawTexturePro(personagemMorto, framesMorte[i], destRec, (Vector2){0, 0}, 0.0f, WHITE);
            EndDrawing();
        }

        tempoInicial = GetTime();
    }

    DrawTexturePro(personagemMorto, framesMorte[numFramesMorte - 1], destRec, (Vector2){0, 0}, 0.0f, WHITE);
}

int direcao = 0;

void drawGame() {
    ClearBackground(RAYWHITE);

    Color map0Color = (Color){210, 178, 104, 255};
    Color map1Color = (Color){210, 178, 104, 255};
    Color map2Color = (Color){228, 162, 68, 255};

    Vector2 origin = {0, 0};

    Rectangle ruinasSourceRec = {0, 0, ruinasDeAreiaGrandes.width, ruinasDeAreiaGrandes.height};
    Rectangle ruinasDestRec = {20, 20, 256, 256};

    if (mapaAtual == 0) {
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
            DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, map0Color);
        }
    }

    for (int i = 0; i < NUM_PEDRAS; i++) {
        Vector2 posicaoPedra = { posicoesPedras[i].x * TILE_SIZE, posicoesPedras[i].y * TILE_SIZE };
        DrawTexture(sandRuins2, posicaoPedra.x, posicaoPedra.y, RAYWHITE);
    }

    for (int i = 0; i < DUNAS_MAPA1; i++) {
        Vector2 posicaoDuna = { posicoesDunasMapa1[i].x * TILE_SIZE, posicoesDunasMapa1[i].y * TILE_SIZE };
        Rectangle destRect = { posicaoDuna.x, posicaoDuna.y, 96, 96 };
        Rectangle sourceRect = { 0, 0, sandRuins3.width, sandRuins3.height };
        DrawTexturePro(sandRuins3, sourceRect, destRect, origin, 0.0f, WHITE);
    }

    Vector2 baseSafezonePosition1 = {20 * TILE_SIZE, 20 * TILE_SIZE}; 
    Vector2 baseSafezonePosition2 = {10 * TILE_SIZE, 15 * TILE_SIZE}; 
    Vector2 baseSafezonePosition3 = {30 * TILE_SIZE, 5 * TILE_SIZE};  
    Vector2 baseSafezonePosition4 = {25 * TILE_SIZE, 10 * TILE_SIZE}; 
    Rectangle safezoneRec = {376, 136, 32, 32}; 

    Rectangle destRect1 = {baseSafezonePosition1.x, baseSafezonePosition1.y, 96, 64};
    Vector2 origin1 = {0, 0};
    DrawTexturePro(safezone, safezoneRec, destRect1, origin1, 0.0f, RAYWHITE);

    Rectangle destRect2 = {baseSafezonePosition2.x, baseSafezonePosition2.y, 64, 64};
    Vector2 origin2 = {0, 0};
    DrawTexturePro(safezone, safezoneRec, destRect2, origin2, 0.0f, RAYWHITE);

    Rectangle destRect3 = {baseSafezonePosition3.x, baseSafezonePosition3.y, 64, 64};
    Vector2 origin3 = {0, 0};
    DrawTexturePro(safezone, safezoneRec, destRect3, origin3, 0.0f, RAYWHITE);

    Rectangle destRect4 = {baseSafezonePosition4.x, baseSafezonePosition4.y, 64, 96};
    Vector2 origin4 = {0, 0};
    DrawTexturePro(safezone, safezoneRec, destRect4, origin4, 0.0f, RAYWHITE);

    Rectangle map0SourceRect = {51, 59, 24, 24}; 
    Vector2 map0Position = {20 * TILE_SIZE, 10 * TILE_SIZE}; 
    Rectangle map0DestRect = {map0Position.x, map0Position.y, 24, 24}; 
    DrawTexturePro(map0, map0SourceRect, map0DestRect, origin, 0.0f, WHITE);

    Rectangle map0SourceRect2 = {70, 322, 133, 53}; 
    Vector2 map0Position2 = {30 * TILE_SIZE, 16 * TILE_SIZE}; 
    Rectangle map0DestRect2 = {map0Position2.x, map0Position2.y, 133, 53}; 
    DrawTexturePro(map0, map0SourceRect2, map0DestRect2, origin, 0.0f, WHITE);

    Rectangle map0SourceRect3 = {106, 122, 31, 23}; 
    Vector2 map0Position3 = {8 * TILE_SIZE, 5 * TILE_SIZE}; 
    Rectangle map0DestRect3 = {map0Position3.x, map0Position3.y, 31, 23}; 
    DrawTexturePro(map0, map0SourceRect3, map0DestRect3, origin, 0.0f, WHITE);

    Rectangle map0SourceRect4 = {76, 118, 22, 30}; 
    Vector2 map0Position4 = {7 * TILE_SIZE, 4 * TILE_SIZE}; 
    Rectangle map0DestRect4 = {map0Position4.x, map0Position4.y, 22, 30}; 
    DrawTexturePro(map0, map0SourceRect4, map0DestRect4, origin, 0.0f, WHITE);

    Rectangle map0SourceRect5 = {191, 80, 20, 50}; 
    Vector2 map0Position5 = {28 * TILE_SIZE, 6 * TILE_SIZE}; 
    Rectangle map0DestRect5 = {map0Position5.x, map0Position5.y, 20, 50}; 
    DrawTexturePro(map0, map0SourceRect5, map0DestRect5, origin, 0.0f, WHITE);

    Rectangle map0SourceRect6 = {19, 59, 28, 26}; 
    Vector2 map0Position6 = {3 * TILE_SIZE, 19 * TILE_SIZE}; 
    Rectangle map0DestRect6 = {map0Position6.x, map0Position6.y, 28, 26}; 
    DrawTexturePro(map0, map0SourceRect6, map0DestRect6, origin, 0.0f, WHITE);
}

 else if (mapaAtual == 1) {
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
            DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, map1Color);
        }
    }
    Rectangle bonesSourceRect = {774, 113, 38, 33};
    Vector2 bonesPosition = {30 * TILE_SIZE, 10 * TILE_SIZE};
    Rectangle bonesDestRect = {bonesPosition.x, bonesPosition.y, 38, 33}; 
    
    Rectangle bonesSourceRect2 = {739, 117, 25, 28};
    Vector2 bonesPosition2 = {20 * TILE_SIZE, 18 * TILE_SIZE};
    Rectangle bonesDestRect2 = {bonesPosition2.x, bonesPosition2.y, 25, 28};

    Rectangle bonesSourceRect3 = {117, 142, 27, 22};
    Vector2 bonesPosition3 = {10 * TILE_SIZE, 6 * TILE_SIZE};
    Rectangle bonesDestRect3 = {bonesPosition3.x, bonesPosition3.y, 27, 22};

    Rectangle bonesSourceRect4 = {557, 60, 45, 37};
    Vector2 bonesPosition4 = {30 * TILE_SIZE, 3 * TILE_SIZE};
    Rectangle bonesDestRect4 = {bonesPosition4.x, bonesPosition4.y, 45, 37};

    Rectangle bonesSourceRect5 = {557, 60, 45, 37};
    Vector2 bonesPosition5 = {12 * TILE_SIZE, 18 * TILE_SIZE};
    Rectangle bonesDestRect5 = {bonesPosition5.x, bonesPosition5.y, 45, 37};

    Rectangle bonesSourceRect6 = {739, 203, 32, 27};
    Vector2 bonesPosition6 = {22 * TILE_SIZE, 6 * TILE_SIZE};
    Rectangle bonesDestRect6 = {bonesPosition6.x, bonesPosition6.y, 32, 27};

    Rectangle bonesSourceRect7 = {739, 203, 32, 27};
    Vector2 bonesPosition7 = {17 * TILE_SIZE, 12 * TILE_SIZE};
    Rectangle bonesDestRect7 = {bonesPosition7.x, bonesPosition7.y, 32, 27};

    DrawTexturePro(bonesTexture, bonesSourceRect, bonesDestRect, origin, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, bonesSourceRect2, bonesDestRect2, origin, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, bonesSourceRect3, bonesDestRect3, origin, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, bonesSourceRect4, bonesDestRect4, origin, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, bonesSourceRect5, bonesDestRect5, origin, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, bonesSourceRect6, bonesDestRect6, origin, 0.0f, WHITE);
    DrawTexturePro(bonesTexture, bonesSourceRect7, bonesDestRect7, origin, 0.0f, WHITE);

    DrawTexture(environment1_2, 20, 20, RAYWHITE);
    DrawTexturePro(ruinasDeAreiaGrandes, ruinasSourceRec, ruinasDestRec, origin, 0.0f, WHITE);
    DrawTexture(ruinasDeAreiaPequenas, 20, 20, RAYWHITE);

    for (int i = 0; i < DUNAS_MAPA2; i++) {
        Vector2 posicaoDuna = { posicoesDunasMapa1[i].x * TILE_SIZE, posicoesDunasMapa1[i].y * TILE_SIZE };
        Rectangle destRect = { posicaoDuna.x, posicaoDuna.y, 64, 64 };

        if (i % 5 == 0) {

            Rectangle sandRuinsSourceRect3 = {3, 0, 64, 64};
            DrawTexturePro(sandRuins3, sandRuinsSourceRect3, destRect, origin, 0.0f, WHITE);
        } else if (i % 5 == 1 || i % 5 == 2) {
            Rectangle sandRuinsSourceRect2 = {4, 3, 53, 57};
            DrawTexturePro(sandRuins2, sandRuinsSourceRect2, destRect, origin, 0.0f, WHITE);


        } else {
            Rectangle sandRuinsSourceRect4 = {3, 1, 43, 43};
            DrawTexturePro(sandRuins4, sandRuinsSourceRect4, destRect, origin, 0.0f, WHITE);
        }
    }

    Vector2 baseSafezonePosition1_Map1 = {5 * TILE_SIZE, 10 * TILE_SIZE};   
    Vector2 baseSafezonePosition2_Map1 = {35 * TILE_SIZE, 15 * TILE_SIZE}; 
    Rectangle safezoneRec = {376, 136, 32, 32}; 

    Rectangle destRect1_Map1 = {baseSafezonePosition1_Map1.x, baseSafezonePosition1_Map1.y, 96, 64}; 
    Vector2 origin1_Map1 = {0, 0};
    DrawTexturePro(safezone, safezoneRec, destRect1_Map1, origin1_Map1, 0.0f, RAYWHITE);

    Rectangle destRect2_Map1 = {baseSafezonePosition2_Map1.x, baseSafezonePosition2_Map1.y, 64, 96};
    Vector2 origin2_Map1 = {0, 0};
    DrawTexturePro(safezone, safezoneRec, destRect2_Map1, origin2_Map1, 0.0f, RAYWHITE);
}
 else if (mapaAtual == 2) {
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
            DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, map2Color);
        }
    }

    Rectangle sprite1 = { 19, 8, 67, 80 };
    Rectangle sprite2 = { 105, 34, 66, 54 };
    Rectangle sprite3 = { 188, 41, 46, 46 };
    Rectangle sprite4 = { 82, 101, 40, 39 };

    Vector2 collisionPositions[10] = { 
        {8 * TILE_SIZE, 8 * TILE_SIZE}, {14 * TILE_SIZE, 12 * TILE_SIZE},
        {22 * TILE_SIZE, 16 * TILE_SIZE}, {4 * TILE_SIZE, 18 * TILE_SIZE},
        {28 * TILE_SIZE, 6 * TILE_SIZE}, {18 * TILE_SIZE, 10 * TILE_SIZE},
        {10 * TILE_SIZE, 14 * TILE_SIZE}, {26 * TILE_SIZE, 4 * TILE_SIZE},
        {6 * TILE_SIZE, 20 * TILE_SIZE}, {16 * TILE_SIZE, 2 * TILE_SIZE}
    };

    for (int i = 0; i < 10; i++) {
        Rectangle destRect = { collisionPositions[i].x, collisionPositions[i].y, 76, 76 };
        Rectangle sourceRect;

        if (i < 2) {
            sourceRect = sprite1;
        } else if (i < 4) {
            sourceRect = sprite2;  
        } else if (i < 7) {
            sourceRect = sprite3;  
        } else {
            sourceRect = sprite4;  
        }

        DrawTexturePro(environment3_1, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
    }

    Rectangle bonesSourceRect = {145, 426, 42, 41};  
    Vector2 bonesPosition = {30 * TILE_SIZE, 10 * TILE_SIZE};  
    Rectangle bonesDestRect = {bonesPosition.x, bonesPosition.y, 42, 41}; 

    DrawTexturePro(bonesTexture, bonesSourceRect, bonesDestRect, (Vector2){0, 0}, 0.0f, WHITE); 

    Rectangle bonesSourceRect2 = {229, 436, 30, 30};  
    Vector2 bonesPosition2 = {10 * TILE_SIZE, 6 * TILE_SIZE};  
    Rectangle bonesDestRect2 = {bonesPosition2.x, bonesPosition2.y, 30, 30};  

    DrawTexturePro(bonesTexture, bonesSourceRect2, bonesDestRect2, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle bonesSourceRect3 = {229, 436, 30, 30}; 
    Vector2 bonesPosition3 = {15 * TILE_SIZE, 2 * TILE_SIZE};
    Rectangle bonesDestRect3 = {bonesPosition3.x, bonesPosition3.y, 30, 30};

    DrawTexturePro(bonesTexture, bonesSourceRect3, bonesDestRect3, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle bonesSourceRect4 = {387, 387, 38, 30};
    Vector2 bonesPosition4 = {34 * TILE_SIZE, 7 * TILE_SIZE};
    Rectangle bonesDestRect4 = {bonesPosition4.x, bonesPosition4.y, 38, 30};

    DrawTexturePro(bonesTexture, bonesSourceRect4, bonesDestRect4, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle bonesSourceRect5 = {194, 428, 24, 38};
    Vector2 bonesPosition5 = {38 * TILE_SIZE, 18 * TILE_SIZE};
    Rectangle bonesDestRect5 = {bonesPosition5.x, bonesPosition5.y, 24, 38};

    DrawTexturePro(bonesTexture, bonesSourceRect5, bonesDestRect5, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle bonesSourceRect6 = {194, 428, 24, 38}; 
    Vector2 bonesPosition6 = {29 * TILE_SIZE, 20 * TILE_SIZE};
    Rectangle bonesDestRect6 = {bonesPosition6.x, bonesPosition6.y, 24, 38};

    DrawTexturePro(bonesTexture, bonesSourceRect6, bonesDestRect6, (Vector2){0, 0}, 0.0f, WHITE);
}





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

    Rectangle sourceRec;
    switch (lastDirection) {
        case 1:
            sourceRec = (Rectangle){0, 192, 64, 64};
            break;
        case 2:
            sourceRec = (Rectangle){0, 64, 64, 64};
            break;
        case 3:
            sourceRec = (Rectangle){0, 0, 64, 64};
            break;
        case 4:
            sourceRec = (Rectangle){0, 128, 64, 64};
            break;
        default:
            sourceRec = (Rectangle){0, 0, 64, 64};
            break;
    }

    Vector2 position = { player_x * TILE_SIZE, player_y * TILE_SIZE };

    Rectangle destRec = { position.x - 32, position.y - 32, 96, 96 };

    if (isWalking) {
        DrawTexturePro(sombra, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        DrawTexturePro(personagemAndando, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawTexturePro(sombra, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        DrawTexturePro(personagem, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
    }

    if (!items[0].collected) {
        Vector2 itemPosition = { items[0].position.x * TILE_SIZE, items[0].position.y * TILE_SIZE };
        DrawTextureRec(cerealsTexture, cerealsSourceRec, itemPosition, WHITE);
    }

    int infoBoxX = SCREEN_WIDTH - 230;
    int infoBoxY = 10;
    int infoBoxWidth = 220;
    int infoBoxHeight = 100;

    DrawRectangleRounded((Rectangle){infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight}, 0.1f, 16, (Color){205, 133, 63, 255});

    DrawRectangleRoundedLines((Rectangle){infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight}, 0.1f, 16, (Color){101, 67, 33, 255});
    DrawRectangleRoundedLines((Rectangle){infoBoxX + 1, infoBoxY + 1, infoBoxWidth - 2, infoBoxHeight - 2}, 0.1f, 16, (Color){101, 67, 33, 255});
    DrawRectangleRoundedLines((Rectangle){infoBoxX + 2, infoBoxY + 2, infoBoxWidth - 4, infoBoxHeight - 4}, 0.1f, 16, (Color){101, 67, 33, 255});

    int especiariaIconY = infoBoxY + 1;
    Vector2 especiariaIconPos = { infoBoxX + 10, especiariaIconY };
    DrawTextureRec(cerealsTexture, (Rectangle){64, 64, 32, 32}, especiariaIconPos, WHITE);

    DrawText("Especiarias:", infoBoxX + 50, infoBoxY + 10, 18, WHITE);
    DrawText(TextFormat("%d/%d", itemsCollected, MAX_ESPECIARIAS), infoBoxX + 160, infoBoxY + 10, 18, WHITE);

    Vector2 goldIconPos = { infoBoxX + 15, infoBoxY + 35 };
    Rectangle goldSourceRec = { 0, 0, 16, 16 };
    Rectangle goldDestRec = { goldIconPos.x, goldIconPos.y, 24, 24 };

    DrawTexturePro(goldTexture, goldSourceRec, goldDestRec, (Vector2){0, 0}, 0.0f, WHITE);
    DrawText(TextFormat("Dinheiro: %d", playerMoney), infoBoxX + 50, infoBoxY + 40, 18, WHITE);

    Vector2 aguaIconPos = { infoBoxX + 18, infoBoxY + 65 };
    Rectangle aguaSourceRec = { 0, 0, aguaTexture.width, aguaTexture.height };
    Rectangle aguaDestRec = { aguaIconPos.x, aguaIconPos.y, 24, 24 };

    DrawTexturePro(aguaTexture, aguaSourceRec, aguaDestRec, (Vector2){0, 0}, 0.0f, WHITE);
    DrawText(TextFormat("Água: %.0f%%", playerWater), infoBoxX + 55, infoBoxY + 70, 18, WHITE);

    portalAnimationTimer += GetFrameTime();
    if (portalAnimationTimer >= 0.1) {
        portalAnimationTimer = 0.0f;
        portalFrameIndex++;
        if (portalFrameIndex > 3) {
            portalFrameIndex = 0;
        }
    }

    Rectangle portalSourceRec = {portalFrameIndex * 32, 0, 32, 32};
    Rectangle portalDestRec = {PORTAL_RETORNO_X * TILE_SIZE, PORTAL_RETORNO_Y * TILE_SIZE, TILE_SIZE * PORTAL_RETORNO_LARGURA, TILE_SIZE * PORTAL_RETORNO_ALTURA};

    DrawTexturePro(portal, portalSourceRec, portalDestRec, (Vector2){0, 0}, 0.0f, WHITE);

    if (mensagem != NULL) {
        DrawDialogBox(mensagem, 70, 580, 400, 110, WHITE, BLACK, false);
    }
}


int contar_ocorrencias_consecutivas(const char *historico, const char *padrao, size_t padrao_len) {
    int contagem = 0;
    int sequencias_consecutivas = 0;
    size_t historico_len = strlen(historico);

    for (size_t i = 0; i <= historico_len - padrao_len;) {
        if (strncmp(&historico[i], padrao, padrao_len) == 0) {
            sequencias_consecutivas++;
            i += padrao_len;
        } else {
            if (sequencias_consecutivas > 0) {
                contagem += (sequencias_consecutivas > 1) ? sequencias_consecutivas : 0;
                sequencias_consecutivas = 0;
            }
            i++;
        }
    }

    contagem += (sequencias_consecutivas > 1) ? sequencias_consecutivas : 0;
    return contagem;
}

int identificar_padrao_mais_frequente(const char *historico, size_t tamanho_padrao, char *padrao_mais_frequente) {
    char padrao_atual[MAX_PADRAO + 1];
    int max_ocorrencias = 0;
    size_t historico_len = strlen(historico);

    for (size_t i = 0; i <= historico_len - tamanho_padrao; i++) {
        strncpy(padrao_atual, &historico[i], tamanho_padrao);
        padrao_atual[tamanho_padrao] = '\0';

        int ocorrencias = contar_ocorrencias_consecutivas(historico, padrao_atual, tamanho_padrao);

        if (ocorrencias > max_ocorrencias) {
            max_ocorrencias = ocorrencias;
            strncpy(padrao_mais_frequente, padrao_atual, tamanho_padrao);
            padrao_mais_frequente[tamanho_padrao] = '\0';
        }
    }

    return max_ocorrencias;
}


void limparHistoricoPassos() {
    memset(historico, 0, sizeof(historico));
}

void resetarJogo() {
    player_x = MAPA_LARGURA / 2;
    player_y = MAPA_ALTURA / 2;
    limparHistoricoPassos();
    inicializarEspeciaria();
    playerWater = 100.0;

    isMonsterActive = false;
    nextMonsterCheckTime = 0.0;
    monsterStartTime = 0.0;
    lastEPressTime = 0.0;
    ePressCount = 0;
    spriteFrameIndex = 0;
    spriteAnimationTimer = 0.0f;
    lastMap = -1;
}

void zerarMonetaria() {
    itemsCollected = 0;
    playerMoney = 0;
}

bool isPlayerNearPortal() {
    int portalMinX = PORTAL_RETORNO_X;
    int portalMaxX = PORTAL_RETORNO_X + PORTAL_RETORNO_LARGURA - 1;
    int portalMinY = PORTAL_RETORNO_Y;
    int portalMaxY = PORTAL_RETORNO_Y + PORTAL_RETORNO_ALTURA - 1;

    if ((abs(player_x - portalMinX) <= 1 && player_y >= portalMinY - 1 && player_y <= portalMaxY + 1) ||
        (abs(player_x - portalMaxX) <= 1 && player_y >= portalMinY - 1 && player_y <= portalMaxY + 1) ||
        (abs(player_y - portalMinY) <= 1 && player_x >= portalMinX - 1 && player_x <= portalMaxX + 1) ||
        (abs(player_y - portalMaxY) <= 1 && player_x >= portalMinX - 1 && player_x <= portalMaxX + 1)) {
        return true;
    }
    return false;
}

void playGame(GameScreen *currentScreen) {
    mapaEntradaTime = GetTime();
    ClearBackground(BLACK);
    BeginDrawing();
    EndDrawing();

    PlaySound(spellCastSound);
    sleep(2);

    if (mapaAtual == 0) {
        PlaySound(musicaMapa0);
        showLoadingScreen(loadingImagesMap0);
    } else if (mapaAtual == 1) {
        PlaySound(musicaMapa1);
        showLoadingScreen(loadingImagesMap1);
    } else if (mapaAtual == 2) {
        PlaySound(musicaMapa2);
        showLoadingScreen(loadingImagesMap2);
    }

    if (mapaAtual != -1) {
        inicializarEspeciaria();
    }

    player_x = PORTAL_RETORNO_X + (PORTAL_RETORNO_LARGURA / 2);
    player_y = PORTAL_RETORNO_Y + PORTAL_RETORNO_ALTURA + 1;
    memset(historico, 0, sizeof(historico));
    bool pertoDoPortal = false;

    while (!WindowShouldClose()) {
        int dx = 0, dy = 0;
        char movimento = '\0';

        updateWaterLevel(currentScreen);

        if (playerWater <= 0.0) {
            StopSound(musicaMapa0);
            StopSound(musicaMapa1);
            StopSound(musicaMapa2);

            Texture2D personagemMorto = LoadTexture("static/image/dead.png");
            ClearBackground(BLACK);
            desenharAnimacaoMorte(personagem, personagemMorto);
            DrawText("GAME OVER - Você ficou sem água!", 10, 40, 20, RED);
            sleep(3);
            atualizarRanking(playerName, playerMoney);
            zerarMonetaria();
            *currentScreen = RANKINGS;
            resetarJogo();
            return;
        }
        if (IsKeyPressed(KEY_W)) { dy = -1; movimento = 'w'; }
        if (IsKeyPressed(KEY_S)) { dy = 1; movimento = 's'; }
        if (IsKeyPressed(KEY_A)) { dx = -1; movimento = 'a'; }
        if (IsKeyPressed(KEY_D)) { dx = 1; movimento = 'd'; }

        if (isPlayerNearPortal()) {
            mensagem = "Você deseja voltar para o lobby? Pressione [P]";
            pertoDoPortal = true;
        } else {
            mensagem = NULL;
            pertoDoPortal = false;
        }

        if (pertoDoPortal && IsKeyPressed(KEY_P)) {
            if (mapaAtual == 0) {
                player_x = PORTAL_LOBBY_MAPA1_X + (PORTAL_HORIZONTAL_LARGURA / 2);
                player_y = PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA + 1;
            } else if (mapaAtual == 1) {
                player_x = PORTAL_LOBBY_MAPA2_X + (PORTAL_VERTICAL_LARGURA / 2);
                player_y = PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA + 1;
            } else if (mapaAtual == 2) {
                player_x = PORTAL_LOBBY_MAPA3_X + (PORTAL_HORIZONTAL_LARGURA / 2);
                player_y = PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA + 1;
            }

            ClearBackground(BLACK);
            StopSound(musicaMapa0);
            StopSound(musicaMapa1);
            StopSound(musicaMapa2);
            PlaySound(spellCastSound);
            sleep(2);

            showLoadingScreen(loadingImagesLobby);
            *currentScreen = LOBBY;
            mapaAtual = -1;
            mensagem = NULL;
            break;
        }

        int tamanho_padrao = 0;
        switch (mapaAtual) {
            case 0: tamanho_padrao = 10; break;
            case 1: tamanho_padrao = 8; break;
            case 2: tamanho_padrao = 6; break;
        }

        if (faltamDoisParaSequencia(historico, tamanho_padrao)) {
            if (!isHeartbeatPlaying) {
                PlaySound(heartbeatSound);
                isHeartbeatPlaying = true;
            }
        } else {
            isHeartbeatPlaying = false;
        }


        if (movimento != '\0') {
            movePlayer(dx, dy);
            checkItemCollection();

            bool emSafeZone = (
                (mapaAtual == 0 && ((player_x >= 20 && player_x < 23 && player_y >= 20 && player_y < 22) || 
                                    (player_x >= 10 && player_x < 12 && player_y >= 15 && player_y < 17) ||
                                    (player_x >= 30 && player_x < 32 && player_y >= 5 && player_y < 7) ||
                                    (player_x >= 25 && player_x < 27 && player_y >= 10 && player_y < 13))) ||
                (mapaAtual == 1 && ((player_x >= 5 && player_x < 8 && player_y >= 10 && player_y < 12) ||
                                    (player_x >= 35 && player_x < 37 && player_y >= 15 && player_y < 18))) ||
                (mapaAtual == 2 && (player_x >= 20 && player_x < 22 && player_y >= 15 && player_y < 17))
            );

            if (!emSafeZone) {
                size_t len = strlen(historico);
                if (len < MAX_HISTORICO - 1) {
                    historico[len] = movimento;
                    historico[len + 1] = '\0';
                }
            } else {
                memset(historico, 0, sizeof(historico));
            }

            char padrao_encontrado[MAX_PADRAO + 1] = "";
            int encontrou_padrao = 0;
            int dificuldade;

            switch (mapaAtual) {
                case 0: dificuldade = 5; break;
                case 1: dificuldade = 4; break;
                case 2: dificuldade = 3; break;
            }

            if ((dificuldade == 5 && strlen(historico) >= 5 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 5, padrao_encontrado))) ||
                (dificuldade == 4 && strlen(historico) >= 4 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 4, padrao_encontrado))) ||
                (dificuldade == 3 && strlen(historico) >= 3 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 3, padrao_encontrado)))) 
            {

                StopSound(musicaMapa0);
                StopSound(musicaMapa1);
                StopSound(musicaMapa2);
                
                sleep(1);
                PlaySound(gameOverSound);
                sleep(1);

                Texture2D personagemMorto = LoadTexture("static/image/dead.png");
                ClearBackground(BLACK);
                desenharAnimacaoMorte(personagem, personagemMorto);
                deathEmotivaTocando = 1;
                PlaySound(deathEmotiva);
                sleep(3);

                Vector2 spritePos = {GetScreenWidth() / 2, GetScreenHeight() / 2};
                float scale = 10.0f;
                Rectangle frames[3] = {
                    {128, 192, 64, 64},
                    {64, 192, 64, 64},
                    {0, 192, 64, 64}
                };
                int frameIndex = 0;
                float frameTime = 1.0f;
                float moveSpeed = 20.0f;
                float startTime = GetTime();
                float elapsedTime = 0.0f;
                float maxScreenTime = 5.0f;

                PlaySound(barulhoMonstro);
                Vector2 monstersPos = {850, spritePos.y};

                while ((GetTime() - startTime < maxScreenTime) && !WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(BLACK);

                    elapsedTime = GetTime() - startTime;

                    Rectangle destMonsters = {monstersPos.x, monstersPos.y, 128 * scale, 128 * scale};
                    Rectangle srcMonsters;
                    Vector2 originMonsters = {64 * scale, 64 * scale};

                    if (frameIndex == 0) {
                        srcMonsters = (Rectangle){352, 320, 128, 128};
                    } else if (frameIndex == 1) {
                        srcMonsters = (Rectangle){240, 320, 128, 128};
                    } else if (frameIndex == 2) {
                        srcMonsters = (Rectangle){128, 320, 128, 128};
                    }

                    DrawTexturePro(monsters, srcMonsters, destMonsters, originMonsters, 0.0f, WHITE);

                    Rectangle dest = {
                        spritePos.x, spritePos.y,
                        frames[frameIndex].width * scale,
                        frames[frameIndex].height * scale
                    };
                    Vector2 origin = {frames[frameIndex].width / 2, frames[frameIndex].height / 2};

                    DrawTexturePro(personagemMorto, frames[frameIndex], dest, origin, 0.0f, WHITE);

                    spritePos.x -= moveSpeed * GetFrameTime();
                    monstersPos.x -= moveSpeed * GetFrameTime();

                    if (elapsedTime >= frameTime * (frameIndex + 1) && frameIndex < 2) {
                        frameIndex++;
                    }

                    DrawText(TextFormat("GAME OVER - Padrão repetido: \"%s\" encontrado", padrao_encontrado), 10, 40, 20, RED);

                    EndDrawing();

                    sleep(0.5);
                }
                ClearBackground(BLACK);

                sleep(2);

                const char *euFalhei = "Eu... Eu falhei minha missão...";
                int caractereExibido = 0;
                float tempoPorCaractere = 0.2f;
                float timer = 0;

                while (caractereExibido < strlen(euFalhei) && !WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    timer += GetFrameTime();

                    if (timer >= tempoPorCaractere) {
                        caractereExibido++;
                        timer = 0;
                    }

                    DrawText(TextSubtext(euFalhei, 0, caractereExibido), 
                             GetScreenWidth() / 2 - MeasureText(euFalhei, 20) / 2, 
                             GetScreenHeight() / 2, 20, RAYWHITE);
                    EndDrawing();
                }

                sleep(2);

                atualizarRanking(playerName, playerMoney);
                zerarMonetaria();
                resetarJogo();
                *currentScreen = RANKINGS;
                return;
            }
        }

        if (mapaAtual != lastMap) {
            lastMap = mapaAtual;
            nextMonsterCheckTime = GetTime() + GetRandomValue(6, 10);
            isMonsterActive = false;
        }

        if (!isMonsterActive && GetTime() >= nextMonsterCheckTime) {
            if (GetRandomValue(1, 100) <= 33) {
                PlaySound(monsterGrowl2);
                SetSoundVolume(monsterGrowl2, 1.0f);
                isMonsterActive = true;
                monsterStartTime = GetTime();
                lastEPressTime = 0.0;
                ePressCount = 0;
                spriteFrameIndex = 0;
                spriteAnimationTimer = 0.0f;
            }

            nextMonsterCheckTime = GetTime() + 15.0;
        }

        if (isMonsterActive) {
            double elapsedTime = GetTime() - monsterStartTime;

            if (elapsedTime >= 0.5) {
                BeginDrawing();

                spriteAnimationTimer += GetFrameTime();
                if (spriteAnimationTimer >= 0.2f) {
                    spriteFrameIndex = (spriteFrameIndex + 1) % 2;
                    spriteAnimationTimer = 0.0f;
                }

                Vector2 spritePosition = {player_x * TILE_SIZE + TILE_SIZE, player_y * TILE_SIZE};
                Rectangle sourceRec = (spriteFrameIndex == 0)
                                        ? (Rectangle){160, 192, 704, 656}
                                        : (Rectangle){160, 2336, 704, 560};
                Rectangle destRec = {spritePosition.x, spritePosition.y, 48, 48};
                Vector2 origin = {0, 0};

                DrawTexturePro(EpressSprite, sourceRec, destRec, origin, 0.0f, WHITE);

                if (IsKeyPressed(KEY_E)) {
                    double currentTime = GetTime();

                    if (lastEPressTime == 0.0 || currentTime - lastEPressTime <= 0.5) {
                        ePressCount++;
                        lastEPressTime = currentTime;
                    } else {
                        ePressCount = 1;
                        lastEPressTime = currentTime;
                    }

                    if (ePressCount >= 3) {
                        isMonsterActive = false;
                    }
                }

                EndDrawing();
            }

            if (!IsSoundPlaying(monsterGrowl2)) {
                isMonsterActive = false;
            }

            if (GetTime() - monsterStartTime > 4.5f && ePressCount < 3) {
                StopSound(monsterGrowl2);

                StopSound(musicaMapa0);
                StopSound(musicaMapa1);
                StopSound(musicaMapa2);
                
                sleep(1);
                PlaySound(gameOverSound);
                sleep(1);

                Texture2D personagemMorto = LoadTexture("static/image/dead.png");
                ClearBackground(BLACK);
                desenharAnimacaoMorte(personagem, personagemMorto);
                deathEmotivaTocando = 1;
                PlaySound(deathEmotiva);
                sleep(3);

                Vector2 spritePos = {GetScreenWidth() / 2, GetScreenHeight() / 2};
                float scale = 10.0f;
                Rectangle frames[3] = {
                    {128, 192, 64, 64},
                    {64, 192, 64, 64},
                    {0, 192, 64, 64}
                };
                int frameIndex = 0;
                float frameTime = 1.0f;
                float moveSpeed = 20.0f;
                float startTime = GetTime();
                float elapsedTime = 0.0f;
                float maxScreenTime = 5.0f;

                PlaySound(barulhoMonstro);
                Vector2 monstersPos = {850, spritePos.y};

                while ((GetTime() - startTime < maxScreenTime) && !WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(BLACK);

                    elapsedTime = GetTime() - startTime;

                    Rectangle destMonsters = {monstersPos.x, monstersPos.y, 128 * scale, 128 * scale};
                    Rectangle srcMonsters;
                    Vector2 originMonsters = {64 * scale, 64 * scale};

                    if (frameIndex == 0) {
                        srcMonsters = (Rectangle){352, 320, 128, 128};
                    } else if (frameIndex == 1) {
                        srcMonsters = (Rectangle){240, 320, 128, 128};
                    } else if (frameIndex == 2) {
                        srcMonsters = (Rectangle){128, 320, 128, 128};
                    }

                    DrawTexturePro(monsters, srcMonsters, destMonsters, originMonsters, 0.0f, WHITE);

                    Rectangle dest = {
                        spritePos.x, spritePos.y,
                        frames[frameIndex].width * scale,
                        frames[frameIndex].height * scale
                    };
                    Vector2 origin = {frames[frameIndex].width / 2, frames[frameIndex].height / 2};

                    DrawTexturePro(personagemMorto, frames[frameIndex], dest, origin, 0.0f, WHITE);

                    spritePos.x -= moveSpeed * GetFrameTime();
                    monstersPos.x -= moveSpeed * GetFrameTime();

                    if (elapsedTime >= frameTime * (frameIndex + 1) && frameIndex < 2) {
                        frameIndex++;
                    }

                    DrawText(TextFormat("GAME OVER - O verme de areia lhe identificou"), 10, 40, 20, RED);

                    EndDrawing();

                    sleep(0.5);
                }
                ClearBackground(BLACK);

                sleep(2);

                const char *euFalhei = "Eu... Eu falhei minha missão...";
                int caractereExibido = 0;
                float tempoPorCaractere = 0.2f;
                float timer = 0;

                while (caractereExibido < strlen(euFalhei) && !WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    timer += GetFrameTime();

                    if (timer >= tempoPorCaractere) {
                        caractereExibido++;
                        timer = 0;
                    }

                    DrawText(TextSubtext(euFalhei, 0, caractereExibido), 
                             GetScreenWidth() / 2 - MeasureText(euFalhei, 20) / 2, 
                             GetScreenHeight() / 2, 20, RAYWHITE);
                    EndDrawing();
                }

                sleep(2);

                

                atualizarRanking(playerName, playerMoney);
                zerarMonetaria();
                resetarJogo();
                *currentScreen = RANKINGS;
                return;
            }
        }

        BeginDrawing();
        drawGame();
        EndDrawing();
    }
}

