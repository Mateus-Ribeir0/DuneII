#include "game.h"
#include "lobby.h"

static Texture2D personagem;
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

#define NUM_ITEMS 5
#define MAX_HISTORICO 1000
#define MAX_PADRAO 9
#define TOTAL_ESPECIARIAS 15

int playerMoney = 0;  // Definição
int deathEmotivaTocando;

Rectangle cerealsSourceRec = { 64, 64, 32, 32 };

// Arrays para armazenar as imagens de carregamento
Texture2D loadingImagesMap0[4];
Texture2D loadingImagesMap1[4];
Texture2D loadingImagesMap2[4];
Texture2D loadingImagesLobby[4];

// Tempos de exibição de cada imagem (aumentado em 2 segundos)
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

// Variáveis de animação do personagem
int frameAtual = 0;
float tempoAnimacao = 0;
const float duracaoFrame = 0.2f;  // Duração de cada quadro em segundos

// Coordenadas predefinidas para especiarias, garantindo distância entre os pontos
// Coordenadas predefinidas para especiarias, revisadas para evitar colisões e garantir acessibilidade no mapa 2
Point posicoesEspeciarias[TOTAL_ESPECIARIAS] = {
    {20, 4}, {8, 6}, {12, 11}, {17, 8}, {23, 12}, {28, 5}, {32, 14}, 
    {36, 17}, {6, 15}, {15, 16}, {20, 7}, {26, 18}, {30, 10}, {33, 6}, {37, 13}
};


// Índice para rastrear a próxima especiaria a aparecer
int indiceEspeciariaAtual = 0;

// Funções para inicializar as imagens de carregamento para cada mapa
void initializeLoadingScreen() {
    // Carregamento para mapa 0
    loadingImagesMap0[0] = LoadTexture("static/image/mapa0.1.png");
    loadingImagesMap0[1] = LoadTexture("static/image/mapa0.2.png");
    loadingImagesMap0[2] = LoadTexture("static/image/mapa0.3.png");
    loadingImagesMap0[3] = LoadTexture("static/image/mapa0.4.png");

    // Carregamento para mapa 1
    loadingImagesMap1[0] = LoadTexture("static/image/mapa1.1.png");
    loadingImagesMap1[1] = LoadTexture("static/image/mapa1.2.png");
    loadingImagesMap1[2] = LoadTexture("static/image/mapa1.3.png");
    loadingImagesMap1[3] = LoadTexture("static/image/mapa1.4.png");

    // Carregamento para mapa 2
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
    // Carrega texturas
    personagem = LoadTexture("static/image/newstoppedsprites.png");
    personagemAndando = LoadTexture("static/image/newwalkingsprites.png");
    cerealsTexture = LoadTexture("static/image/Cereals.png");
    ruinasDeAreiaGrandes = LoadTexture("static/image/Sand_ruins1.png");
    environment1_1 = LoadTexture("static/image/Rock6_1.png");
    environment1_2 = LoadTexture("static/image/Rock6_3.png");
    environment2_1 = LoadTexture("static/image/Rock2_1.png");
    environment2_2 = LoadTexture("static/image/Rock2_3.png");
    environment3_1 = LoadTexture("static/image/Rock8_1.png");
    environment3_2 = LoadTexture("static/image/Rock8_3.png");
    safezone = LoadTexture ("static/image/desert_tileset2.png");
    ruinasDeAreiaPequenas = LoadTexture("static/image/Sand_ruins5.png");
    goldTexture = LoadTexture("static/image/gold.png");
    aguaTexture = LoadTexture("static/image/agua.png");
    characterBack = LoadTexture("static/image/characterback.png");
    sandworm = LoadTexture("static/image/sandworm.png");
    portal = LoadTexture("static/image/portal.png");

    // Carrega sons
    musicaMapa0 = LoadSound("static/music/mapa0musica.wav");
    musicaMapa1 = LoadSound("static/music/mapa1musica.wav");
    musicaMapa2 = LoadSound("static/music/mapa2musica.wav");
    spellCastSound = LoadSound("static/music/spellcast.mp3");
    gameOverSound = LoadSound("static/music/deathsfx2.wav");
    barulhoMonstro = LoadSound("static/music/monster.mp3");
    deathEmotiva = LoadSound("static/music/deathemotiva.wav");

    // Inicializa as imagens de carregamento
    initializeLoadingScreen();
}

void finalizarGame() {
    // Descarrega texturas
    UnloadTexture(personagem);
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

    // Descarrega sons
    UnloadSound(musicaMapa0);
    UnloadSound(musicaMapa1);
    UnloadSound(musicaMapa2);
    UnloadSound(spellCastSound);
    UnloadSound(gameOverSound);
    UnloadSound(barulhoMonstro);
    UnloadSound(deathEmotiva);

    // Descarrega as imagens de carregamento
    for (int i = 0; i < 4; i++) {
        UnloadTexture(loadingImagesMap0[i]);
        UnloadTexture(loadingImagesMap1[i]);
        UnloadTexture(loadingImagesMap2[i]);
        UnloadTexture(loadingImagesLobby[i]);
    }
}

// Função para exibir a sequência de telas de carregamento
void showLoadingScreen(Texture2D* loadingImages) {
    for (int i = 0; i < 4; i++) {
        ClearBackground(BLACK);
        DrawTexture(loadingImages[i], 0, 0, WHITE);  // Desenha a imagem de carregamento
        EndDrawing();

        // Espera o tempo especificado antes de passar para a próxima imagem
        float waitTime = loadingImageDisplayTimes[i];
        float startTime = GetTime();

        while (GetTime() - startTime < waitTime) {
            // Mantém a tela visível pelo tempo especificado
            BeginDrawing();
            DrawTexture(loadingImages[i], 0, 0, WHITE);
            EndDrawing();
        }
    }
}

// Inicializa a primeira especiaria a ser coletada
void inicializarEspeciaria() {
    if (indiceEspeciariaAtual < TOTAL_ESPECIARIAS) {
        items[0].position = posicoesEspeciarias[indiceEspeciariaAtual];
        items[0].collected = false;
    }
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

            // Define a pontuação das especiarias com base no mapa atual
            switch (mapaAtual) {
                case 0: especiariasGanhas = 1; break;
                case 1: especiariasGanhas = 2; break;
                case 2: especiariasGanhas = 4; break;
            }

            // Calcula a nova quantidade de especiarias sem ultrapassar o limite da bolsa
            itemsCollected += especiariasGanhas;
            if (itemsCollected > MAX_ESPECIARIAS) {
                itemsCollected = MAX_ESPECIARIAS;
            }

            // Atualiza para a próxima especiaria
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

// Array para posições das dunas em cada mapa
Point posicoesDunasMapa1[DUNAS_MAPA1] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5} };
Point posicoesDunasMapa2[DUNAS_MAPA2] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5}, {12, 14} };
Point posicoesDunasMapa3[DUNAS_MAPA3] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5}, {20, 10}, {12, 14}, {28, 6}, {7, 19}, {17, 3} };


void movePlayer(int dx, int dy) {
    int new_x = player_x + dx;
    int new_y = player_y + dy;

    // Define o retângulo do jogador
    Rectangle playerRect = { new_x * TILE_SIZE, new_y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

    // Define o retângulo de colisão das ruínas, removendo 20 pixels da parte superior
    Rectangle ruinasCollisionBox = { 30, 40, 190, 180 };  // Y deslocado para 40, altura reduzida para 180

    // Verifica se a nova posição está dentro dos limites do mapa
    if (new_x >= 0 && new_x < MAPA_LARGURA && new_y >= 0 && new_y < MAPA_ALTURA) {
        bool colidiuComDuna = false;
        bool colidiuComPedra = false;

        // Verifica colisão com o retângulo das ruínas apenas no mapa 2
        if (mapaAtual == 1 && CheckCollisionRecs(playerRect, ruinasCollisionBox)) {
            return; // Bloqueia a movimentação se houver colisão com as ruínas
        }

        // Verifica colisão com os portais no lobby e nos mapas
        if (isPlayerOnPortal(new_x, new_y, mapaAtual)) {
            return; // Bloqueia a movimentação se houver colisão com um portal
        }

        // Verifica colisão com as pedras no mapa 0
        if (mapaAtual == 0) {
            for (int i = 0; i < NUM_PEDRAS; i++) {
                if (new_x == (int)posicoesPedras[i].x && new_y == (int)posicoesPedras[i].y) {
                    colidiuComPedra = true;
                    break;
                }
            }

            // Verifica colisão com dunas no mapa 0
            for (int i = 0; i < DUNAS_MAPA1; i++) {
                if ((new_x >= posicoesDunasMapa1[i].x && new_x < posicoesDunasMapa1[i].x + 3) &&
                    (new_y >= posicoesDunasMapa1[i].y && new_y < posicoesDunasMapa1[i].y + 2)) {
                    colidiuComDuna = true;
                    break;
                }
            }
        } else if (mapaAtual == 1) {
            // Verifica colisão com dunas no mapa 1
            for (int i = 0; i < DUNAS_MAPA2; i++) {
                if ((new_x >= posicoesDunasMapa2[i].x && new_x < posicoesDunasMapa2[i].x + 3) &&
                    (new_y >= posicoesDunasMapa2[i].y && new_y < posicoesDunasMapa2[i].y + 2)) {
                    colidiuComDuna = true;
                    break;
                }
            }
        } else if (mapaAtual == 2) {
            // Verifica colisão com dunas no mapa 2
            for (int i = 0; i < DUNAS_MAPA3; i++) {
                if ((new_x >= posicoesDunasMapa3[i].x && new_x < posicoesDunasMapa3[i].x + 3) &&
                    (new_y >= posicoesDunasMapa3[i].y && new_y < posicoesDunasMapa3[i].y + 2)) {
                    colidiuComDuna = true;
                    break;
                }
            }
        }

        // Se não houver colisão com uma duna, ruínas, pedra ou portal, movimenta o jogador
        if (!colidiuComDuna && !colidiuComPedra) {
            player_x = new_x;
            player_y = new_y;
        }
    }
}

void desenharAnimacaoMorte(Texture2D personagem, Texture2D personagemMorto) {
    // Coordenadas dos frames para a animação de morte
    Rectangle framesMorte[] = {
        {128, 0, 64, 64}, {192, 0, 64, 64},
        {256, 0, 64, 64}, {320, 0, 64, 64}, {384, 0, 64, 64}
    };
    int numFramesMorte = sizeof(framesMorte) / sizeof(framesMorte[0]);

    // Posição exata do jogador, ajustando para centralizar o sprite de 96x96 pixels
    Vector2 posicao = {player_x * TILE_SIZE, player_y * TILE_SIZE};
    Rectangle destRec = {posicao.x - 32, posicao.y, 96, 96};  // 96x96 para centralizar como em drawGame

    // Duração de cada frame em segundos
    const float duracaoFramePrimeiro = 0.5f; // Duração estendida para o primeiro quadro
    const float duracaoFrame = 0.1f;         // Duração para os demais quadros
    float tempoInicial = GetTime();

    // Exibe cada frame da animação de morte
    for (int i = 0; i < numFramesMorte; i++) {
        // Define a duração específica para o primeiro frame
        float tempoFrameAtual = (i == 0) ? duracaoFramePrimeiro : duracaoFrame;

        while (GetTime() - tempoInicial < tempoFrameAtual) {
            // Desenha o frame atual até que o tempo de exibição passe do tempo do quadro atual
            ClearBackground(BLACK);
            BeginDrawing();
            DrawTexturePro(personagemMorto, framesMorte[i], destRec, (Vector2){0, 0}, 0.0f, WHITE);
            EndDrawing();
        }

        // Atualiza o tempo inicial para o próximo frame
        tempoInicial = GetTime();
    }

    // Desenha o último frame permanentemente
    DrawTexturePro(personagemMorto, framesMorte[numFramesMorte - 1], destRec, (Vector2){0, 0}, 0.0f, WHITE);
}

// Variável para armazenar a direção atual do personagem
int direcao = 0;  // 0 = parado, 1 = esquerda, 2 = direita, 3 = cima, 4 = baixo

void drawGame() {
    ClearBackground(RAYWHITE);

    // Itera sobre a área do mapa e desenha o tile em cada posição
    Color map0Color = (Color){210, 178, 104, 255};  // Define a cor desejada
    Color map1Color = (Color){210, 178, 104, 255};
    Color map2Color = (Color){228, 162, 68, 255};

    Rectangle sourceRect = {64, 64, 64, 64}; // Área da textura original
    Vector2 origin = {0, 0}; // Origem de rotação (mantida em (0, 0))

    Rectangle ruinasSourceRec = {0, 0, ruinasDeAreiaGrandes.width, ruinasDeAreiaGrandes.height}; // Recorte original da textura
    Rectangle ruinasDestRec = {20, 20, 256, 256}; // Destino com escala maior (256x256)

    // Definindo a caixa de colisão de ruinasDeAreia em 256x256

    if (mapaAtual == 0) {
    // Desenha as pedras no mapa environment2_2
        for (int y = 0; y < MAPA_ALTURA; y++) {
            for (int x = 0; x < MAPA_LARGURA; x++) {
                Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
                DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, map0Color);
            }
        }

        for (int i = 0; i < NUM_PEDRAS; i++) {
            Vector2 posicaoPedra = { posicoesPedras[i].x * TILE_SIZE, posicoesPedras[i].y * TILE_SIZE };
            DrawTexture(environment2_2, posicaoPedra.x, posicaoPedra.y, RAYWHITE);
        }

        // Desenha as dunas (sua lógica original)
        for (int i = 0; i < DUNAS_MAPA1; i++) {
            Vector2 posicaoDuna = { posicoesDunasMapa2[i].x * TILE_SIZE, posicoesDunasMapa2[i].y * TILE_SIZE };
            Rectangle destRect = { posicaoDuna.x, posicaoDuna.y, 96, 96 };
            DrawTexturePro(environment2_1, sourceRect, destRect, origin, 0.0f, WHITE);
        }

        Vector2 safezonePosition = {10 * TILE_SIZE, 10 * TILE_SIZE}; // Ajuste conforme necessário
        Rectangle safezoneRec = {376, 136, 32, 32}; // Recorte da `safezone`
        DrawTextureRec(safezone, safezoneRec, safezonePosition, RAYWHITE);

    } else if (mapaAtual == 1) {
        for (int y = 0; y < MAPA_ALTURA; y++) {
            for (int x = 0; x < MAPA_LARGURA; x++) {
                Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
                DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, map1Color);
            }
        }
        // Desenha ruinasDeAreia em tamanho maior
        DrawTexture(environment1_2, 20, 20, RAYWHITE);
        DrawTexturePro(ruinasDeAreiaGrandes, ruinasSourceRec, ruinasDestRec, origin, 0.0f, WHITE);
        DrawTexture(ruinasDeAreiaPequenas, 20, 20, RAYWHITE);
        for (int i = 0; i < DUNAS_MAPA2; i++) {
            Vector2 posicaoDuna = { posicoesDunasMapa1[i].x * TILE_SIZE, posicoesDunasMapa1[i].y * TILE_SIZE };
            Rectangle destRect = { posicaoDuna.x, posicaoDuna.y, 96, 96 }; // Tamanho maior: 96x96
            DrawTexturePro(environment1_1, sourceRect, destRect, origin, 0.0f, WHITE); 
        }

        Vector2 safezonePosition = {15 * TILE_SIZE, 12 * TILE_SIZE}; // Posição ajustável
        Rectangle safezoneRec = {376, 136, 32, 32}; 
        DrawTextureRec(safezone, safezoneRec, safezonePosition, RAYWHITE);

    } else if (mapaAtual == 2) {
        for (int y = 0; y < MAPA_ALTURA; y++) {
            for (int x = 0; x < MAPA_LARGURA; x++) {
                Vector2 tilePosition = { x * TILE_SIZE, y * TILE_SIZE };
                DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, map2Color);
            }
        }
        DrawTexture(environment3_2, 20, 20, RAYWHITE);
        for (int i = 0; i < DUNAS_MAPA3; i++) {
            Vector2 posicaoDuna = { posicoesDunasMapa3[i].x * TILE_SIZE, posicoesDunasMapa3[i].y * TILE_SIZE };
            Rectangle destRect = { posicaoDuna.x, posicaoDuna.y, 96, 96 }; // Tamanho maior: 96x96
            DrawTexturePro(environment3_1, sourceRect, destRect, origin, 0.0f, WHITE);
        }

        Vector2 safezonePosition = {20 * TILE_SIZE, 15 * TILE_SIZE}; // Posição ajustável
        Rectangle safezoneRec = {376, 136, 32, 32}; 
        DrawTextureRec(safezone, safezoneRec, safezonePosition, RAYWHITE);

    }

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
    Rectangle sourceRec;
    switch (lastDirection) {
        case 1: // Cima
            sourceRec = (Rectangle){0, 192, 64, 64};
            break;
        case 2: // Esquerda
            sourceRec = (Rectangle){0, 64, 64, 64};
            break;
        case 3: // Baixo
            sourceRec = (Rectangle){0, 0, 64, 64};
            break;
        case 4: // Direita
            sourceRec = (Rectangle){0, 128, 64, 64};
            break;
        default:
            sourceRec = (Rectangle){0, 0, 64, 64}; // Posição padrão (parado)
            break;
    }

    // Define a posição do personagem na tela
    Vector2 position = { player_x * TILE_SIZE, player_y * TILE_SIZE };

    // Desenha o sprite redimensionado para 96x96, centralizando sobre a posição de 32x32
    Rectangle destRec = { position.x - 32, position.y - 32, 96, 96 };  // Ajuste para centralizar o sprite maior

    // Se estiver "andando", desenha o sprite de "andando"; caso contrário, desenha o sprite de direção final
    if (isWalking) {
        DrawTexturePro(personagemAndando, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawTexturePro(personagem, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
    }

    // Desenha o item usando o novo sprite recortado se ele não estiver coletado
    if (!items[0].collected) {
        Vector2 itemPosition = { items[0].position.x * TILE_SIZE, items[0].position.y * TILE_SIZE };
        DrawTextureRec(cerealsTexture, cerealsSourceRec, itemPosition, WHITE);
    }

    // Desenha o HUD do jogo no canto direito da tela
    int infoBoxX = SCREEN_WIDTH - 230;
    int infoBoxY = 10;
    int infoBoxWidth = 220;
    int infoBoxHeight = 100;

    // Desenhar o preenchimento da caixa com um marrom mais suave
    DrawRectangleRounded((Rectangle){infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight}, 0.1f, 16, (Color){205, 133, 63, 255});

    // Desenhar a borda simulando uma espessura extra
    DrawRectangleRoundedLines((Rectangle){infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight}, 0.1f, 16, (Color){101, 67, 33, 255});
    DrawRectangleRoundedLines((Rectangle){infoBoxX + 1, infoBoxY + 1, infoBoxWidth - 2, infoBoxHeight - 2}, 0.1f, 16, (Color){101, 67, 33, 255});
    DrawRectangleRoundedLines((Rectangle){infoBoxX + 2, infoBoxY + 2, infoBoxWidth - 4, infoBoxHeight - 4}, 0.1f, 16, (Color){101, 67, 33, 255});


    // Posiciona a imagem da especiaria (cerealsTexture) e o texto "Especiarias"
    int especiariaIconY = infoBoxY + 1;
    Vector2 especiariaIconPos = { infoBoxX + 10, especiariaIconY };
    DrawTextureRec(cerealsTexture, (Rectangle){64, 64, 32, 32}, especiariaIconPos, WHITE);

    // Desenha o texto "Especiarias:" ao lado da imagem e a quantidade coletada
    DrawText("Especiarias:", infoBoxX + 50, infoBoxY + 10, 18, WHITE);
    DrawText(TextFormat("%d/%d", itemsCollected, MAX_ESPECIARIAS), infoBoxX + 160, infoBoxY + 10, 18, WHITE);

    // Exibe o ícone de ouro e o texto do dinheiro logo abaixo
    Vector2 goldIconPos = { infoBoxX + 15, infoBoxY + 35 };
    Rectangle goldSourceRec = { 0, 0, 16, 16 };
    Rectangle goldDestRec = { goldIconPos.x, goldIconPos.y, 24, 24 };

    DrawTexturePro(goldTexture, goldSourceRec, goldDestRec, (Vector2){0, 0}, 0.0f, WHITE);
    DrawText(TextFormat("Dinheiro: %d", playerMoney), infoBoxX + 50, infoBoxY + 40, 18, WHITE);

    // Exibe o ícone de água e o texto de água logo abaixo de goldTexture
    Vector2 aguaIconPos = { infoBoxX + 18, infoBoxY + 65 };
    Rectangle aguaSourceRec = { 0, 0, aguaTexture.width, aguaTexture.height };
    Rectangle aguaDestRec = { aguaIconPos.x, aguaIconPos.y, 24, 24 };

    DrawTexturePro(aguaTexture, aguaSourceRec, aguaDestRec, (Vector2){0, 0}, 0.0f, WHITE);
    DrawText(TextFormat("Água: %.0f%%", playerWater), infoBoxX + 55, infoBoxY + 70, 18, WHITE);

    // Desenha o portal de retorno ao lobby
    Rectangle portalSourceRec = { 0, 0, 32, 32 };
    Rectangle portalDestRec = {
        PORTAL_RETORNO_X * TILE_SIZE,
        PORTAL_RETORNO_Y * TILE_SIZE,
        TILE_SIZE * PORTAL_RETORNO_LARGURA,
        TILE_SIZE * PORTAL_RETORNO_ALTURA
    };
    
    // Desenha o portal de retorno ao lobby usando a mesma textura
    DrawTexturePro(portal, portalSourceRec, portalDestRec, origin, 0.0f, WHITE);

    // Desenha a mensagem centralizada, se existir
    if (mensagem != NULL) {
        int screenWidth = GetScreenWidth();
        int textWidth = MeasureText(mensagem, 20);
        int xPosition = (screenWidth - textWidth) / 2;
        DrawText(mensagem, xPosition, GetScreenHeight() / 2, 20, BLACK);
    }
}

// Contador de ocorrências consecutivas
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

// Função para identificar o padrão mais frequente
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
    playerWater = 100.0;  // Restaura o nível de água para 100%
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
    player_y = PORTAL_RETORNO_Y + PORTAL_RETORNO_ALTURA;
    memset(historico, 0, sizeof(historico));
    bool pertoDoPortal = false;

    while (!WindowShouldClose()) {
        int dx = 0, dy = 0;
        char movimento = '\0';

        // Atualiza o nível de água de acordo com o mapa atual
        updateWaterLevel(currentScreen);

        if (playerWater <= 0.0) {
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
                // Retornando do mapa 0 (Zamirat)
                player_x = PORTAL_LOBBY_MAPA1_X + (PORTAL_HORIZONTAL_LARGURA / 2);
                player_y = PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA;
            } else if (mapaAtual == 1) {
                // Retornando do mapa 1 (Bashir'har)
                player_x = PORTAL_LOBBY_MAPA2_X + (PORTAL_VERTICAL_LARGURA / 2);
                player_y = PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA;
            } else if (mapaAtual == 2) {
                // Retornando do mapa 2 (Qasr'Rahim)
                player_x = PORTAL_LOBBY_MAPA3_X + (PORTAL_HORIZONTAL_LARGURA / 2);
                player_y = PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA;
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

        if (movimento != '\0') {
            movePlayer(dx, dy);
            checkItemCollection();

            size_t len = strlen(historico);
            if (len < MAX_HISTORICO - 1) {
                historico[len] = movimento;
                historico[len + 1] = '\0';
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
                DrawText(TextFormat("GAME OVER - Padrão repetido: \"%s\" encontrado", padrao_encontrado), 10, 40, 20, RED);
                sleep(3);
                PlaySound(barulhoMonstro);
                sleep(1);

                deathEmotivaTocando = 1;
                PlaySound(deathEmotiva);

                int sandwormPosY = GetScreenHeight() / 2 - sandworm.height / 2;
                int startTime = GetTime();

                while ((GetTime() - startTime < 5) && !WindowShouldClose()) {
                    sandwormPosY -= 1;

                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawTexture(sandworm, GetScreenWidth() / 2 - sandworm.width / 2, sandwormPosY + 40, WHITE);
                    DrawTexture(characterBack, 0, GetScreenHeight() - characterBack.height, WHITE);
                    EndDrawing();

                    sleep(1);
                }

                //UnloadSound(gameOverSound);
                //UnloadSound(barulhoMonstro);
                //UnloadTexture(characterBack);
                //UnloadTexture(sandworm);

                const char *euFalhei = "Eu... Eu falhei minha missão...";
                int caractereExibido = 0;
                float tempoPorCaractere = 0.3f;
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
                ClearBackground(RAYWHITE);
                *currentScreen = RANKINGS;
                return;
            }
        }

        BeginDrawing();
        drawGame();
        EndDrawing();
    }

}
