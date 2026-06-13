#ifndef GHOST_H
#define GHOST_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include <SDL2/SDL_ttf.h>
#define GHOST_WIDTH 20
#define GHOST_HEIGHT 28
#define GHOST_VISION 160

typedef struct
{
    int x;
    int y;

    int dirX;
    int dirY;

    int speed;

    int frame;

} Ghost;

void Ghost_Init(
    Ghost *ghost,
    int x,
    int y
);

void Ghost_Update(
    Ghost *ghost,
    int playerX,
    int playerY,
    int map[][21]
);

void Ghost_Render(
    Ghost *ghost,
    SDL_Renderer *renderer,
    SDL_Texture *texture
);

int Ghost_CollidesPlayer(
    Ghost *ghost,
    Player *player
);

void Ghost_SpawnRandom(Ghost *ghost);
#endif
