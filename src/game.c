#include "game.h"

#define NUM_ITEMS 5
#define MAX_HISTORICO 1000
#define MAX_PADRAO 9

int playerMoney = 0;  // Definição
Texture2D personagem;
Texture2D environment;

// Arrays para armazenar as imagens de carregamento
Texture2D loadingImagesMap0[4];
Texture2D loadingImagesMap1[4];
Texture2D loadingImagesMap2[4];

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

    // Verifica se as texturas foram carregadas corretamente
    for (int i = 0; i < 4; i++) {
        if (loadingImagesMap0[i].id == 0) {
            printf("Erro ao carregar a imagem de carregamento mapa 0 %d.\n", i + 1);
        }
        if (loadingImagesMap1[i].id == 0) {
            printf("Erro ao carregar a imagem de carregamento mapa 1 %d.\n", i + 1);
        }
        if (loadingImagesMap2[i].id == 0) {
            printf("Erro ao carregar a imagem de carregamento mapa 2 %d.\n", i + 1);
        }
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

// Inicializa itens colecionáveis
void initializeItems() {
    for (int i = 0; i < NUM_ITEMS; i++) {
        items[i].position.x = custom_rand() % MAPA_LARGURA;
        items[i].position.y = custom_rand() % MAPA_ALTURA;
        items[i].collected = false;
    }
}

// Movimenta o jogador
void movePlayer(int dx, int dy) {
    int new_x = player_x + dx;
    int new_y = player_y + dy;

    // Ajuste a posição e tamanho de acordo com o `environment`
    Rectangle hitboxDuna = {5 * TILE_SIZE, 5 * TILE_SIZE, 96, 96};
    Rectangle hitboxPersonagem = {new_x * TILE_SIZE, new_y * TILE_SIZE, 32, 64};  // Posição futura do jogador em pixels

    // Verifica se a nova posição está dentro dos limites do mapa
    if (new_x >= 0 && new_x < MAPA_LARGURA && new_y >= 0 && new_y < MAPA_ALTURA) {
        bool emLobby = (mapaAtual == -1);  // -1 indica que está no lobby

        // Verifica colisão com os portais no lobby
        if (emLobby) {
            if (
                (new_x >= PORTAL_LOBBY_MAPA1_X && new_x < PORTAL_LOBBY_MAPA1_X + PORTAL_HORIZONTAL_LARGURA &&
                 new_y >= PORTAL_LOBBY_MAPA1_Y && new_y < PORTAL_LOBBY_MAPA1_Y + PORTAL_HORIZONTAL_ALTURA) ||
                (new_x >= PORTAL_LOBBY_MAPA2_X && new_x < PORTAL_LOBBY_MAPA2_X + PORTAL_VERTICAL_LARGURA &&
                 new_y >= PORTAL_LOBBY_MAPA2_Y && new_y < PORTAL_LOBBY_MAPA2_Y + PORTAL_VERTICAL_ALTURA) ||
                (new_x >= PORTAL_LOBBY_MAPA3_X && new_x < PORTAL_LOBBY_MAPA3_X + PORTAL_HORIZONTAL_LARGURA &&
                 new_y >= PORTAL_LOBBY_MAPA3_Y && new_y < PORTAL_LOBBY_MAPA3_Y + PORTAL_HORIZONTAL_ALTURA)
            ) {
                return;
            }
        }

        // Impede o jogador de entrar no portal de retorno no mapa
        if (!emLobby) {
            if (new_x >= PORTAL_RETORNO_X && new_x < PORTAL_RETORNO_X + PORTAL_VERTICAL_LARGURA &&
                new_y >= PORTAL_RETORNO_Y && new_y < PORTAL_RETORNO_Y + PORTAL_VERTICAL_ALTURA) {
                return;
            }
        }

        // Verifica colisão com o mercador no lobby
        if (emLobby && new_x == MERCHANT_X && new_y == MERCHANT_Y) {
            return;
        }

        // Verifica colisão com o `environment`
        if (CheckCollisionRecs(hitboxPersonagem, hitboxDuna)) {
            return;  // Impede o movimento ao colidir com o environment
        }

        // Se todas as verificações passarem, atualiza a posição do jogador
        player_x = new_x;
        player_y = new_y;
    }
}

// Verifica a coleta de itens
void checkItemCollection() {
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (!items[i].collected && items[i].position.x == player_x && items[i].position.y == player_y) {
            // Só permite a coleta se não tiver atingido o máximo de especiarias
            if (itemsCollected < MAX_ESPECIARIAS) {
                items[i].collected = true;

                // Aplica o valor da especiaria baseado no mapa atual
                int especiariasGanhas = 0;
                switch (mapaAtual) {
                    case 0:
                        especiariasGanhas = 1;  // Mapa 1: Cada especiaria vale 1
                        break;
                    case 1:
                        especiariasGanhas = 2;  // Mapa 2: Cada especiaria vale 2
                        break;
                    case 2:
                        especiariasGanhas = 4;  // Mapa 3: Cada especiaria vale 4
                        break;
                }

                // Adiciona as especiarias coletadas, respeitando a capacidade máxima
                itemsCollected += especiariasGanhas;
                if (itemsCollected > MAX_ESPECIARIAS) {
                    itemsCollected = MAX_ESPECIARIAS;  // Limita ao máximo permitido
                }
            } else {
                // Mensagem ao jogador caso o limite tenha sido atingido
                DrawText("Bolsa cheia! Não é possível coletar mais especiarias.", 10, 30, 20, RED);
            }
        }
    }
}

// Função de desenho do jogo
int frameAtual = 0;
float tempoAnimacao = 0;
const float duracaoFrame = 0.2f;  // Duração de cada quadro em segundos

// Função de desenho do jogo
void drawGame() {
    ClearBackground(RAYWHITE);

    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){195, 160, 81, 255});
        }
    }

    environment = LoadTexture("static/image/environment.png");

    Rectangle hitboxDuna = {96, 96, 96, 96};
    Vector2 posicaoDuna = {20, 20};

    personagem = LoadTexture("static/image/spritesheet-character.png");

    // Atualiza o quadro de animação com base no tempo
    tempoAnimacao += GetFrameTime();
    if (tempoAnimacao >= duracaoFrame) {
        frameAtual = (frameAtual + 1) % 2;  // Alterna entre os quadros 0 e 1
        tempoAnimacao = 0;
    }

    // Define o retângulo de origem com base no quadro atual
    Rectangle sourceRec = { frameAtual * 32, 0, 32, 64 };
    Vector2 position = { player_x * TILE_SIZE, player_y * TILE_SIZE };

    // Desenha a textura animada na posição do jogador
    DrawTextureRec(environment, hitboxDuna, posicaoDuna, WHITE);
    DrawTextureRec(personagem, sourceRec, position, WHITE);

    for (int i = 0; i < NUM_ITEMS; i++) {
        if (!items[i].collected) {
            DrawCircle(items[i].position.x * TILE_SIZE + TILE_SIZE / 2, items[i].position.y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 4, GOLD);
        }
    }

    // Mostra a quantidade de especiarias coletadas na "bolsa"
    DrawText(TextFormat("Especiarias na bolsa: %d/%d", itemsCollected, MAX_ESPECIARIAS), 10, 10, 20, BLACK);

    // Desenha o portal de retorno ao lobby
    DrawRectangle(PORTAL_RETORNO_X * TILE_SIZE, PORTAL_RETORNO_Y * TILE_SIZE,
                  TILE_SIZE * PORTAL_RETORNO_LARGURA, TILE_SIZE * PORTAL_RETORNO_ALTURA, ORANGE);

    // Exibe a mensagem no centro da tela, se o jogador estiver em volta do portal
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
    limparHistoricoPassos();  // Limpa o histórico de passos

    // Reconfigura o jogo como se fosse uma nova partida
    custom_srand(1322);  // Semente para a randomização
    initializeItems();
}

void zerarMonetaria() {
    itemsCollected = 0;  // Zera o número de especiarias (itens) coletadas
    playerMoney = 0;
}

bool isPlayerNearPortal() {
    int portalMinX = PORTAL_RETORNO_X;
    int portalMaxX = PORTAL_RETORNO_X + PORTAL_RETORNO_LARGURA - 1;
    int portalMinY = PORTAL_RETORNO_Y;
    int portalMaxY = PORTAL_RETORNO_Y + PORTAL_RETORNO_ALTURA - 1;

    // Verifica se o jogador está em uma célula adjacente ao portal
    if ((abs(player_x - portalMinX) <= 1 && player_y >= portalMinY - 1 && player_y <= portalMaxY + 1) ||
        (abs(player_x - portalMaxX) <= 1 && player_y >= portalMinY - 1 && player_y <= portalMaxY + 1) ||
        (abs(player_y - portalMinY) <= 1 && player_x >= portalMinX - 1 && player_x <= portalMaxX + 1) ||
        (abs(player_y - portalMaxY) <= 1 && player_x >= portalMinX - 1 && player_x <= portalMaxX + 1)) {
        return true;
    }
    return false;
}

void playGame(GameScreen *currentScreen) {
    // Inicializa a tela de carregamento antes de iniciar o jogo
    initializeLoadingScreen();

    // Tela preta antes de mostrar as imagens de carregamento
    ClearBackground(BLACK);
    BeginDrawing();
    EndDrawing();
    
    // Toca o som de spellcast
    spellCastSound = LoadSound("static/music/spellcast.mp3");
    PlaySound(spellCastSound);
    
    // Espera 2 segundos
    sleep(2);


    // Exibe a sequência de telas de carregamento com base no mapa atual
    if (mapaAtual == 0) {
        Sound musicaMapa0 = LoadSound("static/music/mapa0musica.wav");
        PlaySound(musicaMapa0);
        showLoadingScreen(loadingImagesMap0);
    } else if (mapaAtual == 1) {
        Sound musicaMapa1 = LoadSound("static/music/mapa1musica.wav");
        PlaySound(musicaMapa1);
        showLoadingScreen(loadingImagesMap1);
    } else if (mapaAtual == 2) {
        Sound musicaMapa2 = LoadSound("static/music/mapa2musica.wav");
        PlaySound(musicaMapa2);
        showLoadingScreen(loadingImagesMap2);
    }

    int dificuldade;

    // Define a dificuldade com base no mapa atual
    switch (mapaAtual) {
        case 0: dificuldade = 5; break;
        case 1: dificuldade = 4; break;
        case 2: dificuldade = 3; break;
    }

    if (mapaAtual != -1) {  // Somente inicializa se estiver em um mapa
        resetarJogo();
    }

    // Define a posição inicial padrão do jogador (por exemplo, centro do mapa)
    player_x = MAPA_LARGURA / 2;
    player_y = MAPA_ALTURA / 2;

    char historico[MAX_HISTORICO] = "";
    memset(historico, 0, sizeof(historico));

    bool pertoDoPortal = false;

    while (!WindowShouldClose()) {
        int dx = 0, dy = 0;
        char movimento = '\0';

        if (IsKeyPressed(KEY_W)) { dy = -1; movimento = 'w'; }
        if (IsKeyPressed(KEY_S)) { dy = 1; movimento = 's'; }
        if (IsKeyPressed(KEY_A)) { dx = -1; movimento = 'a'; }
        if (IsKeyPressed(KEY_D)) { dx = 1; movimento = 'd'; }

        // Verifica se o jogador está próximo do portal
        if (isPlayerNearPortal()) {
            mensagem = "Você deseja voltar para o lobby? Pressione [P]";
            pertoDoPortal = true;
        } else {
            mensagem = NULL;
            pertoDoPortal = false;
        }

        // Verifica se o jogador está perto do portal e pressionou ENTER para retornar ao lobby
        if (pertoDoPortal && IsKeyPressed(KEY_P)) {
            player_x = MAPA_LARGURA / 2;
            player_y = MAPA_ALTURA / 2;
            *currentScreen = LOBBY;
            mapaAtual = -1;
            mensagem = NULL;
            break;
        }

        if (movimento != '\0') {
            movePlayer(dx, dy);
            checkItemCollection();

            // Armazena o movimento no histórico
            size_t len = strlen(historico);
            if (len < MAX_HISTORICO - 1) {
                historico[len] = movimento;
                historico[len + 1] = '\0';
            }

            char padrao_encontrado[MAX_PADRAO + 1] = "";
            int encontrou_padrao = 0;
            if ((dificuldade == 5 && strlen(historico) >= 5 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 5, padrao_encontrado))) ||
                (dificuldade == 4 && strlen(historico) >= 4 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 4, padrao_encontrado))) ||
                (dificuldade == 3 && strlen(historico) >= 3 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 3, padrao_encontrado)))) 
            {
                char padrao_completo[MAX_PADRAO + 1];
                for (int i = 0; i < dificuldade; i++) {
                    padrao_completo[i] = padrao_encontrado[0];
                }
                padrao_completo[dificuldade] = '\0';

                // Realiza a animação do game over
                Sound gameOverSound = LoadSound("static/music/deathsound.wav");
                Sound barulhoMonstro = LoadSound("static/music/monster.mp3");
                Texture2D characterBack = LoadTexture("static/image/characterback.png");
                Texture2D sandworm = LoadTexture("static/image/sandworm.png");

                // Zera a quantidade de especiarias coletadas no game over
                zerarMonetaria();  // Zera a quantidade de especiarias coletadas

                // Toca o deathsound imediatamente
                PlaySound(gameOverSound);

                sleep(1);

                // Mostra o game over
                for (int i = 0; i < 180; i++) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                    DrawText(TextFormat("GAME OVER - Padrão repetido: \"%s\" encontrado", padrao_completo), 10, 40, 20, RED);
                    EndDrawing();
                }

                // Pausa antes de tocar o som do monstro
                sleep(2);

                // Toca o barulho do monstro uma única vez
                PlaySound(barulhoMonstro);

                // Exibe a animação da sandworm
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

                resetarJogo();
                *currentScreen = RANKINGS;
                break;
            }
        }

        BeginDrawing();
        drawGame();
        EndDrawing();

        if (itemsCollected == NUM_ITEMS) break;
    }
}

// Função para liberar recursos no final do jogo
void UnloadAssets() {
    for (int i = 0; i < 4; i++) {
        UnloadTexture(loadingImagesMap0[i]);  // Libera cada imagem de carregamento do mapa 0
        UnloadTexture(loadingImagesMap1[i]);  // Libera cada imagem de carregamento do mapa 1
        UnloadTexture(loadingImagesMap2[i]);  // Libera cada imagem de carregamento do mapa 2
    }
}

