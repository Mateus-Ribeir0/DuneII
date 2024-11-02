// src/game.c
#include "raylib.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TILE_SIZE 32
#define MAPA_LARGURA 40
#define MAPA_ALTURA 30
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

static unsigned long seed = 1234;
IrregularZone safe_zones[NUM_SAFE_ZONES];
IrregularZone collision_zones[NUM_COLLISION_ZONES];
Item items[NUM_ITEMS];
int player_x = MAPA_LARGURA / 2;
int player_y = MAPA_ALTURA / 2;
int itemsCollected = 0;
char historico[MAX_HISTORICO] = "";
int passosRepetidosMax = 3;

// Funções de randomização
void custom_srand(unsigned long s) {
    seed = s;
}

int custom_rand() {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return (int)(seed % 32768); 
}

// Inicializa as zonas seguras e de colisão
void gerar_zona_irregular(IrregularZone *zone, int centro_x, int centro_y, int num_pontos) {
    zone->num_points = num_pontos;
    for (int i = 0; i < num_pontos; i++) {
        int offset_x = (custom_rand() % 5) - 2;
        int offset_y = (custom_rand() % 5) - 2;
        zone->points[i].x = centro_x + offset_x;
        zone->points[i].y = centro_y + offset_y;
    }
}

void inicializar_zonas() {
    gerar_zona_irregular(&safe_zones[0], 10, 10, 20);
    gerar_zona_irregular(&safe_zones[1], 35, 5, 30);
    gerar_zona_irregular(&safe_zones[2], 60, 15, 25);
    gerar_zona_irregular(&safe_zones[3], 20, 25, 30);
    gerar_zona_irregular(&safe_zones[4], 45, 30, 25);

    gerar_zona_irregular(&collision_zones[0], 25, 8, 15);
    gerar_zona_irregular(&collision_zones[1], 50, 20, 20);

    int idx = 2;
    for (int i = 0; i < MAPA_LARGURA; i++) {
        collision_zones[idx].points[i] = (Point){i, 0};
        collision_zones[idx + 1].points[i] = (Point){i, MAPA_ALTURA - 1};
    }
    for (int i = 0; i < MAPA_ALTURA; i++) {
        collision_zones[idx + 2].points[i] = (Point){0, i};
        collision_zones[idx + 3].points[i] = (Point){MAPA_LARGURA - 1, i};
    }
    collision_zones[2].num_points = MAPA_LARGURA;
    collision_zones[3].num_points = MAPA_LARGURA;
    collision_zones[4].num_points = MAPA_ALTURA;
    collision_zones[5].num_points = MAPA_ALTURA;
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
        if (!is_in_irregular_zone(new_x, new_y, collision_zones, NUM_COLLISION_ZONES)) {
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

// Função de desenho do jogo
void drawGame() {
    ClearBackground(RAYWHITE);

    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);

            if (is_in_irregular_zone(x, y, safe_zones, NUM_SAFE_ZONES)) {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
            } else if (is_in_irregular_zone(x, y, collision_zones, NUM_COLLISION_ZONES)) {
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
void playGame(int dificuldade) {
    custom_srand(1234);
    initializeItems();
    inicializar_zonas();
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
            if (!is_in_irregular_zone(player_x, player_y, safe_zones, NUM_SAFE_ZONES)) {
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
                (dificuldade == 3 && strlen(historico) >= 3 && (encontrou_padrao = identificar_padrao_mais_frequente(historico, 3, padrao_encontrado))) ||
                (dificuldade == 9)) {
                for (size_t i = 3; i <= 9 && dificuldade == 9; i++) {
                    if (strlen(historico) >= i && (encontrou_padrao = identificar_padrao_mais_frequente(historico, i, padrao_encontrado))) {
                        break;
                    }
                }
            }

            if (encontrou_padrao) {
                for (int i = 0; i < 180; i++) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText(TextFormat("GAME OVER - Padrão repetido: \"%s\" encontrado", padrao_encontrado), 10, 40, 20, RED);
                    EndDrawing();
                }
                break;
            }
        }

        BeginDrawing();
        drawGame();
        EndDrawing();

        if (itemsCollected == NUM_ITEMS) break;
    }
}


