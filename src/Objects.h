#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>

// contain all objects structs required for the game
enum class ItemType {
    Health,
    Shield,
    Time
};

struct Player {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0}; 
    int width = 0;
    int height = 0;
    int speed = 300;
    int currentHealth = 10;
    int maxHealth = 10;
    bool shield = false;

    Uint32 coolDown = 300; // cool down between projectiles
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
    
    float itemDropRate = 1.0f;
    Uint32 coolDown = 3000; // cool down between projectiles
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

struct Explosion {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;

    int currentFrame = 0;
    int totalFrames= 0;
    Uint32 startTime = 0;
    Uint32 fps = 10;
};

struct Item {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;

    SDL_FPoint direction; // random
    int bounceCount = 3;
    ItemType type = ItemType::Health;
};

struct Background {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 50;

    float offset = 0;
};

#endif