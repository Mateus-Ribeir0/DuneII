#include "game.h"

#define NUM_ITEMS 5
#define MAX_HISTORICO 1000
#define MAX_PADRAO 9
#define TOTAL_ESPECIARIAS 15

int playerMoney = 0;  // Definição
Texture2D personagem;
Texture2D environment;
int teveUnload = 0;
int deathEmotivaTocando;

// Arrays para armazenar as imagens de carregamento
Texture2D loadingImagesMap0[4];
Texture2D loadingImagesMap1[4];
Texture2D loadingImagesMap2[4];
Texture2D loadingImagesLobby[4];

// Tempos de exibição de cada imagem (aumentado em 2 segundos)
const float loadingImageDisplayTimes[4] = {2.5f, 3.0f, 3.5f, 4.0f};  
Sound spellCastSound;  // Som para a tela de carregamento

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
static unsigned long seed = 22135234;

// Variáveis de animação do personagem
int frameAtual = 0;
float tempoAnimacao = 0;
const float duracaoFrame = 0.2f;  // Duração de cada quadro em segundos

// Coordenadas predefinidas para especiarias, garantindo distância entre os pontos
Point posicoesEspeciarias[TOTAL_ESPECIARIAS] = {
    {2, 2}, {6, 8}, {10, 4}, {14, 10}, {18, 16}, {22, 3}, {26, 15}, 
    {30, 7}, {34, 18}, {38, 5}, {5, 14}, {15, 19}, {20, 2}, {28, 9}, {35, 13}
};

// Índice para rastrear a próxima especiaria a aparecer
int indiceEspeciariaAtual = 0;

// Funções de randomização
void custom_srand(unsigned long s) {
    seed = s;
}

int custom_rand() {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return (int)(seed % 32768); 
}

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



// Lógica de coleta de especiarias, com atualização para a próxima posição após a coleta
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

            itemsCollected += especiariasGanhas;

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

#define DUNAS_MAPA1 5
#define DUNAS_MAPA2 7
#define DUNAS_MAPA3 10

// Array para posições das dunas em cada mapa
Point posicoesDunasMapa1[DUNAS_MAPA1] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5} };
Point posicoesDunasMapa2[DUNAS_MAPA2] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5}, {20, 10}, {12, 14} };
Point posicoesDunasMapa3[DUNAS_MAPA3] = { {10, 9}, {15, 12}, {25, 18}, {5, 17}, {35, 5}, {20, 10}, {12, 14}, {28, 6}, {7, 19}, {17, 3} };


void movePlayer(int dx, int dy) {
    int new_x = player_x + dx;
    int new_y = player_y + dy;

    // Verifica se a nova posição está dentro dos limites do mapa
    if (new_x >= 0 && new_x < MAPA_LARGURA && new_y >= 0 && new_y < MAPA_ALTURA) {
        bool colidiuComDuna = false;

        // Verifica colisão com dunas dependendo do mapa atual
        if (mapaAtual == 0) {
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

        // Se não houver colisão com uma duna, movimenta o jogador
        if (!colidiuComDuna) {
            player_x = new_x;
            player_y = new_y;
        }
    }
}





void drawGame() {
    ClearBackground(RAYWHITE);

    // Desenha o mapa de fundo
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){195, 160, 81, 255});
        }
    }

    // Carrega a textura do ambiente
    environment = LoadTexture("static/image/environment.png");
    Rectangle hitboxDuna = {96, 96, 96, 96};

    // Desenha as dunas com base no mapa atual
    if (mapaAtual == 0) {
        for (int i = 0; i < DUNAS_MAPA1; i++) {
            Vector2 posicaoDuna = { posicoesDunasMapa1[i].x * TILE_SIZE, posicoesDunasMapa1[i].y * TILE_SIZE };
            DrawTextureRec(environment, hitboxDuna, posicaoDuna, WHITE);
        }
    } else if (mapaAtual == 1) {
        for (int i = 0; i < DUNAS_MAPA2; i++) {
            Vector2 posicaoDuna = { posicoesDunasMapa2[i].x * TILE_SIZE, posicoesDunasMapa2[i].y * TILE_SIZE };
            DrawTextureRec(environment, hitboxDuna, posicaoDuna, WHITE);
        }
    } else if (mapaAtual == 2) {
        for (int i = 0; i < DUNAS_MAPA3; i++) {
            Vector2 posicaoDuna = { posicoesDunasMapa3[i].x * TILE_SIZE, posicoesDunasMapa3[i].y * TILE_SIZE };
            DrawTextureRec(environment, hitboxDuna, posicaoDuna, WHITE);
        }
    }

    // Desenha o personagem
    personagem = LoadTexture("static/image/spritesheet-character.png");

    // Atualiza o quadro de animação do personagem com base no tempo
    tempoAnimacao += GetFrameTime();
    if (tempoAnimacao >= duracaoFrame) {
        frameAtual = (frameAtual + 1) % 2;
        tempoAnimacao = 0;
    }

    Rectangle sourceRec = { frameAtual * 32, 0, 32, 64 };
    Vector2 position = { player_x * TILE_SIZE, player_y * TILE_SIZE };
    DrawTextureRec(personagem, sourceRec, position, WHITE);

    // Desenha o item se não estiver coletado
    if (!items[0].collected) {
        DrawCircle(items[0].position.x * TILE_SIZE + TILE_SIZE / 2, items[0].position.y * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE / 4, GOLD);
    }

    // Desenha o HUD do jogo
    DrawText(TextFormat("Especiarias na bolsa: %d/%d", itemsCollected, MAX_ESPECIARIAS), 10, 10, 20, BLACK);
    DrawText(TextFormat("Nível de Água: %.0f%%", playerWater), 10, 60, 20, BLUE);

    // Desenha o portal de retorno ao lobby
    DrawRectangle(PORTAL_RETORNO_X * TILE_SIZE, PORTAL_RETORNO_Y * TILE_SIZE,
                  TILE_SIZE * PORTAL_RETORNO_LARGURA, TILE_SIZE * PORTAL_RETORNO_ALTURA, ORANGE);

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
    int ocorrencias;
    size_t historico_len = strlen(historico);

    for (size_t i = 0; i <= historico_len - tamanho_padrao; i++) {
        strncpy(padrao_atual, &historico[i], tamanho_padrao);
        padrao_atual[tamanho_padrao] = '\0';
        
        ocorrencias = contar_ocorrencias_consecutivas(historico, padrao_atual, tamanho_padrao);
        
        if (ocorrencias > max_ocorrencias) {
            max_ocorrencias = ocorrencias;
            strncpy(padrao_mais_frequente, padrao_atual, MAX_PADRAO);
        }
    }
    
    return max_ocorrencias > 1 ? max_ocorrencias : 0;
}

void limparHistoricoPassos() {
    memset(historico, 0, sizeof(historico));
}

void resetarJogo() {
    player_x = MAPA_LARGURA / 2;
    player_y = MAPA_ALTURA / 2;
    limparHistoricoPassos();
    custom_srand(1322);
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
    initializeLoadingScreen();

    ClearBackground(BLACK);
    BeginDrawing();
    EndDrawing();

    teveUnload = 1;

    Sound musicaMapa0 = LoadSound("static/music/mapa0musica.wav");
    Sound musicaMapa1 = LoadSound("static/music/mapa1musica.wav");
    Sound musicaMapa2 = LoadSound("static/music/mapa2musica.wav");

    spellCastSound = LoadSound("static/music/spellcast.mp3");
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

    player_x = MAPA_LARGURA / 2;
    player_y = MAPA_ALTURA / 2;
    memset(historico, 0, sizeof(historico));
    bool pertoDoPortal = false;

    while (!WindowShouldClose()) {
        int dx = 0, dy = 0;
        char movimento = '\0';

        // Atualiza o nível de água de acordo com o mapa atual
        updateWaterLevel(currentScreen);

        if (playerWater <= 0.0) {
            *currentScreen = RANKINGS;
            resetarJogo();
            break;
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
            player_x = MAPA_LARGURA / 2;
            player_y = MAPA_ALTURA / 2;
            if(mapaAtual == 0) {
                UnloadSound(musicaMapa0);
                teveUnload = 0;
            } else if(mapaAtual == 1) {
                UnloadSound(musicaMapa1);
                teveUnload = 0;
            } else if(mapaAtual == 2) {
                UnloadSound(musicaMapa2);
                teveUnload = 0;
            }

            ClearBackground(BLACK);
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
                char padrao_completo[MAX_PADRAO + 1];
                for (int i = 0; i < dificuldade; i++) {
                    padrao_completo[i] = padrao_encontrado[0];
                }
                padrao_completo[dificuldade] = '\0';

                Sound gameOverSound = LoadSound("static/music/deathsound.wav");
                Sound barulhoMonstro = LoadSound("static/music/monster.mp3");
                Texture2D characterBack = LoadTexture("static/image/characterback.png");
                Texture2D sandworm = LoadTexture("static/image/sandworm.png");

                zerarMonetaria();

                teveUnload = 0;
                UnloadSound(musicaMapa0);
                UnloadSound(musicaMapa1);
                UnloadSound(musicaMapa2);

                PlaySound(gameOverSound);

                sleep(1);

                for (int i = 0; i < 180; i++) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                    DrawText(TextFormat("GAME OVER - Padrão repetido: \"%s\" encontrado", padrao_completo), 10, 40, 20, RED);
                    EndDrawing();
                }

                sleep(2);

                PlaySound(barulhoMonstro);
                Sound deathEmotiva = LoadSound("static/music/deathemotiva.wav");
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

                UnloadSound(gameOverSound);
                UnloadSound(barulhoMonstro);
                UnloadTexture(characterBack);
                UnloadTexture(sandworm);

                // Exibição do texto com animação de digitação
                const char *euFalhei = "Eu... Eu falhei minha missão...";
                int caractereExibido = 0;
                float tempoPorCaractere = 0.5f;  // Tempo em segundos para cada caractere aparecer
                float timer = 0;

                while (caractereExibido < strlen(euFalhei) && !WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    timer += GetFrameTime();

                    // Exibe um caractere a cada intervalo de tempo
                    if (timer >= tempoPorCaractere) {
                        caractereExibido++;
                        timer = 0;
                    }

                    DrawText(TextSubtext(euFalhei, 0, caractereExibido), 
                             GetScreenWidth() / 2 - MeasureText(euFalhei, 20) / 2, 
                             GetScreenHeight() / 2, 20, RAYWHITE);
                    EndDrawing();
                }

                // Exibe o texto completo por mais 2 segundos antes de ir para o ranking
                sleep(2);

                resetarJogo();
                
                *currentScreen = RANKINGS;
                break;
            }
        }

        BeginDrawing();
        drawGame();
        EndDrawing();
    }

    if (teveUnload != 0) {
        UnloadSound(musicaMapa0);
        UnloadSound(musicaMapa1);
        UnloadSound(musicaMapa2);
    }
}



// Função para liberar recursos no final do jogo
void UnloadAssets() {
    for (int i = 0; i < 4; i++) {
        UnloadTexture(loadingImagesMap0[i]);
        UnloadTexture(loadingImagesMap1[i]);
        UnloadTexture(loadingImagesMap2[i]);
        UnloadTexture(loadingImagesLobby[i]);
    }
}
