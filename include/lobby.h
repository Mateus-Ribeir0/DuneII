#ifndef LOBBY_H
#define LOBBY_H

#include "config.h"
#include "game.h"
void verificarProximidadePoco(float playerX, float playerY);
void tentarRecuperarAguaNoPoco();

// Declarações para zonas de colisão
extern Rectangle npcslobbyCollisionBox;  // Zona de colisão para o NPC 1
extern Rectangle npcslobby2CollisionBox; // Zona de colisão para o NPC 2

void iniciarLobby();
void finalizarLobby();
int isPlayerNearMerchant();
void DrawDialogBox(const char *text, int posX, int posY, int width, int height, Color boxColor, Color textColor, bool isPortalDialog);
void processarEntradaLobby(GameScreen *currentScreen);
void drawLobby();
void resetSpaceshipAnimation();
void updateAndDrawSpaceshipAnimation();
void processarTelaVazia();

#endif
