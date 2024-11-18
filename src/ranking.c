#include "ranking.h"

Player *head = NULL;

static Texture2D backgroundTexture;
static Sound rankingMusic;

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
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        char *delimiter = strchr(line, '|');
        if (delimiter) {
            *delimiter = '\0';
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

    backgroundTexture = LoadTexture("static/image/fundo2.png");
    rankingMusic = LoadSound("static/music/musica.mp3");
    PlaySound(rankingMusic);

    int paginaAtual = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawTexture(backgroundTexture, 0, 0, WHITE);

        const char *titulo = "Ranking:";
        int tituloFontSize = 120;
        int tituloWidth = MeasureText(titulo, tituloFontSize);
        int tituloX = (GetScreenWidth() - tituloWidth) / 2;
        int tituloY = 50;

        DrawText(titulo, tituloX + 4, tituloY + 4, tituloFontSize, BLACK);
        DrawText(titulo, tituloX, tituloY, tituloFontSize, WHITE);

        Player *atual = head;
        int yPosition = tituloY + 160;
        int position = 1;
        int startPos = paginaAtual * 10 + 1;
        int endPos = startPos + 9;

        int posicaoColuna = GetScreenWidth() / 3;
        int nomeColuna = posicaoColuna + 50;
        int pontuacaoColuna = 2 * GetScreenWidth() / 3;

        while (atual && position < startPos) {
            atual = atual->next;
            position++;
        }

        while (atual && position <= endPos) {
            char posicaoTexto[16], nomeTexto[100], pontuacaoTexto[20];
            snprintf(posicaoTexto, sizeof(posicaoTexto), "%d", position);
            snprintf(nomeTexto, sizeof(nomeTexto), "%s", atual->name);
            snprintf(pontuacaoTexto, sizeof(pontuacaoTexto), "%d", atual->score);

            DrawText(posicaoTexto, posicaoColuna, yPosition, 20, WHITE);
            DrawText(nomeTexto, nomeColuna, yPosition, 20, WHITE);
            DrawText(pontuacaoTexto, pontuacaoColuna, yPosition, 20, WHITE);

            yPosition += 30;
            atual = atual->next;
            position++;
        }

        const char *instrucoes = "Q => para voltar ao menu principal";
        const char *anterior = "A => para página anterior";
        const char *proxima = "D => para próxima página";

        int totalWidth = MeasureText(anterior, 20) + MeasureText(" | ", 20) * 2 + MeasureText(instrucoes, 20) + MeasureText(proxima, 20);
        int instrY = GetScreenHeight() - 150;

        int instrX = (GetScreenWidth() - totalWidth) / 2;

        DrawText(anterior, instrX, instrY, 20, WHITE);
        instrX += MeasureText(anterior, 20);
        DrawText("   |   ", instrX, instrY, 20, WHITE);
        instrX += MeasureText("   |   ", 20);
        DrawText(instrucoes, instrX, instrY, 20, WHITE);
        instrX += MeasureText(instrucoes, 20);
        DrawText("   |   ", instrX, instrY, 20, WHITE);
        instrX += MeasureText("   |   ", 20);
        DrawText(proxima, instrX, instrY, 20, WHITE);
                    

        EndDrawing();

        if (IsKeyPressed(KEY_A) && paginaAtual > 0) {
            paginaAtual--;
        }
        if (IsKeyPressed(KEY_D) && atual) {
            paginaAtual++;
        }
        if (IsKeyPressed(KEY_Q)) {
            *currentScreen = TITLE;
            break;
        }
    }

    UnloadTexture(backgroundTexture);
    StopSound(rankingMusic);
    UnloadSound(rankingMusic);

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