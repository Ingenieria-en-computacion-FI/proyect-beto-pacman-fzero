#include "player.h"
#include "map.h"
#define w
#define a
#define s
#define d

SDL_Rect walkDown[] = {
    {0, 12, 18, 37},
    {22, 11, 19, 38},
    {45, 11, 18, 38},
};

SDL_Rect walkHorizontal[] = {
    {1, 55, 18, 37},
    {22, 56, 17, 36},
    {43, 55, 18, 37},
    {65, 56, 18, 36},
    {88, 56, 18, 36},
};

SDL_Rect walkUp[] = {
    {1, 99, 19, 36},
    {24, 99, 19, 36},
    {47, 98, 21, 37},
    {72, 98, 21, 37},
};

SDL_Rect idle[] = {
    {68, 13, 22, 36},
    {94, 11, 22, 36},
};

void Player_Init(Player *player, int posx, int posy)
{

    player->x = posx;
    player->y = posy;

    player->velX = 0;
    player->velY = 0;

    player->speed = 2;

    player->frame = 0;
    player->direction = 2;

    player->moving = false;

    player->state = STATE_IDLE;
    player->previousState = STATE_IDLE;

    player->flip = SDL_FLIP_NONE;

    player->lastFrameTime = 0;

    player->score = 0;
    player->lives = 3;
}

void Player_Update(Player *player, const Uint8 *keyboard)
{
    Player_HandleInput(player, keyboard);

    Player_Move(player);

    Player_CheckCollisions(player);

    Player_UpdateState(player);

    Player_UpdateAnimation(player);
}

// ======================================================
// INPUT
// ======================================================
static void Player_HandleInput(Player *player, const unsigned char *keyboard)
{
    player->velX = 0;
    player->velY = 0;

    player->moving = false;

    // DERECHA
    if (keyboard[SDL_SCANCODE_RIGHT])
    {

        player->velX = player->speed;

        player->flip = SDL_FLIP_HORIZONTAL;

        player->direction = 0;

        player->moving = true;
    }

    // IZQUIERDA
    if (keyboard[SDL_SCANCODE_LEFT])
    {

        player->velX = -player->speed;

        player->flip = SDL_FLIP_NONE;

        player->direction = 0;

        player->moving = true;
    }

    // ARRIBA
    if (keyboard[SDL_SCANCODE_UP])
    {

        player->velY = -player->speed;

        player->direction = 1;

        player->moving = true;
    }

    // ABAJO
    if (keyboard[SDL_SCANCODE_DOWN])
    {

        player->velY = player->speed;

        player->direction = 2;

        player->moving = true;
    }
}

// ======================================================
// MOVIMIENTO
// ======================================================

static void Player_Move(Player *player)
{
    int nextX = player->x + player->velX;
    int nextY = player->y + player->velY;

    if (PuedeMover(nextX, player->y, map))
    {
        player->x = nextX;
    }

    if (PuedeMover(player->x, nextY, map))
    {
        player->y = nextY;
    }

    /* --------- RECOGER PELLETS --------- */

    int centroX = (player->x + 10) / TILE_SIZE;
    int centroY = (player->y + 13) / TILE_SIZE;

    if (map[centroY][centroX] == 2)
    {
        map[centroY][centroX] = 0;

        /* Si tienes score:
           player->score += 10;
        */
        player->score += 10;
    }
}

// ======================================================
// COLISIONES
// ======================================================

static void Player_CheckCollisions(Player *player)
{
    if (player->x < 0)
        player->x = 0;

    if (player->y < 0)
        player->y = 0;

    if (player->x > COLS * TILE_SIZE - 20)
        player->x = COLS * TILE_SIZE - 20;

    if (player->y > ROWS * TILE_SIZE - 28)
        player->y = ROWS * TILE_SIZE - 28;
}

// ======================================================
// ESTADOS
// ======================================================

static void Player_UpdateState(Player *player)
{
    if (player->moving)
    {
        player->state = STATE_WALK;
    }
    else
    {
        player->state = STATE_IDLE;
    }

    // SI CAMBIÓ EL ESTADO
    if (player->state != player->previousState)
    {

        player->frame = 0;

        player->previousState = player->state;
    }
}

// ======================================================
// ANIMACIONES
// ======================================================

static void Player_UpdateAnimation(Player *player)
{
    int animationSpeed;

    switch (player->state)
    {

    case STATE_IDLE:
        animationSpeed = 500;
        break;

    case STATE_WALK:
        animationSpeed = 150;
        break;

    default:
        animationSpeed = 150;
        break;
    }

    unsigned int currentTime = SDL_GetTicks();

    if (currentTime > player->lastFrameTime + animationSpeed)
    {

        player->frame++;

        player->lastFrameTime = currentTime;
    }
}
void Player_Render(Player *player, SDL_Renderer *renderer, SDL_Texture *texture)
{

    SDL_Rect src;

    switch (player->state)
    {
    case STATE_IDLE:
        src = idle[player->frame % 2];
        break;

    case STATE_WALK:
        switch (player->direction)
        {
        case 0:
            src = walkHorizontal[player->frame % 5];
            break;

        case 1:
            src = walkUp[player->frame % 4];
            break;

        case 2:
            src = walkDown[player->frame % 3];
            break;
        default:
            src = idle[0];
            break;
        }
    }

    SDL_Rect dest = {
        player->x,
        player->y,
        20,
        28};

    SDL_RenderCopyEx(
        renderer,
        texture,
        &src,
        &dest,
        0,
        NULL,
        player->flip);
}