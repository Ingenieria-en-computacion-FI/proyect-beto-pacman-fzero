#ifndef __PLAYER_H__
#define __PLAYER_H__
#ifndef __ENEMIGOS_H__
#define __ENEMIGOS_H__

#include <stdbool.h>

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


void Player_HandleInput(
    Player *player,
    const Uint8 *keyboard
);

void Player_Move(Player *player);

void Player_CheckCollisions(Player *player);

void Player_UpdateState(Player *player);

void Player_UpdateAnimation(Player *player);

#endif
#endif