#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>

// contain all objects structs required for the game

struct Player {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0}; 
    int width = 0;
    int height = 0;
    int speed = 300;
    int currentHealth = 3;

    Uint32 coolDown = 500; // cool down between projectiles
    Uint32 lastProjectileTime = 0;
};

struct ProjectilePlayer {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0}; 
    int width = 0;
    int height = 0;
    int speed = 600;
    int damage = 1;
};

struct Enemy {
    SDL_Texture* texture = nullptr; 
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 150;
    int currentHealth = 2;

    Uint32 coolDown = 2000; // cool down between projectiles
    Uint32 lastProjectileTime = 0;
};

struct ProjectileEnemy {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int damage = 1;

    SDL_FPoint direction; // shoot toward player everytime
};

#endif