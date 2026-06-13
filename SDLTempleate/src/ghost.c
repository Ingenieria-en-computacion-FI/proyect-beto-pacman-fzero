#include "ghost.h"
#include <stdlib.h>
#include <time.h>
#include "map.h"
#define TILE_SIZE 32

#define ROWS 21
#define COLS 21

static int CanMove(
    int x,
    int y,
    int map[][21])
{
    int left = x / TILE_SIZE;
    int right = (x + GHOST_WIDTH - 1) / TILE_SIZE;

    int top = y / TILE_SIZE;
    int bottom = (y + GHOST_HEIGHT - 1) / TILE_SIZE;

    if (left < 0 || right >= COLS)
        return 0;

    if (top < 0 || bottom >= ROWS)
        return 0;

    if (map[top][left] == 1)
        return 0;
    if (map[top][right] == 1)
        return 0;
    if (map[bottom][left] == 1)
        return 0;
    if (map[bottom][right] == 1)
        return 0;

    return 1;
}

void Ghost_SpawnRandom(Ghost *ghost)
{
    int row;
    int col;

    do
    {
        row = rand() % ROWS;
        col = rand() % COLS;

    } while (map[row][col] == 1);

    ghost->x = col * TILE_SIZE;
    ghost->y = row * TILE_SIZE;
}

void Ghost_Init(
    Ghost *ghost,
    int x,
    int y)
{
    ghost->x = x;
    ghost->y = y;

    ghost->dirX = 1;
    ghost->dirY = 0;

    ghost->speed = 1;

    ghost->frame = 0;
}

void Ghost_Update(
    Ghost *ghost,
    int playerX,
    int playerY,
    int map[][21])
{
    int dx = playerX - ghost->x;
    int dy = playerY - ghost->y;

    int distancia =
        abs(dx) + abs(dy);

    if (distancia < GHOST_VISION)
    {
        if (abs(dx) > abs(dy))
        {
            ghost->dirX = (dx > 0) ? 1 : -1;
            ghost->dirY = 0;
        }
        else
        {
            ghost->dirX = 0;
            ghost->dirY = (dy > 0) ? 1 : -1;
        }
    }
    else
    {
        /* 5% de probabilidad de cambiar dirección */

        if (rand() % 100 < 5)
        {
            int dirs[4][2] =
                {
                    {1, 0},
                    {-1, 0},
                    {0, 1},
                    {0, -1}};

            int validos[4];
            int cantidad = 0;

            for (int i = 0; i < 4; i++)
            {
                int tx =
                    ghost->x +
                    dirs[i][0] * ghost->speed;

                int ty =
                    ghost->y +
                    dirs[i][1] * ghost->speed;

                if (CanMove(tx, ty, map))
                {
                    validos[cantidad++] = i;
                }
            }

            if (cantidad > 0)
            {
                int elegir =
                    validos[rand() % cantidad];

                ghost->dirX =
                    dirs[elegir][0];

                ghost->dirY =
                    dirs[elegir][1];
            }
        }
    }

    int nextX =
        ghost->x +
        ghost->dirX *
            ghost->speed;

    int nextY =
        ghost->y +
        ghost->dirY *
            ghost->speed;

    /*
        Si hay pared,
        busca otra dirección.
    */

    if (!CanMove(nextX, nextY, map))
    {
        int dirs[4][2] =
            {
                {1, 0},
                {-1, 0},
                {0, 1},
                {0, -1}};

        int validos[4];

        int cantidad = 0;

        for (int i = 0; i < 4; i++)
        {
            int tx =
                ghost->x +
                dirs[i][0] *
                    ghost->speed;

            int ty =
                ghost->y +
                dirs[i][1] *
                    ghost->speed;

            if (CanMove(tx, ty, map))
            {
                validos[cantidad] = i;
                cantidad++;
            }
        }

        if (cantidad > 0)
        {
            int elegir =
                validos[rand() % cantidad];

            ghost->dirX =
                dirs[elegir][0];

            ghost->dirY =
                dirs[elegir][1];

            nextX =
                ghost->x +
                ghost->dirX *
                    ghost->speed;

            nextY =
                ghost->y +
                ghost->dirY *
                    ghost->speed;
        }
    }

    if (CanMove(nextX, ghost->y, map))
    {
        ghost->x = nextX;
    }

    if (CanMove(ghost->x, nextY, map))
    {
        ghost->y = nextY;
    }
}

int Ghost_CollidesPlayer(
    Ghost *ghost,
    Player *player)
{
    SDL_Rect g =
        {
            ghost->x,
            ghost->y,
            GHOST_WIDTH,
            GHOST_HEIGHT};

    SDL_Rect p =
        {
            player->x,
            player->y,
            20,
            28};

    return SDL_HasIntersection(&g, &p);
}

void Ghost_Render(
    Ghost *ghost,
    SDL_Renderer *renderer,
    SDL_Texture *texture)
{
    SDL_Rect dst =
        {
            ghost->x,
            ghost->y,
            GHOST_WIDTH,
            GHOST_HEIGHT};

    SDL_SetRenderDrawColor(
        renderer,
        255,
        0,
        0,
        255);

    SDL_RenderFillRect(
        renderer,
        &dst);

    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &dst);
}