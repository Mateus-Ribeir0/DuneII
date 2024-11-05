#include "game.h"

#define NUM_SAFE_ZONES 5
#define NUM_COLLISION_ZONES 6
#define MAX_POINTS_PER_ZONE 60
#define NUM_ITEMS 5
#define MAX_HISTORICO 1000
#define MAX_PADRAO 9

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[MAX_POINTS_PER_ZONE];
    int num_points;
} IrregularZone;

typedef struct {
    Point position;
    bool collected;
} Item;

IrregularZone safe_zones[3][NUM_SAFE_ZONES];
IrregularZone collision_zones[3][NUM_COLLISION_ZONES];
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

// Inicializa as zonas seguras e de colisão para cada mapa
void gerar_zona_irregular(IrregularZone *zone, int centro_x, int centro_y, int num_pontos) {
    zone->num_points = num_pontos;
    for (int i = 0; i < num_pontos; i++) {
        int offset_x = (custom_rand() % 5) - 2;
        int offset_y = (custom_rand() % 5) - 2;
        zone->points[i].x = centro_x + offset_x;
        zone->points[i].y = centro_y + offset_y;
    }
}

void inicializar_zonas(int mapa) {
    if (mapa == 0) {  // Configuração do mapa 1
        gerar_zona_irregular(&safe_zones[0][0], 10, 10, 20);
        gerar_zona_irregular(&safe_zones[0][1], 35, 5, 30);
        gerar_zona_irregular(&safe_zones[0][2], 60, 15, 25);
        gerar_zona_irregular(&safe_zones[0][3], 20, 25, 30);
        gerar_zona_irregular(&safe_zones[0][4], 45, 30, 25);

        gerar_zona_irregular(&collision_zones[0][0], 25, 8, 15);
        gerar_zona_irregular(&collision_zones[0][1], 50, 20, 20);
        // Bordas
        for (int i = 0; i < MAPA_LARGURA; i++) {
            collision_zones[0][2].points[i] = (Point){i, 0};
            collision_zones[0][3].points[i] = (Point){i, MAPA_ALTURA - 1};
        }
        for (int i = 0; i < MAPA_ALTURA; i++) {
            collision_zones[0][4].points[i] = (Point){0, i};
            collision_zones[0][5].points[i] = (Point){MAPA_LARGURA - 1, i};
        }
        collision_zones[0][2].num_points = MAPA_LARGURA;
        collision_zones[0][3].num_points = MAPA_LARGURA;
        collision_zones[0][4].num_points = MAPA_ALTURA;
        collision_zones[0][5].num_points = MAPA_ALTURA;
    } else if (mapa == 1) {  // Configuração do mapa 2
        gerar_zona_irregular(&safe_zones[1][0], 5, 5, 25);
        gerar_zona_irregular(&safe_zones[1][1], 30, 10, 20);
        gerar_zona_irregular(&safe_zones[1][2], 50, 20, 15);
        gerar_zona_irregular(&safe_zones[1][3], 15, 27, 20);
        gerar_zona_irregular(&safe_zones[1][4], 60, 25, 30);

        gerar_zona_irregular(&collision_zones[1][0], 20, 5, 10);
        gerar_zona_irregular(&collision_zones[1][1], 55, 18, 25);
        // Bordas
        for (int i = 0; i < MAPA_LARGURA; i++) {
            collision_zones[1][2].points[i] = (Point){i, 0};
            collision_zones[1][3].points[i] = (Point){i, MAPA_ALTURA - 1};
        }
        for (int i = 0; i < MAPA_ALTURA; i++) {
            collision_zones[1][4].points[i] = (Point){0, i};
            collision_zones[1][5].points[i] = (Point){MAPA_LARGURA - 1, i};
        }
        collision_zones[1][2].num_points = MAPA_LARGURA;
        collision_zones[1][3].num_points = MAPA_LARGURA;
        collision_zones[1][4].num_points = MAPA_ALTURA;
        collision_zones[1][5].num_points = MAPA_ALTURA;
    } else if (mapa == 2) {  // Configuração do mapa 3
        gerar_zona_irregular(&safe_zones[2][0], 8, 8, 30);
        gerar_zona_irregular(&safe_zones[2][1], 25, 15, 25);
        gerar_zona_irregular(&safe_zones[2][2], 40, 5, 15);
        gerar_zona_irregular(&safe_zones[2][3], 35, 28, 30);
        gerar_zona_irregular(&safe_zones[2][4], 55, 20, 20);

        gerar_zona_irregular(&collision_zones[2][0], 18, 10, 18);
        gerar_zona_irregular(&collision_zones[2][1], 45, 22, 30);
        // Bordas
        for (int i = 0; i < MAPA_LARGURA; i++) {
            collision_zones[2][2].points[i] = (Point){i, 0};
            collision_zones[2][3].points[i] = (Point){i, MAPA_ALTURA - 1};
        }
        for (int i = 0; i < MAPA_ALTURA; i++) {
            collision_zones[2][4].points[i] = (Point){0, i};
            collision_zones[2][5].points[i] = (Point){MAPA_LARGURA - 1, i};
        }
        collision_zones[2][2].num_points = MAPA_LARGURA;
        collision_zones[2][3].num_points = MAPA_LARGURA;
        collision_zones[2][4].num_points = MAPA_ALTURA;
        collision_zones[2][5].num_points = MAPA_ALTURA;
    }
}

// Checa se o jogador está dentro de uma zona
bool is_in_irregular_zone(int x, int y, IrregularZone *zones, int num_zones) {
    for (int i = 0; i < num_zones; i++) {
        for (int j = 0; j < zones[i].num_points; j++) {
            if (x == zones[i].points[j].x && y == zones[i].points[j].y) {
                return true;
            }
        }
    }
    return false;
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

    if (new_x >= 0 && new_x < MAPA_LARGURA && new_y >= 0 && new_y < MAPA_ALTURA) {
        if (!is_in_irregular_zone(new_x, new_y, collision_zones[mapaAtual], NUM_COLLISION_ZONES)) {
            player_x = new_x;
            player_y = new_y;
        }
    }
}

// Verifica a coleta de itens
void checkItemCollection() {
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (!items[i].collected && items[i].position.x == player_x && items[i].position.y == player_y) {
            items[i].collected = true;
            itemsCollected++;
        }
    }
}

// Função de desenho do jogo
void drawGame() {
    ClearBackground(RAYWHITE);

    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, WHITE);

            if (is_in_irregular_zone(x, y, safe_zones[mapaAtual], NUM_SAFE_ZONES)) {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
            } else if (is_in_irregular_zone(x, y, collision_zones[mapaAtual], NUM_COLLISION_ZONES)) {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
            }
        }
    }

    DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (!items[i].collected) {
            DrawCircle(items[i].position.x * TILE_SIZE + TILE_SIZE / 2, items[i].position.y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 4, GOLD);
        }
    }

    DrawText(TextFormat("Itens coletados: %d", itemsCollected), 10, 10, 20, BLACK);
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

void playGame() {
    int dificuldade;
    
    // Define a dificuldade com base no mapa atual
    switch (mapaAtual) {
        case 0:  // Mapa 1
            dificuldade = 5;
            break;
        case 1:  // Mapa 2
            dificuldade = 4;
            break;
        case 2:  // Mapa 3
            dificuldade = 3;
            break;
    }

    custom_srand(1234);
    initializeItems();
    inicializar_zonas(mapaAtual);  // Inicializa as zonas do mapa atual
    itemsCollected = 0;

    // Define a posição inicial padrão do jogador (por exemplo, centro do mapa)
    player_x = MAPA_LARGURA / 2;
    player_y = MAPA_ALTURA / 2;

    char historico[MAX_HISTORICO] = "";
    memset(historico, 0, sizeof(historico));

    while (!WindowShouldClose()) {
        int dx = 0, dy = 0;
        char movimento = '\0';

        if (IsKeyPressed(KEY_W)) { dy = -1; movimento = 'w'; }
        if (IsKeyPressed(KEY_S)) { dy = 1; movimento = 's'; }
        if (IsKeyPressed(KEY_A)) { dx = -1; movimento = 'a'; }
        if (IsKeyPressed(KEY_D)) { dx = 1; movimento = 'd'; }

        if (movimento != '\0') {
            movePlayer(dx, dy);
            checkItemCollection();

            // Verifica se o jogador está em uma zona segura antes de registrar o movimento
            if (!is_in_irregular_zone(player_x, player_y, safe_zones[mapaAtual], NUM_SAFE_ZONES)) {
                size_t len = strlen(historico);
                if (len < MAX_HISTORICO - 1) {
                    historico[len] = movimento;
                    historico[len + 1] = '\0';
                }
            } else {
                // Zera o histórico quando entra em uma safe zone
                memset(historico, 0, sizeof(historico));
            }

            char padrao_encontrado[MAX_PADRAO + 1] = "";
            int encontrou_padrao = 0;
            if ((dificuldade == 5 && strlen(historico) >= 5 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 5, padrao_encontrado))) ||
                (dificuldade == 4 && strlen(historico) >= 4 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 4, padrao_encontrado))) ||
                (dificuldade == 3 && strlen(historico) >= 3 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 3, padrao_encontrado)))) 
                {
                
                for (size_t i = 3; i <= dificuldade; i++) {
                    if (strlen(historico) >= i && (encontrou_padrao = identificar_padrao_mais_frequente(historico, i, padrao_encontrado))) {
                        break;
                    }
                }
            }

            if (encontrou_padrao) {
                Sound gameOverSound = LoadSound("static/music/deathsound.wav");
                Sound barulhoMonstro = LoadSound("static/music/monster.mp3");
                Texture2D characterBack = LoadTexture("static/image/characterback.png");
                Texture2D sandworm = LoadTexture("static/image/sandworm.png");

                // Toca o deathsound imediatamente
                PlaySound(gameOverSound);

                sleep(1);

                for (int i = 0; i < 180; i++) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE); // Desenha apenas o personagem
                    DrawText(TextFormat("GAME OVER - Padrão repetido: \"%s\" encontrado", padrao_encontrado), 10, 40, 20, RED);
                    EndDrawing();
                }

                // Pausa de 2 segundos antes de tocar o som do monstro
                sleep(2);

                // Toca o barulho do monstro uma única vez
                PlaySound(barulhoMonstro);

                // Define a posição inicial da sandworm no centro da tela
                int sandwormPosY = GetScreenHeight() / 2 - sandworm.height / 2;
                int startTime = GetTime(); // Marca o tempo inicial

                // Mostra a nova tela com characterback sobreposta à animação da sandworm subindo
                while ((GetTime() - startTime < 5) && !WindowShouldClose()) {
                    sandwormPosY -= 1; // Move a sandworm para cima lentamente

                    BeginDrawing();
                    ClearBackground(BLACK);

                    // Desenha sandworm no centro da tela e move-a para cima
                    DrawTexture(sandworm, GetScreenWidth() / 2 - sandworm.width / 2, sandwormPosY + 40, WHITE);

                    // Desenha characterback acima da sandworm, na borda inferior da tela
                    DrawTexture(characterBack, 0, GetScreenHeight() - characterBack.height, WHITE);

                    EndDrawing();

                    sleep(1); // Controla a velocidade de subida
                }

                // Libera os recursos de áudio e texturas
                UnloadSound(gameOverSound);
                UnloadSound(barulhoMonstro);
                UnloadTexture(characterBack);
                UnloadTexture(sandworm);

                break;
            }

        }

        BeginDrawing();
        drawGame();
        EndDrawing();

        if (itemsCollected == NUM_ITEMS) break;
    }
}
