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
    } else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) ||
            IsKeyPressed(KEY_D) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_E)) {
        *usandoControle = false;
    }
}

void DrawDialogBoxWithButtons(const char *text, int posX, int posY, int width, int height, Color boxColor, Color textColor, bool usandoControle, Texture2D texture, Rectangle *botaoAreas) {
    // Desenhar a caixa de diálogo
    DrawRectangleRounded((Rectangle){ posX, posY, width, height }, 0.1f, 16, boxColor);

    // Configurações de texto
    int cursorX = posX + 20; // Margem interna da caixa
    int cursorY = posY + 20;
    int fontSize = 20;

    // Fator de escala para os botões (25% do tamanho original)
    float scale = 1.50f;

    // Processar o texto para renderizar botões e texto
    const char *ptr = text;
    while (*ptr != '\0') {
        if (*ptr == '\n') { // Detectar quebra de linha
            cursorX = posX + 20; // Resetar posição horizontal
            cursorY += fontSize + 4; // Avançar para a próxima linha
        } else if (*ptr == '[') { // Detectar marcador de botão ou texto especial
            ptr++;
            int botaoIndex = -1;

            // Identificar o marcador correspondente
            if (*ptr == 'Y') botaoIndex = 0;
            else if (*ptr == 'A') botaoIndex = 1;
            else if (*ptr == 'B') botaoIndex = 2;
            else if (*ptr == 'X') botaoIndex = 3;

            if (usandoControle && botaoIndex >= 0) {
                // Calcula tamanho escalado do botão
                float scaledWidth = botaoAreas[botaoIndex].width * scale;
                float scaledHeight = botaoAreas[botaoIndex].height * scale;

                // Calcula o deslocamento vertical para centralizar o botão com o texto
                float textHeight = (float)fontSize; // Altura do texto
                float verticalOffset = (textHeight - scaledHeight) / 2.0f;

                // Verifica quebra de linha para o botão
                if (cursorX + scaledWidth > posX + width - 20) {
                    cursorX = posX + 20;
                    cursorY += fontSize + 2; // Avançar para a próxima linha
                }

                // Desenha o botão escalado com ajuste vertical
                DrawTexturePro(
                    texture,
                    botaoAreas[botaoIndex],
                    (Rectangle){cursorX, cursorY + verticalOffset, scaledWidth, scaledHeight},
                    (Vector2){0, 0}, // Sem rotação
                    0.0f,
                    WHITE
                );

                // Avança o cursor com o tamanho do botão escalado
                cursorX += scaledWidth + 8; // Espaçamento após o botão
            } else {
                // Renderizar o marcador (ex: [P]) como texto
                char marcador[4] = { '[', *ptr, ']', '\0' };
                DrawText(marcador, cursorX, cursorY, fontSize, textColor);
                cursorX += MeasureText(marcador, fontSize) + 2;
            }

            // Avançar ponteiro até o fechamento do colchete ']'
            while (*ptr != ']' && *ptr != '\0') ptr++;
        } else {
            // Verifica quebra de linha para texto
            int charWidth = MeasureText((char[2]){*ptr, '\0'}, fontSize);
            if (cursorX + charWidth > posX + width - 20) {
                cursorX = posX + 20;
                cursorY += fontSize + 4; // Avançar para a próxima linha
            }

            // Desenhar caractere normal
            char caractere[2] = {*ptr, '\0'};
            DrawText(caractere, cursorX, cursorY, fontSize, textColor);
            cursorX += charWidth + 2; // Avança o cursor com base no caractere
        }

        ptr++;
    }
}

void DrawMenuOptionsWithButtons(const char *text, int posX, int posY, int fontSize, float buttonScale, Color textColor, bool usandoControle, Texture2D texture, Rectangle *botaoAreas) {
    int cursorX = posX; // Posição inicial do texto
    int cursorY = posY;

    const char *ptr = text;
    while (*ptr != '\0') {
        if (*ptr == '[') { // Detectar marcador de botão
            ptr++;
            int botaoIndex = -1;

            // Identificar o botão correspondente
            if (*ptr == 'Y') botaoIndex = 0;
            else if (*ptr == 'A') botaoIndex = 1;
            else if (*ptr == 'B') botaoIndex = 2;
            else if (*ptr == 'X') botaoIndex = 3;
            else if (*ptr == 'K') botaoIndex = 4;
            else if (*ptr == 'L') botaoIndex = 5;

            if (usandoControle && botaoIndex >= 0) {
                // Calcula tamanho escalado do botão
                float scaledWidth = botaoAreas[botaoIndex].width * buttonScale;
                float scaledHeight = botaoAreas[botaoIndex].height * buttonScale;

                // Calcula o deslocamento vertical para centralizar o botão com o texto
                float textHeight = (float)fontSize;
                float verticalOffset = (textHeight - scaledHeight) / 2.0f;

                // Desenha o botão escalado
                DrawTexturePro(
                    texture,
                    botaoAreas[botaoIndex],
                    (Rectangle){cursorX, cursorY + verticalOffset, scaledWidth, scaledHeight},
                    (Vector2){0, 0}, // Sem rotação
                    0.0f,
                    WHITE
                );

                // Avança o cursor com o tamanho do botão escalado
                cursorX += scaledWidth + 2; // Espaçamento após o botão
            }

            // Avançar ponteiro até o fechamento do colchete ']'
            while (*ptr != ']' && *ptr != '\0') ptr++;
        } else {
            // Desenhar caractere normal
            char caractere[2] = {*ptr, '\0'};
            DrawText(caractere, cursorX, cursorY, fontSize, textColor);
            cursorX += MeasureText(caractere, fontSize) + 3; // Avança o cursor com base no caractere
        }

        ptr++;
    }
}