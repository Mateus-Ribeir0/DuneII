#include "config.h"

int player_x = MAPA_LARGURA / 2;
int player_y = MAPA_ALTURA / 2;
int mapaAtual = 0;
int itemsCollected = 0;
float playerWater = 100.0;
float playerLucky = 5.0;
double lastWaterUpdateTime = 0.0;
char playerName[MAX_NAME_LENGTH] = "";
bool usandoControle = true;
extern bool telaVaziaBloqueada = true;
bool spaceshipAnimationPlayed = false;

void comandoJogador(bool *usandoControle) {
    if (IsGamepadAvailable(0) && (
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP))) {
        *usandoControle = true;
    } else if (GetKeyPressed() != 0) {
        *usandoControle = false;
    }
}

void DrawDialogBoxWithButtons(const char *text, int posX, int posY, int width, int height, Color boxColor, Color textColor, bool usandoControle, Texture2D texture, Rectangle *botaoAreas, float scale) {
    DrawRectangleRounded((Rectangle){ posX, posY, width, height }, 0.1f, 16, boxColor);

    int cursorX = posX + 10;
    int cursorY = posY + 10;
    int fontSize = 20;

    const char *ptr = text;
    while (*ptr != '\0') {
        if (*ptr == '\n') { 
            cursorX = posX + 10; 
            cursorY += fontSize + 2; 
        } else if (*ptr == '[') { 
            ptr++;
            int botaoIndex = -1;

            if (*ptr == 'Y') botaoIndex = 0;
            else if (*ptr == 'A') botaoIndex = 1;
            else if (*ptr == 'B') botaoIndex = 2;
            else if (*ptr == 'X') botaoIndex = 3;

            if (usandoControle && botaoIndex >= 0) {
                float scaledWidth = botaoAreas[botaoIndex].width * scale;
                float scaledHeight = botaoAreas[botaoIndex].height * scale;

                float textHeight = (float)fontSize;
                float verticalOffset = (textHeight - scaledHeight) / 2.0f;

                if (cursorX + scaledWidth > posX + width - 10) {
                    cursorX = posX + 10;
                    cursorY += fontSize + 2;
                }

                DrawTexturePro(
                    texture,
                    botaoAreas[botaoIndex],
                    (Rectangle){cursorX, cursorY + verticalOffset, scaledWidth, scaledHeight},
                    (Vector2){0, 0},
                    0.0f,
                    WHITE
                );

                cursorX += scaledWidth + 8;
            } else {
                char marcador[4] = { '[', *ptr, ']', '\0' };
                DrawText(marcador, cursorX, cursorY, fontSize, textColor);
                cursorX += MeasureText(marcador, fontSize) + 2;
            }

            while (*ptr != ']' && *ptr != '\0') ptr++;
        } else {
            int charWidth = MeasureText((char[2]){*ptr, '\0'}, fontSize);
            if (cursorX + charWidth > posX + width - 10) {
                cursorX = posX + 10;
                cursorY += fontSize + 2;
            }

            char caractere[2] = {*ptr, '\0'};
            DrawText(caractere, cursorX, cursorY, fontSize, textColor);
            cursorX += charWidth + 2;
        }

        ptr++;
    }
}

void DrawMenuOptionsWithButtons(const char *text, int posX, int posY, int fontSize, float buttonScale, Color textColor, bool usandoControle, Texture2D texture, Rectangle *botaoAreas) {
    int cursorX = posX;
    int cursorY = posY;

    const char *ptr = text;
    while (*ptr != '\0') {
        if (*ptr == '\n') {
            cursorX = posX;
            cursorY += fontSize + 5;
        } else if (*ptr == '[') {
            ptr++;
            int botaoIndex = -1;

            if (*ptr == 'Y') botaoIndex = 0;
            else if (*ptr == 'A') botaoIndex = 1;
            else if (*ptr == 'B') botaoIndex = 2;
            else if (*ptr == 'X') botaoIndex = 3;
            else if (*ptr == 'H') botaoIndex = 4;
            else if (*ptr == 'L') botaoIndex = 5;

            if (usandoControle && botaoIndex >= 0) {
                float scaledWidth = botaoAreas[botaoIndex].width * buttonScale;
                float scaledHeight = botaoAreas[botaoIndex].height * buttonScale;

                float textHeight = (float)fontSize;
                float verticalOffset = (textHeight - scaledHeight) / 2.0f;

                DrawTexturePro(
                    texture,
                    botaoAreas[botaoIndex],
                    (Rectangle){cursorX, cursorY + verticalOffset, scaledWidth, scaledHeight},
                    (Vector2){0, 0},
                    0.0f,
                    WHITE
                );

                cursorX += scaledWidth + 2;
            }

            while (*ptr != ']' && *ptr != '\0') ptr++;
        } else {
            char caractere[2] = {*ptr, '\0'};
            DrawText(caractere, cursorX, cursorY, fontSize, textColor);
            cursorX += MeasureText(caractere, fontSize) + 3;
        }

        ptr++;
    }
}