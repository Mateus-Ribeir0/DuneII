#include "ranking.h"

Player *head = NULL;

void inserirNaLista(Player *newPlayer) {
    if (!head || head->score < newPlayer->score) {
        newPlayer->next = head;
        head = newPlayer;
    } else {
        Player *atual = head;
        while (atual->next != NULL && atual->next->score >= newPlayer->score) {
            atual = atual->next;
        }
        newPlayer->next = atual->next;
        atual->next = newPlayer;
    }
}

void carregarRanking() {
    FILE *file = fopen("ranking.txt", "r");
    if (!file) return;

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Remove o caractere de nova linha, se houver
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        // Encontra o delimitador '|'
        char *delimiter = strchr(line, '|');
        if (delimiter) {
            *delimiter = '\0'; // Separa o nome da pontuação
            char *name = line;
            char *scoreStr = delimiter + 1;
            int score = atoi(scoreStr);

            Player *newPlayer = (Player *)malloc(sizeof(Player));
            strcpy(newPlayer->name, name);
            newPlayer->score = score;
            newPlayer->next = NULL;
            inserirNaLista(newPlayer);
        }
    }
    fclose(file);
}

void atualizarRanking(const char *name, int score) {
    carregarRanking();

    Player *newPlayer = (Player *)malloc(sizeof(Player));
    strcpy(newPlayer->name, name);
    newPlayer->score = score;
    newPlayer->next = NULL;
    inserirNaLista(newPlayer);

    FILE *file = fopen("ranking.txt", "w");
    if (!file) return;
    Player *atual = head;
    while (atual != NULL) {
        fprintf(file, "%s|%d\n", atual->name, atual->score);
        atual = atual->next;
    }
    fclose(file);

    liberarRanking();
}

void exibirRankingScreen(GameScreen *currentScreen) {
    carregarRanking();

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawText("Ranking:", 100, 50, 30, BLACK);

        Player *atual = head;
        int yPosition = 100;
        int position = 1;
        while (atual && position <= 10) { // Exibe os top 10
            char buffer[100];
            snprintf(buffer, sizeof(buffer), "%d. %s - %d", position, atual->name, atual->score);
            DrawText(buffer, 100, yPosition, 20, BLACK);
            yPosition += 30;
            atual = atual->next;
            position++;
        }

        DrawText("Pressione Q para voltar ao menu principal", 100, yPosition + 50, 20, DARKGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_Q)) {
            *currentScreen = TITLE;
            break;
        }
    }

    liberarRanking();
}

void liberarRanking() {
    Player *atual = head;
    while (atual) {
        Player *temp = atual;
        atual = atual->next;
        free(temp);
    }
    head = NULL;
}