#include "menu.h"

static Texture2D background, logo, objetivoImage;
static Music titleMusic;
static Sound swordsfx;
static Sound rankingssfx;
static Sound controlssfx;
static float backgroundPosX;

void showFadingImage(Texture2D image, float duration) {
    float startTime = GetTime();
    float opacity = 1.0f;

    while (opacity > 0.0f && !WindowShouldClose()) {
        float elapsedTime = GetTime() - startTime;
        if (elapsedTime >= duration) {
            opacity = 0.0f;
        } else {
            opacity = 1.0f - (elapsedTime / duration);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        Rectangle destRect = { 
            GetScreenWidth() / 2 - image.width / 2, 
            GetScreenHeight() / 2 - image.height / 2, 
            image.width, 
            image.height 
        };

        DrawTexturePro(image, (Rectangle){0, 0, image.width, image.height}, destRect, (Vector2){0, 0}, 0.0f, Fade(WHITE, opacity));
        EndDrawing();
    }
}

void recebeNomeDoPlayer(GameScreen *currentScreen) {
    static char nameBuffer[MAX_NAME_LENGTH] = "";
    int letra;

    memset(nameBuffer, 0, sizeof(nameBuffer));

    while (!WindowShouldClose()) {
        
        UpdateMusicStream(titleMusic);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        desenharBackgroundComLogo();

        DrawText("Digite seu nome:", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 50, 25, WHITE);
        DrawRectangle(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 100, 350, 30, WHITE);
        DrawText(nameBuffer, SCREEN_WIDTH / 2 - 195, SCREEN_HEIGHT / 2 + 105, 25, BLACK);
        DrawText("Pressione [ENTER] para continuar", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 150, 25, WHITE);

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

void displayCutscene(Texture2D image, const char* text, Music titleMusic, float speed, float scale, float duration) {
    int charactersToShow = 0;
    int frameCount = 0;
    float scrollX = 0.0f;
    float timer = 0.0f;

    while (timer < duration && !IsKeyPressed(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        timer += GetFrameTime();
        UpdateMusicStream(titleMusic);

        scrollX -= speed * GetFrameTime();
        if (scrollX <= -image.width * scale) {
            scrollX = 0.0f;
        }

        DrawTextureEx(image, (Vector2){scrollX, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(image, (Vector2){scrollX + image.width * scale, 0}, 0.0f, scale, WHITE);

        if (charactersToShow < strlen(text)) {
            frameCount++;
            charactersToShow = (frameCount / (duration * 20)) * strlen(text);
            if (charactersToShow > strlen(text)) charactersToShow = strlen(text);
        }

        int dialogWidth = 600;
        int dialogHeight = 100;
        int posX = GetScreenWidth() / 2 - dialogWidth / 2;
        int posY = GetScreenHeight() - dialogHeight - 20;

        DrawRectangleRounded((Rectangle){posX, posY, dialogWidth, dialogHeight}, 0.1f, 16, WHITE);
        DrawText(TextSubtext(text, 0, charactersToShow), posX + 10, posY + 10, 20, BLACK);

        EndDrawing();
    }
}

void cutsceneArrakis() {

    const char* text1 = "Arrakis. Um vasto deserto, onde a areia carrega\ncicatrizes das guerras travadas pela raras especiarias.";
    const char* text2 = "E eu vim em busca desse raro tesouro, mas cada\npasso é uma batalha. As especiarias não são fáceis\nde conquistar.";
    const char* text3 = "Elas nascem nas profundezas traiçoeiras de \nArrakis, onde os vermes gigantes da areia,estão sempre\na espreita e emergem ao menor sinal de vibração.";
    const char* text4 = "Eles são monstros antigos, vastos e indomáveis,\ncapazes de engolir tudo que cruza seu caminho.";
    const char* text5 = "Aqui, cada grão de especiaria custa suor, água, sangue\ne, muitas vezes, vidas.";
    const char* text6 = "No calor abrasador e sob o constante risco de ser\ndetectado por essas criaturas, eu preciso seguir\nem frente.";
    const char* text7 = "Cada grão de especiaria que eu consigo arrancar\nda areia é uma pequena vitória, mas essencial para\nmim e minha família.";
    const char* text8 = "Minha missão é sobreviver, extrair o que posso e sair\nde Arrakis com o que é preciso para mudar o\ndestino de minha casa.";
    const char* text9 = "Mas neste deserto, cada erro é fatal, e os vermes\nestão sempre à espreita.";
    char text10[128];
    sprintf(text10, "Meu nome é %s e a minha jornada começa AGORA", playerName);

    float speed = 30.0f; 
    float scale = 0.6f;
    float duration = 7.0f;

    Texture2D cutsceneImage1 = LoadTexture("static/image/cutscene2.png");
    displayCutscene(cutsceneImage1, text1, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage1);

    Texture2D cutsceneImage2 = LoadTexture("static/image/cutscene1.png");
    displayCutscene(cutsceneImage2, text2, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage2);

    Texture2D cutsceneImage3 = LoadTexture("static/image/cutscene3.png");
    displayCutscene(cutsceneImage3, text3, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage3);

    Texture2D cutsceneImage4 = LoadTexture("static/image/cutscene4.png");
    displayCutscene(cutsceneImage4, text4, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage4);

    Texture2D cutsceneImage5 = LoadTexture("static/image/cutscene5.png");
    displayCutscene(cutsceneImage5, text5, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage5);

    Texture2D cutsceneImage6 = LoadTexture("static/image/cutscene6.png");
    displayCutscene(cutsceneImage6, text6, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage6);

    Texture2D cutsceneImage7 = LoadTexture("static/image/cutscene7.png");
    displayCutscene(cutsceneImage7, text7, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage7);

    Texture2D cutsceneImage8 = LoadTexture("static/image/cutscene8.png");
    displayCutscene(cutsceneImage8, text8, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage8);

    Texture2D cutsceneImage9 = LoadTexture("static/image/cutscene9.png");
    displayCutscene(cutsceneImage9, text9, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage9);

    Texture2D cutsceneImage10 = LoadTexture("static/image/cutscene10.png");
    displayCutscene(cutsceneImage10, text10, titleMusic, speed, scale, duration);
    UnloadTexture(cutsceneImage10);
}

void iniciarMenu(GameScreen *currentScreen) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DuneII");

    background = LoadTexture("static/image/fundo2.png");
    logo = LoadTexture("static/image/logonova.png");
    titleMusic = LoadMusicStream("static/music/epicversion3.mp3");

    controlssfx = LoadSound("static/music/controls.wav");
    rankingssfx = LoadSound("static/music/rankings.wav");
    SetSoundVolume(controlssfx, 0.6f);
    SetSoundVolume(rankingssfx, 0.6f);

    PlayMusicStream(titleMusic);
    SetTargetFPS(60);
    Texture2D introImage = LoadTexture("static/image/intro.png");
    showFadingImage(introImage, 0.1f);
    sleep(1);
    Texture2D introImage2 = LoadTexture("static/image/intro2.png");
    showFadingImage(introImage2, 0.1f);
    sleep(1);
    UnloadTexture(introImage);
    *currentScreen = TITLE;
}

void atualizarMenu(GameScreen *currentScreen) {
    UpdateMusicStream(titleMusic);

    if (*currentScreen == TITLE) {
        backgroundPosX += 0.2f;
        if (backgroundPosX >= background.width) {
            backgroundPosX = 0;
        }       
        if (IsKeyPressed(KEY_ENTER)) {
            PlaySound(swordsfx);
            *currentScreen = GAME;
        } else if (IsKeyPressed(KEY_C)) {
            PlaySound(controlssfx);
            *currentScreen = OBJETIVO;  
        } else if (IsKeyPressed(KEY_R)) {
            PlaySound(rankingssfx);
            *currentScreen = RANKINGS;  
        }
    } else if (*currentScreen == RANKINGS && IsKeyPressed(KEY_Q)) {
        *currentScreen = TITLE;
        ResumeMusicStream(titleMusic);
    }
}

void exibirObjetivo(GameScreen *currentScreen) {
    objetivoImage = LoadTexture("static/image/objetivo.png");

    while (!WindowShouldClose()) {
        UpdateMusicStream(titleMusic);

        Rectangle source = { 0.0f, 0.0f, (float)objetivoImage.width, (float)objetivoImage.height };
        Rectangle dest = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
        Vector2 origin = { 0.0f, 0.0f };

        BeginDrawing();
        DrawTexturePro(objetivoImage, source, dest, origin, 0.0f, WHITE);
        EndDrawing();

        if (IsKeyPressed(KEY_Q)) {
            *currentScreen = TITLE;
            break;
        }
    }
}

void drawMenu() {
    int mainTextY = 400;
    int optionsY = mainTextY + 100;
    int spacingX = 50;
    int textSize = 25;
    int centerX = SCREEN_WIDTH / 2;

    BeginDrawing();
    desenharBackgroundComLogo();
    DrawText("Pressione [ENTER] para Jogar", SCREEN_WIDTH / 2 - MeasureText("Pressione [ENTER] para Jogar", 30) / 2, mainTextY, 30, WHITE);
    DrawText("[R] Rankings", centerX - MeasureText("[R] Rankings", textSize) - spacingX, optionsY, textSize, WHITE);
    DrawText("[C] Controles", centerX + spacingX, optionsY, textSize, WHITE);
    EndDrawing();
}

void finalizarMenu() {
    UnloadTexture(background);
    UnloadTexture(logo);
    StopMusicStream(titleMusic);
    UnloadMusicStream(titleMusic);
    UnloadTexture(objetivoImage); 
    CloseWindow();
}
