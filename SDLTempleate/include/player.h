#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 600

typedef enum{
    STATE_IDLE,
    STATE_WALK
} State;

typedef struct {
    // Posicion en x, y
    int x;
    int y;

    // velocidad a la que se mueve el personaje
    int speed;

    int velX;
    int velY;

    // Numero de sprite
    int frame;
    // Dirección del personaje
    int direction;

    State state;
    State previousState;

    SDL_RendererFlip flip;

    bool moving;

    unsigned int lastFrameTime;

    int score;
    int lives;

} Player; // El nuevo tipo de dato se llama Player

void Player_Init(Player *player, int posx, int posy);

void Player_Update(
    Player *player,
    const Uint8 *keyboard
);

void Player_Render(
    Player *player,
    SDL_Renderer *renderer,
    SDL_Texture *texture
);


static void Player_HandleInput(
    Player *player,
    const Uint8 *keyboard
);

static void Player_Move(Player *player);

static void Player_CheckCollisions(Player *player);

static void Player_UpdateState(Player *player);

static void Player_UpdateAnimation(Player *player);

#endif