// src/menu.c
#include "raylib.h"
#include "menu.h"
#include "game.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Variáveis para elementos visuais e música
static Texture2D background, logo;
static Music titleMusic;
static float backgroundPosX;

// Inicializa o menu com música e texturas
void iniciarMenu(GameScreen *currentScreen, int *dificuldade) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DuneII");
    InitAudioDevice();

    background = LoadTexture("static/image/fundo2.png");
    logo = LoadTexture("static/image/logonova.png");
    titleMusic = LoadMusicStream("static/music/epicversion3.wav");

    PlayMusicStream(titleMusic);
    SetTargetFPS(60);

    *currentScreen = TITLE; // Define a tela inicial como TITLE
}

// Atualiza a lógica do menu, incluindo a seleção de dificuldade
void atualizarMenu(GameScreen *currentScreen, int *dificuldade) {
    UpdateMusicStream(titleMusic);

    if (*currentScreen == TITLE) {
        // Movimento de fundo
        backgroundPosX += 0.2f;
        if (backgroundPosX >= background.width) {
            backgroundPosX = 0;
        }

        // Seleção de dificuldade
        if (IsKeyPressed(KEY_ONE)) *dificuldade = 5;
        if (IsKeyPressed(KEY_TWO)) *dificuldade = 4;
        if (IsKeyPressed(KEY_THREE)) *dificuldade = 3;
        if (IsKeyPressed(KEY_FOUR)) *dificuldade = 9;

        // Iniciar o jogo ao pressionar ENTER
        if (IsKeyPressed(KEY_ENTER)) {
            *currentScreen = GAME;  // Muda para a tela do jogo
        }
        else if (IsKeyPressed(KEY_R)) {
            *currentScreen = RANKINGS;  // Muda para a tela de rankings ao pressionar R
            PauseMusicStream(titleMusic);
        }
    }
    else if (*currentScreen == RANKINGS && IsKeyPressed(KEY_ESCAPE)) {
        *currentScreen = TITLE;
        ResumeMusicStream(titleMusic);
    }
}

// Desenha o menu principal com as opções de dificuldade e transição para o jogo
void desenharMenu(GameScreen currentScreen) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (currentScreen == TITLE) {
        // Define a escala para que o background cubra a tela
        float scaleX = (float)SCREEN_WIDTH / background.width;
        float scaleY = (float)SCREEN_HEIGHT / background.height;
        float scale = scaleX > scaleY ? scaleX : scaleY;

        // Desenha o fundo em loop horizontal
        DrawTextureEx(background, (Vector2){-backgroundPosX, 0}, 0.0f, scale, WHITE);
        
        // Desenha uma cópia do background à direita para criar o loop contínuo
        DrawTextureEx(background, (Vector2){-backgroundPosX + background.width * scale, 0}, 0.0f, scale, WHITE);
        
        // Atualiza a posição do background para movimento
        backgroundPosX += 0.2f;  // Ajuste a velocidade conforme desejado
        if (backgroundPosX >= background.width * scale) {
            backgroundPosX = 0;  // Reinicia o loop
        }

        // Desenha a logo e o texto do menu

        // Atualmente a dificuldade não esta sendo escolhida! ps: Carlos

        DrawTexture(logo, SCREEN_WIDTH / 2 - logo.width / 2, 150, WHITE);
        DrawText("Pressione ENTER para Jogar", SCREEN_WIDTH / 2 - 150, 400, 20, WHITE);
        //DrawText("Escolha o nível de dificuldade:", SCREEN_WIDTH / 2 - 150, 450, 20, WHITE);
        //DrawText("1 - Fácil (5 letras)", SCREEN_WIDTH / 2 - 150, 500, 20, WHITE);
        //DrawText("2 - Médio (4 letras)", SCREEN_WIDTH / 2 - 150, 550, 20, WHITE);
        //DrawText("3 - Difícil (3 letras)", SCREEN_WIDTH / 2 - 150, 600, 20, WHITE);
        //DrawText("4 - Impossível (3 a 9 letras)", SCREEN_WIDTH / 2 - 150, 650, 20, WHITE);
        //DrawText("Pressione R para Rankings", SCREEN_WIDTH / 2 - 150, 700, 20, WHITE);
    } 
    else if (currentScreen == RANKINGS) {
        DrawText("Tela de Rankings", SCREEN_WIDTH / 2 - 100, 200, 30, WHITE);
        DrawText("Pressione ESC para voltar", SCREEN_WIDTH / 2 - 150, 400, 20, WHITE);
    }

    EndDrawing();
}

// Finaliza e libera os recursos do menu
void finalizarMenu() {
    UnloadTexture(background);
    UnloadTexture(logo);
    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    CloseAudioDevice();
    CloseWindow();
}
