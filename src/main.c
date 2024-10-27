#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define MAX_HISTORICO 1000
#define MAX_PADRAO 9
#define NUM_SAFE_ZONES 5
#define NUM_COLLISION_ZONES 6
#define MAX_POINTS_PER_ZONE 60

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[MAX_POINTS_PER_ZONE];
    int num_points;
} IrregularZone;

static unsigned long seed = 1234;
int MAPA_LARGURA;
int MAPA_ALTURA;

IrregularZone safe_zones[NUM_SAFE_ZONES];
IrregularZone collision_zones[NUM_COLLISION_ZONES];

// Funções de randomização personalizadas
void custom_srand(unsigned long s) {
    seed = s;
}

int custom_rand() {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return (int)(seed % 32768); 
}

// Função para verificar se o jogador está dentro de uma zona irregular
int is_in_irregular_zone(int x, int y, IrregularZone *zones, int num_zones) {
    for (int i = 0; i < num_zones; i++) {
        for (int j = 0; j < zones[i].num_points; j++) {
            if (x == zones[i].points[j].x && y == zones[i].points[j].y) {
                return 1;
            }
        }
    }
    return 0;
}

// Função para contar ocorrências consecutivas de um padrão
int contar_ocorrencias_consecutivas(const char *historico, const char *padrao, size_t padrao_len) {
    int contagem = 0;
    int sequencias_consecutivas = 0;
    size_t historico_len = strlen(historico);

    for (size_t i = 0; i <= historico_len - padrao_len; ) {
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
    // Gerar zonas seguras com formas irregulares
    gerar_zona_irregular(&safe_zones[0], 10, 10, 20);
    gerar_zona_irregular(&safe_zones[1], 35, 5, 30);
    gerar_zona_irregular(&safe_zones[2], 60, 15, 25);
    gerar_zona_irregular(&safe_zones[3], 20, 25, 30);
    gerar_zona_irregular(&safe_zones[4], 45, 30, 25);

    // Gerar zonas de colisão com formas irregulares
    gerar_zona_irregular(&collision_zones[0], 25, 8, 15);
    gerar_zona_irregular(&collision_zones[1], 50, 20, 20);

    // Definir bordas como zonas de colisão
    int idx = 2;
    for (int i = 0; i < MAPA_LARGURA; i++) {
        collision_zones[idx].points[i] = (Point){i, 0};              // Borda superior
        collision_zones[idx + 1].points[i] = (Point){i, MAPA_ALTURA - 1}; // Borda inferior
    }
    for (int i = 0; i < MAPA_ALTURA; i++) {
        collision_zones[idx + 2].points[i] = (Point){0, i};          // Borda esquerda
        collision_zones[idx + 3].points[i] = (Point){MAPA_LARGURA - 1, i}; // Borda direita
    }
    collision_zones[2].num_points = MAPA_LARGURA;
    collision_zones[3].num_points = MAPA_LARGURA;
    collision_zones[4].num_points = MAPA_ALTURA;
    collision_zones[5].num_points = MAPA_ALTURA;
}

void imprimir_mapa(int jogador_x, int jogador_y, int ponto_x, int ponto_y, int pontuacao) {
    screenClear(); 

    for (int i = 0; i < MAPA_ALTURA; i++) {
        for (int j = 0; j < MAPA_LARGURA; j++) {
            if (i == jogador_y && j == jogador_x) {
                printf("O ");
            } else if (i == ponto_y && j == ponto_x) {
                screenSetColor(YELLOW, BLACK);
                printf("* ");
                screenSetNormal();
            } else if (is_in_irregular_zone(j, i, safe_zones, NUM_SAFE_ZONES)) {
                screenSetColor(LIGHTGREEN, BLACK);
                printf("S ");
                screenSetNormal();
            } else if (is_in_irregular_zone(j, i, collision_zones, NUM_COLLISION_ZONES)) {
                screenSetColor(RED, BLACK);
                printf("X ");
                screenSetNormal();
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("Pontuação: %d\n", pontuacao);
    screenUpdate();
}

void menu_principal() {
    char historico[MAX_HISTORICO] = "";
    char entrada;
    size_t dificuldade;
    int jogador_x, jogador_y;
    int ponto_x, ponto_y;
    int pontuacao = 0;

    obterTamanhoTerminal(&MAPA_LARGURA, &MAPA_ALTURA);
    jogador_x = MAPA_LARGURA / 2;
    jogador_y = MAPA_ALTURA / 2;

    custom_srand(1234);
    keyboardInit();
    screenInit(0);

    inicializar_zonas();

    do {
        ponto_x = custom_rand() % MAPA_LARGURA;
        ponto_y = custom_rand() % MAPA_ALTURA;
    } while (is_in_irregular_zone(ponto_x, ponto_y, safe_zones, NUM_SAFE_ZONES) ||
             is_in_irregular_zone(ponto_x, ponto_y, collision_zones, NUM_COLLISION_ZONES));

    printf("Escolha o nível de dificuldade:\n");
    printf("1 - Fácil (5 letras)\n");
    printf("2 - Médio (4 e 5 letras)\n");
    printf("3 - Difícil (3, 4 e 5 letras)\n");
    printf("4 - Impossível (3 a 9 letras)\n");

    entrada = '\0';

    while (1) {
        if (keyhit()) {
            entrada = readch();
            
            if (entrada == '1') {
                dificuldade = 5;
                break;
            } else if (entrada == '2') {
                dificuldade = 4;
                break;
            } else if (entrada == '3') {
                dificuldade = 3;
                break;
            } else if (entrada == '4') {
                dificuldade = 9;
                break;
            } else if (entrada != '\0' && entrada != '\n' && entrada != ' ') {
                printf("\rEscolha inválida. Tente novamente.   ");  
            }
        }
    }


    
    printf("\nDigite jogadas individuais. O programa identifica padrões a partir da %zuª jogada.\n", dificuldade);
    
    imprimir_mapa(jogador_x, jogador_y, ponto_x, ponto_y, pontuacao);
    
    while (1) {
        if (keyhit()) {
            entrada = readch();
            
            int prev_x = jogador_x, prev_y = jogador_y;

            if (entrada == 'w' && jogador_y > 0) jogador_y--;
            if (entrada == 's' && jogador_y < MAPA_ALTURA - 1) jogador_y++;
            if (entrada == 'a' && jogador_x > 0) jogador_x--;
            if (entrada == 'd' && jogador_x < MAPA_LARGURA - 1) jogador_x++;

            if (is_in_irregular_zone(jogador_x, jogador_y, collision_zones, NUM_COLLISION_ZONES)) {
                jogador_x = prev_x;
                jogador_y = prev_y;
            }

            if (is_in_irregular_zone(jogador_x, jogador_y, safe_zones, NUM_SAFE_ZONES)) {
                memset(historico, 0, sizeof(historico));
            }

            if (jogador_x == ponto_x && jogador_y == ponto_y) {
                pontuacao++;
                do {
                    ponto_x = custom_rand() % MAPA_LARGURA;
                    ponto_y = custom_rand() % MAPA_ALTURA;
                } while (is_in_irregular_zone(ponto_x, ponto_y, safe_zones, NUM_SAFE_ZONES) ||
                         is_in_irregular_zone(ponto_x, ponto_y, collision_zones, NUM_COLLISION_ZONES));
            }

            imprimir_mapa(jogador_x, jogador_y, ponto_x, ponto_y, pontuacao);
            
            size_t len = strlen(historico);
            if (len + 1 < MAX_HISTORICO) {
                historico[len] = entrada;
                historico[len + 1] = '\0';
            } else {
                printf("Histórico cheio!\n");
                break;
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
                screenClear();
                printf("GAME OVER\n");
                printf("Padrão repetido: \"%s\" apareceu %d vezes consecutivas.\n", padrao_encontrado, encontrou_padrao);
                screenUpdate();
                sleep(3);
                break;
            }
        }
    }

    keyboardDestroy();
    screenDestroy();
}

int main() {
    while (1) {
        menu_principal();
    }
    return 0;
}
