#include "raylib.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Enumeração das telas do jogo
typedef enum GameScreen { TITLE, RANKINGS } GameScreen;

int main(void)
{
    // Inicialização da janela e do sistema de áudio
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DuneII");
    InitAudioDevice();  // Inicializa o dispositivo de áudio

    // Carrega as texturas do fundo, plataforma, e personagem
    Texture2D background = LoadTexture("/home/kali/Desktop/DuneII-jogo/fundo2.png");
    Texture2D platform = LoadTexture("platform.png");
    Texture2D character = LoadTexture("character.png");

    // Carrega a logo do jogo (uma imagem fictícia de exemplo)
    Texture2D logo = LoadTexture("/home/kali/Desktop/DuneII-jogo/logonova.png");

    // Carrega a música de fundo para a tela de título
    Music titleMusic = LoadMusicStream("/home/kali/Desktop/DuneII-jogo/epicversion3.wav");

    // Calcula a posição da logo
    Vector2 logoPos = { SCREEN_WIDTH / 2 - logo.width / 2, 150 };

    float backgroundPosX = 0.0f;  // Posição do background para o movimento

    // Variável para gerenciar a tela atual
    GameScreen currentScreen = TITLE;

    // Inicia a reprodução da música em loop
    PlayMusicStream(titleMusic);

    SetTargetFPS(60);

    // Loop principal do jogo
    while (!WindowShouldClose())
    {
        // Atualiza a música
        UpdateMusicStream(titleMusic);

        // Verifica qual tela está ativa e realiza a lógica correspondente
        if (currentScreen == TITLE)
        {
            // Movimento lento do background da esquerda para a direita
            backgroundPosX += 0.2f; // Velocidade de movimento do fundo
            if (backgroundPosX >= background.width) {
                backgroundPosX = 0; // Reinicia o loop do movimento
            }
            
            if (IsKeyPressed(KEY_R)) {
                currentScreen = RANKINGS;  // Muda para a tela de rankings ao pressionar R
                PauseMusicStream(titleMusic);  // Pausa a música ao entrar nos rankings
            }
        }
        else if (currentScreen == RANKINGS)
        {
            // Lógica da tela de rankings
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = TITLE;
                ResumeMusicStream(titleMusic);  // Retoma a música ao voltar para o menu
            }
        }

        // Desenha a tela correspondente
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == TITLE)
        {
            // Define o fator de escala para cobrir toda a tela em largura e altura
            float scaleX = (float)SCREEN_WIDTH / background.width;
            float scaleY = (float)SCREEN_HEIGHT / background.height;
            float scale = scaleX > scaleY ? scaleX : scaleY;

            // Desenha o fundo com movimento contínuo e espelhamento horizontal
            DrawTextureEx(background, (Vector2){-backgroundPosX, 0}, 0.0f, scale, WHITE);
            DrawTextureRec(background, (Rectangle){0, 0, -background.width, background.height}, 
                           (Vector2){-backgroundPosX + background.width * scale, 0}, WHITE);

            // Desenha a logo no topo da tela
            DrawTexture(logo, (int)logoPos.x, (int)logoPos.y, WHITE);

            // Opções de menu
            DrawText("Pressione ENTER para Jogar", SCREEN_WIDTH / 2 - 150, 400, 20, WHITE);
            DrawText("Pressione R para Rankings", SCREEN_WIDTH / 2 - 150, 450, 20, WHITE);
        }
        else if (currentScreen == RANKINGS)
        {
            // Exemplo de tela de rankings
            DrawText("Tela de Rankings", SCREEN_WIDTH / 2 - 100, 200, 30, WHITE);
            DrawText("Pressione ESC para voltar", SCREEN_WIDTH / 2 - 150, 400, 20, WHITE);
        }

        EndDrawing();
    }

    // Libera as texturas e fecha a janela
    UnloadTexture(background);
    UnloadTexture(platform);
    UnloadTexture(character);
    UnloadTexture(logo);

    // Para a música e fecha o dispositivo de áudio
    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    CloseAudioDevice();

    CloseWindow();

    return 0;
} 