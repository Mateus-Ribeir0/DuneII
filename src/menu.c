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

void cutsceneArrakis(Music titleMusic) {
    SetMusicVolume(titleMusic, 1.0f);  // Define o volume para garantir que esteja no máximo

    // Carrega as imagens da cutscene
    Texture2D cutsceneImage1 = LoadTexture("static/image/cutscene2.png");
    Texture2D cutsceneImage2 = LoadTexture("static/image/cutscene1.png");
    Texture2D cutsceneImage3 = LoadTexture("static/image/cutscene3.png");
    Texture2D cutsceneImage4 = LoadTexture("static/image/cutscene4.png");
    Texture2D cutsceneImage5 = LoadTexture("static/image/cutscene5.png");
    Texture2D cutsceneImage6 = LoadTexture("static/image/cutscene6.png");
    Texture2D cutsceneImage7 = LoadTexture("static/image/cutscene7.png");
    Texture2D cutsceneImage8 = LoadTexture("static/image/cutscene8.png");
    Texture2D cutsceneImage9 = LoadTexture("static/image/cutscene9.png");

    const char* text1 = "Arrakis. Um vasto deserto, onde a areia carrega cicatrizes das guerras travadas pela especiaria.";
    const char* text2 = "Eu vim em busca desse tesouro raro, mas cada passo é uma batalha. A especiaria não é fácil de conquistar.";
    const char* text3 = "Ela nasce em profundezas traiçoeiras, onde os vermes gigantes,\nprotetores implacáveis do deserto, emergem ao menor sinal de vibração.";
    const char* text4 = "Eles são monstros antigos, vastos e indomáveis, capazes de engolir tudo que cruza seu caminho.";
    const char* text5 = "Aqui, cada grão de especiaria custa suor, sangue e, muitas vezes, vidas.";
    const char* text6 = "No calor abrasador e sob o constante risco de ser detectado por essas criaturas, eu preciso seguir em frente.";
    const char* text7 = "Cada pedaço de especiaria que eu consigo arrancar da areia é uma vitória pequena, mas essencial.";
    const char* text8 = "Minha missão é sobreviver, extrair o que posso e sair\nde Arrakis com o que é preciso para mudar o destino de minha casa.";
    const char* text9 = "Mas neste deserto, cada erro é fatal, e os vermes estão sempre à espreita.";


    float speed = 30.0f; // Velocidade de rolagem
    float scale = 0.6f;  // Escala das imagens
    float timer = 0.0f;  // Timer para controlar a duração das partes
    const float duration = 7.0f;  // Duração de cada parte em segundos

    // Primeira parte da cutscene (movimento da esquerda para a direita)
    timer = 0.0f;  // Reinicia o timer para a primeira parte
    float scrollX1 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX1 -= speed * GetFrameTime();
        if (scrollX1 <= -cutsceneImage1.width * scale) {
            scrollX1 = 0.0f;  // Reinicia a posição da imagem para criar um loop
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage1, (Vector2){scrollX1, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage1, (Vector2){scrollX1 + cutsceneImage1.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na parte superior da tela
        DrawText(text1, GetScreenWidth() / 2 - MeasureText(text1, 20) / 2, 20, 20, RAYWHITE);

        EndDrawing();
    }

    // Segunda parte da cutscene (movimento da direita para a esquerda)
    timer = 0.0f;  // Reinicia o timer para a segunda parte
    float scrollX2 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX2 -= speed * GetFrameTime();
        if (scrollX2 <= -cutsceneImage2.width * scale) {
            scrollX2 = GetScreenWidth();  // Reinicia a posição para começar novamente da direita
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage2, (Vector2){scrollX2, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage2, (Vector2){scrollX2 + cutsceneImage2.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na borda inferior da tela
        DrawText(text2, GetScreenWidth() / 2 - MeasureText(text2, 20) / 2, GetScreenHeight() - 40, 20, BLACK);

        EndDrawing();
    }

    // Terceira parte da cutscene (movimento da esquerda para a direita)
    timer = 0.0f;  // Reinicia o timer para a terceira parte
    float scrollX3 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX3 -= speed * GetFrameTime();
        if (scrollX3 <= -cutsceneImage3.width * scale) {
            scrollX3 = 0.0f;  // Reinicia a posição da imagem para criar um loop
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage3, (Vector2){scrollX3, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage3, (Vector2){scrollX3 + cutsceneImage3.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na parte superior da tela
        DrawText(text3, GetScreenWidth() / 2 - MeasureText(text3, 20) / 2, 20, 20, RAYWHITE);

        EndDrawing();
    }

    timer = 0.0f;  // Reinicia o timer para a quarta parte
    float scrollX4 = 0.0f;  // Inicia a posição da imagem à direita
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem (movimento da direita para a esquerda)
        scrollX4 -= speed * GetFrameTime();
        if (scrollX4 <= -cutsceneImage4.width * scale) {
            scrollX4 = 0.0f;  // Reinicia a posição para começar novamente da direita
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage4, (Vector2){scrollX4, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage4, (Vector2){scrollX4 + cutsceneImage4.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na borda inferior da tela
        DrawText(text4, GetScreenWidth() / 2 - MeasureText(text4, 20) / 2, GetScreenHeight() - 40, 20, BLACK);

        EndDrawing();
    }

    timer = 0.0f;  // Reinicia o timer para a terceira parte
    float scrollX5 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX5 -= speed * GetFrameTime();
        if (scrollX5 <= -cutsceneImage5.width * scale) {
            scrollX5 = 0.0f;  // Reinicia a posição da imagem para criar um loop
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage5, (Vector2){scrollX5, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage5, (Vector2){scrollX5 + cutsceneImage5.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na parte superior da tela
        DrawText(text5, GetScreenWidth() / 2 - MeasureText(text5, 20) / 2, 20, 20, RAYWHITE);

        EndDrawing();
    }

    timer = 0.0f;  // Reinicia o timer para a primeira parte
    float scrollX6 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX6 -= speed * GetFrameTime();
        if (scrollX6 <= -cutsceneImage6.width * scale) {
            scrollX6 = 0.0f;  // Reinicia a posição da imagem para criar um loop
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage6, (Vector2){scrollX6, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage6, (Vector2){scrollX6 + cutsceneImage6.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na parte superior da tela
        DrawText(text6, GetScreenWidth() / 2 - MeasureText(text6, 20) / 2, 20, 20, BLACK);

        EndDrawing();
    }

    timer = 0.0f;  // Reinicia o timer para a terceira parte
    float scrollX7 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX7 -= speed * GetFrameTime();
        if (scrollX7 <= -cutsceneImage7.width * scale) {
            scrollX7 = 0.0f;  // Reinicia a posição da imagem para criar um loop
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage7, (Vector2){scrollX7, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage7, (Vector2){scrollX7 + cutsceneImage7.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na parte superior da tela
        DrawText(text7, GetScreenWidth() / 2 - MeasureText(text7, 20) / 2, GetScreenHeight() - 40, 20, BLACK);

        EndDrawing();
    }

    timer = 0.0f;  // Reinicia o timer para a primeira parte
    float scrollX8 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX8 -= speed * GetFrameTime();
        if (scrollX8 <= -cutsceneImage8.width * scale) {
            scrollX8 = 0.0f;  // Reinicia a posição da imagem para criar um loop
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage8, (Vector2){scrollX8, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage8, (Vector2){scrollX8 + cutsceneImage8.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na parte superior da tela
        DrawText(text8, GetScreenWidth() / 2 - MeasureText(text8, 20) / 2, 20, 20, BLACK);

        EndDrawing();
    }

    timer = 0.0f;  // Reinicia o timer para a segunda parte
    float scrollX9 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX9 -= speed * GetFrameTime();
        if (scrollX9 <= -cutsceneImage9.width * scale) {
            scrollX9 = 0.0f;  // Reinicia a posição para começar novamente da direita
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage9, (Vector2){scrollX9, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage9, (Vector2){scrollX9 + cutsceneImage9.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na borda inferior da tela
        DrawText(text9, GetScreenWidth() / 2 - MeasureText(text9, 20) / 2, GetScreenHeight() - 40, 20, WHITE);

        EndDrawing();
    }


    // Libera a memória das imagens após o uso
    UnloadTexture(cutsceneImage1);
    UnloadTexture(cutsceneImage2);
    UnloadTexture(cutsceneImage3);
    UnloadTexture(cutsceneImage4);
    UnloadTexture(cutsceneImage5);
    UnloadTexture(cutsceneImage6);
    UnloadTexture(cutsceneImage7);
    UnloadTexture(cutsceneImage8);
    UnloadTexture(cutsceneImage9);

}

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
