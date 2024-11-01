/**
 * screen.c
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include "screen.h"

void obterTamanhoTerminal(int *width, int *height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col / 2;   // Divisão por 2 para compatibilidade com o espaçamento de 2 caracteres
    *height = w.ws_row - 2;   // Evita a última linha para posicionamento seguro do cursor
}

void screenDrawBorders() 
{
    char hbc = BOX_HLINE;
    char vbc = BOX_VLINE;
    
    screenClear();
    screenBoxEnable();
    
    int maxWidth, maxHeight;
    obterTamanhoTerminal(&maxWidth, &maxHeight);

    screenGotoxy(MINX, MINY);
    printf("%c", BOX_UPLEFT);

    for (int i=MINX+1; i<maxWidth; i++)
    {
        screenGotoxy(i, MINY);
        printf("%c", hbc);
    }
    screenGotoxy(maxWidth, MINY);
    printf("%c", BOX_UPRIGHT);

    for (int i=MINY+1; i<maxHeight; i++)
    {
        screenGotoxy(MINX, i);
        printf("%c", vbc);
        screenGotoxy(maxWidth, i);
        printf("%c", vbc);
    }

    screenGotoxy(MINX, maxHeight);
    printf("%c", BOX_DWNLEFT);
    for (int i=MINX+1; i<maxWidth; i++)
    {
        screenGotoxy(i, maxHeight);
        printf("%c", hbc);
    }
    screenGotoxy(maxWidth, maxHeight);
    printf("%c", BOX_DWNRIGHT);

    screenBoxDisable();
    
}

void screenInit(int drawBorders)
{
    screenClear();
    if (drawBorders) screenDrawBorders();
    screenHomeCursor();
    screenHideCursor();
}

void screenDestroy()
{
    printf("%s[0;39;49m", ESC); // Reset colors
    screenSetNormal();
    screenClear();
    screenHomeCursor();
    screenShowCursor();
}

void screenGotoxy(int x, int y)
{
    int maxWidth, maxHeight;
    obterTamanhoTerminal(&maxWidth, &maxHeight);
    x = ( x<0 ? 0 : x>=maxWidth ? maxWidth-1 : x);
    y = ( y<0 ? 0 : y>maxHeight ? maxHeight : y);
    
    printf("%s[f%s[%dB%s[%dC", ESC, ESC, y, ESC, x);
}


