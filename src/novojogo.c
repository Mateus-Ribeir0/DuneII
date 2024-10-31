#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_HISTORICO 1000
#define MAX_PADRAO 9
#define TILE_SIZE 32 // Pixels
#define MAPA_LARGURA 40 // Largura do mapa em tiles (40x32px)
#define MAPA_ALTURA 30  // Altura do mapa em tiles (30x32px)
#define NUM_ITEMS 5

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point position;
    bool collected;
} Item;

static unsigned long seed = 1234;
Item items[NUM_ITEMS];
int player_x = MAPA_LARGURA / 2;
int player_y = MAPA_ALTURA / 2;
int itemsCollected = 0;

void custom_srand(unsigned long s) {
    seed = s;
}

int custom_rand() {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return (int)(seed % 32768); 
}

void initializeItems() {
    for (int i = 0; i < NUM_ITEMS; i++) {
        items[i].position.x = custom_rand() % MAPA_LARGURA;
        items[i].position.y = custom_rand() % MAPA_ALTURA;
        items[i].collected = false;
    }
}

bool isPositionValid(int x, int y) {
    return (x >= 0 && x < MAPA_LARGURA && y >= 0 && y < MAPA_ALTURA);
}

void movePlayer(int dx, int dy) {
    int new_x = player_x + dx;
    int new_y = player_y + dy;

    if (isPositionValid(new_x, new_y)) {
        player_x = new_x;
        player_y = new_y;
    }
}

void checkItemCollection() {
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (!items[i].collected && items[i].position.x == player_x && items[i].position.y == player_y) {
            items[i].collected = true;
            itemsCollected++;
        }
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

int main() {
    InitWindow(1280, 720, "Tile-Based Game with Step Limits and Collection");
    SetTargetFPS(60);

    custom_srand(1234);
    initializeItems();

    int passosRepetidosMax = 3; // Limite de passos consecutivos na mesma direção
    int passosRepetidos = 0;
    char historico[MAX_HISTORICO] = "";
    char ultimoPasso = '\0';

    while (!WindowShouldClose()) {
        int dx = 0;
        int dy = 0;
        char movimento = '\0';

        // Detecção de tecla de movimento com clique, sem movimento contínuo
        if (IsKeyPressed(KEY_W)) { dy = -1; movimento = 'w'; }
        if (IsKeyPressed(KEY_S)) { dy = 1; movimento = 's'; }
        if (IsKeyPressed(KEY_A)) { dx = -1; movimento = 'a'; }
        if (IsKeyPressed(KEY_D)) { dx = 1; movimento = 'd'; }

        // Atualiza passos consecutivos e histórico de movimentos
        if (movimento != '\0') {
            if (movimento == ultimoPasso) {
                passosRepetidos++;
            } else {
                passosRepetidos = 1;
                ultimoPasso = movimento;
            }

            if (passosRepetidos <= passosRepetidosMax) {
                movePlayer(dx, dy);
                checkItemCollection();

                // Adiciona ao histórico e verifica se excede o limite
                size_t len = strlen(historico);
                if (len + 1 < MAX_HISTORICO) {
                    historico[len] = movimento;
                    historico[len + 1] = '\0';
                }

                int encontrouPadrao = contar_ocorrencias_consecutivas(historico, &movimento, 1);
                if (encontrouPadrao >= passosRepetidosMax) {
                    DrawText("Limite de passos repetidos atingido!", 10, 10, 20, RED);
                    break;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha o mapa em tiles
        for (int y = 0; y < MAPA_ALTURA; y++) {
            for (int x = 0; x < MAPA_LARGURA; x++) {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
            }
        }

        // Desenha o jogador
        DrawRectangle(player_x * TILE_SIZE, player_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);

        // Desenha os itens coletáveis
        for (int i = 0; i < NUM_ITEMS; i++) {
            if (!items[i].collected) {
                DrawCircle(items[i].position.x * TILE_SIZE + TILE_SIZE / 2, items[i].position.y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 4, GOLD);
            }
        }

        // Exibe a quantidade de itens coletados
        DrawText(TextFormat("Itens coletados: %d", itemsCollected), 10, 40, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
