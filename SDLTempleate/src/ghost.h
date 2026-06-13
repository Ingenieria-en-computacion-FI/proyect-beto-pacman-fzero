#ifndef GHOST_H
#define GHOST_H

#include <SDL2/SDL.h>
#include "player.h"

typedef struct
{
    float x;
    float y;

    int dirX;
    int dirY;

    float speed;

    SDL_Texture* texture;

} Ghost;

void initGhost(Ghost* g,
               SDL_Texture* texture,
               int x,
               int y);

void updateGhost(Ghost* g,
                 Player* p,
                 int map[21][21]);

void drawGhost(Ghost* g,
               SDL_Renderer* renderer);

#endif