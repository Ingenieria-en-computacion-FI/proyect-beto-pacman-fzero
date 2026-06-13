#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_mixer.h>

#include "player.h"
#include "ghost.h"
#include "map.h"
#define NUM_GHOSTS 7

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    if (Mix_OpenAudio(
            44100,
            MIX_DEFAULT_FORMAT,
            2,
            2048) < 0)
    {
        printf(
            "Error SDL_mixer: %s\n",
            Mix_GetError());

        return 1;
    }
    srand(time(NULL));

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);

    if (font == NULL)
    {
        printf("Error cargando fuente: %s\n", TTF_GetError());
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);

    SDL_DisplayMode dm;

    SDL_GetCurrentDisplayMode(0, &dm);

    int windowWidth = dm.w - 100;
    int windowHeight = dm.h - 100;

    SDL_Window *window = SDL_CreateWindow(
        "Juego",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        0);

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);

    SDL_Texture *texture = IMG_LoadTexture(
        renderer,
        "moon.png");
    Player player;

    Mix_Music *music =
        Mix_LoadMUS(
            "F-Zero GP Legend - Mist Flow.mp3");
    Mix_PlayMusic(music, -1);

    SDL_Texture *ghostTexture =
        IMG_LoadTexture(
            renderer,
            "Sonic SPRITE.png");
    bool running = true;

    Player_Init(
        &player,
        (COLS * TILE_SIZE) / 2 - 10,
        (ROWS * TILE_SIZE) / 2 - 14);

    Ghost ghosts[NUM_GHOSTS];

    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        Ghost_Init(&ghosts[i], 0, 0);
        Ghost_SpawnRandom(&ghosts[i]);
    }

    SDL_Event event;

    int gameOver = 0;
    int victory = 0;

    Mix_Music *gameOverMusic =
        Mix_LoadMUS("Total Distortion You Are Dead.mp3");

    if (gameOverMusic == NULL)
    {
        printf(
            "Error cargando gameover.mp3: %s\n",
            Mix_GetError());
    }

    Mix_Music *victoryMusic =
        Mix_LoadMUS(
           "F Zero X OST   Grandprix Ending.mp3");

    if (victoryMusic == NULL)
    {
        printf(
            "Error cargando victory.mp3: %s\n",
            Mix_GetError());
    }

    while (running)
    {

        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        const unsigned char *keyboard = SDL_GetKeyboardState(NULL);
        if (!gameOver && !victory)
        {
            Player_Update(
                &player,
                keyboard);

            for (int i = 0; i < NUM_GHOSTS; i++)
            {
                Ghost_Update(
                    &ghosts[i],
                    player.x,
                    player.y,
                    map);
            }

            if (!victory &&
                CountPellets() == 0)
            {
                victory = 1;

                Mix_HaltMusic();

                if (victoryMusic != NULL)
                {
                    Mix_PlayMusic(
                        victoryMusic,
                        0);
                }
            }
        }

        for (int i = 0; i < NUM_GHOSTS; i++)
        {
            if (Ghost_CollidesPlayer(&ghosts[i], &player))
            {
                player.lives--;

                if (player.lives <= 0)
                {
                    gameOver = 1;

                    Mix_HaltMusic();

                    if (gameOverMusic != NULL)
                    {
                        Mix_PlayMusic(
                            gameOverMusic,
                            0);
                    }
                }

                player.x =
                    (COLS * TILE_SIZE) / 2 - 10;

                player.y =
                    (ROWS * TILE_SIZE) / 2 - 14;

                for (int j = 0; j < NUM_GHOSTS; j++)
                {
                    Ghost_SpawnRandom(&ghosts[j]);
                }

                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);
        SDL_Rect tile;

        for (int y = 0; y < ROWS; y++)
        {
            for (int x = 0; x < COLS; x++)
            {
                tile.x = x * TILE_SIZE;
                tile.y = y * TILE_SIZE;
                tile.w = TILE_SIZE;
                tile.h = TILE_SIZE;

                switch (map[y][x])
                {
                case 1: // Pared

                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    SDL_RenderFillRect(renderer, &tile);
                    break;

                case 2: // Moneda

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tile);

                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

                    SDL_Rect coin =
                        {
                            tile.x + TILE_SIZE / 2 - 4,
                            tile.y + TILE_SIZE / 2 - 4,
                            8,
                            8};

                    SDL_RenderFillRect(renderer, &coin);
                    break;

                default:

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tile);
                    break;
                }
            }
        }

        Player_Render(&player, renderer, texture);

        for (int i = 0; i < NUM_GHOSTS; i++)
        {
            Ghost_Render(
                &ghosts[i],
                renderer,
                ghostTexture);
        }

        char textoHUD[100];

        sprintf(
            textoHUD,
            "Puntaje: %d   Vidas: %d",
            player.score,
            player.lives);

        SDL_Color blanco = {255, 255, 255, 255};

        SDL_Surface *surfaceTexto =
            TTF_RenderText_Solid(font, textoHUD, blanco);

        SDL_Texture *textoTexture =
            SDL_CreateTextureFromSurface(
                renderer,
                surfaceTexto);

        SDL_Rect textoRect = {
            10,
            10,
            surfaceTexto->w,
            surfaceTexto->h};

        SDL_RenderCopy(
            renderer,
            textoTexture,
            NULL,
            &textoRect);

        SDL_FreeSurface(surfaceTexto);
        SDL_DestroyTexture(textoTexture);

        if (gameOver)
        {
            SDL_Color rojo =
                {
                    255,
                    0,
                    0,
                    255};

            char texto[128];

            sprintf(
                texto,
                "GAME OVER - PUNTOS: %d",
                player.score);

            SDL_Surface *surface2 =
                TTF_RenderText_Solid(
                    font,
                    "Cierra la ventana para salir",
                    blanco);

            SDL_Texture *texture2 =
                SDL_CreateTextureFromSurface(
                    renderer,
                    surface2);

            SDL_Surface *surface =
                TTF_RenderText_Solid(
                    font,
                    texto,
                    rojo);

            SDL_Texture *textureGO =
                SDL_CreateTextureFromSurface(
                    renderer,
                    surface);

            SDL_Rect dst =
                {
                    WIDTH / 2 - 250,
                    HEIGHT / 2 - 50,
                    500,
                    100};

            SDL_RenderCopy(
                renderer,
                textureGO,
                NULL,
                &dst);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(textureGO);
        }

        if (victory)
        {
            SDL_Color verde =
                {
                    0,
                    255,
                    0,
                    255};

            char texto[128];

            sprintf(
                texto,
                "VICTORIA - PUNTOS: %d",
                player.score);

            SDL_Surface *surface =
                TTF_RenderText_Solid(
                    font,
                    texto,
                    verde);

            SDL_Texture *textureWin =
                SDL_CreateTextureFromSurface(
                    renderer,
                    surface);

            SDL_Rect dst =
                {
                    windowWidth / 2 - 250,
                    windowHeight / 2 - 50,
                    500,
                    100};

            SDL_RenderCopy(
                renderer,
                textureWin,
                NULL,
                &dst);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(textureWin);
        }

        SDL_Surface *surface2 =
            TTF_RenderText_Solid(
                font,
                "Has recogido todos los pellets",
                blanco);

        if (gameOver &&
            event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_RETURN)
        {
            gameOver = 0;

            player.lives = 3;
            player.score = 0;

            player.x =
                (COLS * TILE_SIZE) / 2;

            player.y =
                (ROWS * TILE_SIZE) / 2;

            Mix_HaltMusic();

            Mix_PlayMusic(
                music,
                -1);

            for (int i = 0; i < NUM_GHOSTS; i++)
            {
                Ghost_SpawnRandom(
                    &ghosts[i]);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    TTF_CloseFont(font);

    TTF_Quit();

    IMG_Quit();

    Mix_FreeMusic(music);

    Mix_FreeMusic(
        victoryMusic);

    Mix_CloseAudio();

    Mix_Quit();

    SDL_Quit();

    return 0;
}