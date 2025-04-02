#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>

// contain all objects structs required for the game

struct Player {
    SDL_Texture* texture = nullptr;  // player texture
    SDL_FPoint position = {0, 0};    // player coordinates
    int width = 0;
    int height = 0;
    int speed = 200;

    Uint32 coolDown = 500;
    Uint32 lastProjectileTime = 0;
};

struct ProjectilePlayer {
    SDL_Texture* texture = nullptr;  // player texture
    SDL_FPoint position = {0, 0};    // player coordinates
    int width = 0;
    int height = 0;
    int speed = 400;
};

#endif