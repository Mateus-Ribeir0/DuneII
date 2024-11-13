#include "menu.h"

// Variáveis para elementos visuais
static Texture2D background, logo, objetivoImage;
static Music titleMusic;
static float backgroundPosX;

void recebeNomeDoPlayer(GameScreen *currentScreen) {
    static char nameBuffer[MAX_NAME_LENGTH] = "";
    int letra;

    memset(nameBuffer, 0, sizeof(nameBuffer));

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);
        desenharBackgroundComLogo();
        DrawText("Digite seu nome:", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, 20, WHITE);
        DrawRectangle(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, 350, 30, WHITE);
        DrawText(nameBuffer, SCREEN_WIDTH / 2 - 95, SCREEN_HEIGHT / 2 + 105, 20, BLACK);
        DrawText("Pressione ENTER para continuar", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 150, 20, WHITE);
        EndDrawing();

        letra = GetCharPressed();
        while (letra > 0) {
            if ((letra >= 32) && (letra <= 125) && (strlen(nameBuffer) < MAX_NAME_LENGTH - 1)) {
                int len = strlen(nameBuffer);
                nameBuffer[len] = (char)letra;
                nameBuffer[len + 1] = '\0';
            }
            letra = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && strlen(nameBuffer) > 0) {
            nameBuffer[strlen(nameBuffer) - 1] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER) && strlen(nameBuffer) > 0) {
            strcpy(playerName, nameBuffer);
            *currentScreen = CUTSCENE;
            break;
        }
    }
}

void desenharBackgroundComLogo() {
    float scaleX = (float)SCREEN_WIDTH / background.width;
    float scaleY = (float)SCREEN_HEIGHT / background.height;
    float scale = scaleX > scaleY ? scaleX : scaleY;

    DrawTextureEx(background, (Vector2){-backgroundPosX, 0}, 0.0f, scale, WHITE);

    DrawTextureEx(background, (Vector2){-backgroundPosX + background.width * scale, 0}, 0.0f, scale, WHITE);

    backgroundPosX += 0.2f;
    if (backgroundPosX >= background.width * scale) {
        backgroundPosX = 0;
    }

    DrawTexture(logo, SCREEN_WIDTH / 2 - logo.width / 2, 150, WHITE);
}

void cutsceneArrakis(Music titleMusic) {
    SetMusicVolume(titleMusic, 1.0f);

    // Carrega as imagens da cutscene
    Texture2D cutsceneImage1 = LoadTexture("static/image/cutscene2.png");

    const char* text1 = "Arrakis. Um vasto deserto, onde a areia carrega cicatrizes das guerras travadas pela especiaria.";
    const char* text2 = "Eu vim em busca desse tesouro raro, mas cada passo é uma batalha. A especiaria não é fácil de conquistar.";
    const char* text3 = "Ela nasce em profundezas traiçoeiras, onde os vermes gigantes,\nprotetores implacáveis do deserto, emergem ao menor sinal de vibração.";
    const char* text4 = "Eles são monstros antigos, vastos e indomáveis, capazes de engolir tudo que cruza seu caminho.";
    const char* text5 = "Aqui, cada grão de especiaria custa suor, sangue e, muitas vezes, vidas.";
    const char* text6 = "No calor abrasador e sob o constante risco de ser detectado por essas criaturas, eu preciso seguir em frente.";
    const char* text7 = "Cada pedaço de especiaria que eu consigo arrancar da areia é uma vitória pequena, mas essencial.";
    const char* text8 = "Minha missão é sobreviver, extrair o que posso e sair\nde Arrakis com o que é preciso para mudar o destino de minha casa.";
    const char* text9 = "Mas neste deserto, cada erro é fatal, e os vermes estão sempre à espreita.";
    const char* text10 = "Minha missão começa AGORA";


    float speed = 30.0f; // Velocidade de rolagem
    float scale = 0.6f;  // Escala das imagens
    float timer = 0.0f;  // Timer para controlar a duração das partes
    const float duration = 7.0f;  // Duração de cada parte em segundos (7 é o padrão)

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

    Texture2D cutsceneImage2 = LoadTexture("static/image/cutscene1.png");
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

    Texture2D cutsceneImage3 = LoadTexture("static/image/cutscene3.png");
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

    Texture2D cutsceneImage4 = LoadTexture("static/image/cutscene4.png");
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

    Texture2D cutsceneImage5 = LoadTexture("static/image/cutscene5.png");
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

    Texture2D cutsceneImage6 = LoadTexture("static/image/cutscene6.png");

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

    Texture2D cutsceneImage7 = LoadTexture("static/image/cutscene7.png");
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

    Texture2D cutsceneImage8 = LoadTexture("static/image/cutscene8.png");
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

    Texture2D cutsceneImage9 = LoadTexture("static/image/cutscene9.png");
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

    Texture2D cutsceneImage10 = LoadTexture("static/image/cutscene10.png");
    timer = 0.0f;  // Reinicia o timer para a segunda parte
    float scrollX10 = 0.0f;
    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);  // Mantém a música tocando durante a cutscene

        // Atualiza a posição horizontal da imagem
        scrollX10 -= speed * GetFrameTime();
        if (scrollX10 <= -cutsceneImage10.width * scale) {
            scrollX10 = 0.0f;  // Reinicia a posição para começar novamente da direita
        }

        // Desenha a imagem duas vezes para dar efeito de movimento contínuo
        DrawTextureEx(cutsceneImage10, (Vector2){scrollX10, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(cutsceneImage10, (Vector2){scrollX10 + cutsceneImage10.width * scale, 0}, 0.0f, scale, WHITE);

        // Desenha o texto na borda inferior da tela
        DrawText(text10, GetScreenWidth() / 2 - MeasureText(text10, 20) / 2, GetScreenHeight() - 40, 20, WHITE);

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
    UnloadTexture(cutsceneImage10);

}

// Inicializa o menu com música e texturas
void iniciarMenu(GameScreen *currentScreen) {
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
void atualizarMenu(GameScreen *currentScreen) {
    UpdateMusicStream(titleMusic);

    if (*currentScreen == TITLE) {
        backgroundPosX += 0.2f;
        if (backgroundPosX >= background.width) {
            backgroundPosX = 0;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            *currentScreen = GAME;
        } else if (IsKeyPressed(KEY_C)) {
            *currentScreen = OBJETIVO;  // Abre a tela do objetivo
        } else if (IsKeyPressed(KEY_R)) {
            *currentScreen = RANKINGS;  // Continua levando para a tela de rankings
        }
    } else if (*currentScreen == RANKINGS && IsKeyPressed(KEY_Q)) {
        *currentScreen = TITLE;
        ResumeMusicStream(titleMusic);
    }
}


void exibirObjetivo() {
    static bool objetivoLoaded = false;

    if (!objetivoLoaded) {
        objetivoImage = LoadTexture("static/image/objetivo.png");
        objetivoLoaded = true;
    }

    Rectangle source = { 0.0f, 0.0f, (float)objetivoImage.width, (float)objetivoImage.height }; // Área completa da imagem
    Rectangle dest = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }; // Tamanho da tela para ocupar 100%
    Vector2 origin = { 0.0f, 0.0f };

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexturePro(objetivoImage, source, dest, origin, 0.0f, WHITE); // Desenho com ajuste de tamanho
    EndDrawing();
}




// Desenha o menu principal com as opções de dificuldade e transição para o jogo
void desenharMenu(GameScreen currentScreen) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (currentScreen == TITLE) {
        desenharBackgroundComLogo();
        DrawText("Pressione ENTER para Jogar", SCREEN_WIDTH / 2 - 150, 400, 20, WHITE);
        DrawText("Pressione R para Rankings", SCREEN_WIDTH / 2 - 150, 650, 20, WHITE);
        DrawText("Pressione C para Controles", SCREEN_WIDTH / 2 - 150, 600, 20, WHITE);
    } 

    EndDrawing();
}

// Finaliza e libera os recursos do menu
void finalizarMenu() {
    UnloadTexture(background);
    UnloadTexture(logo);
    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    UnloadTexture(objetivoImage); // Libera a textura do objetivo
    CloseAudioDevice();
    CloseWindow();
}
