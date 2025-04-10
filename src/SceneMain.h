#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <list>
#include <map>
#include <random>

#include "Objects.h"
#include "Scene.h"

/*
    prevent problems when SceneMain include Game
    and Game also include SceneMain -> have include
    in SceneMain.cpp instead
*/
class Game;  // forward declaration

class SceneMain : public Scene {  // inherit from Scene class
   public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvents(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

    // update helper functions
    void keyboardControls(float deltaTime);
    void updatePlayerProjectiles(float deltaTime);
    void spawnEnemy();
    void updateEnemies(float deltaTime);
    void udpateEnemyProjectiles(float deltaTime);
    void updatePlayer(float deltaTime);
    void updateExplosion(float deltaTime);
    void updateItem(float deltaTime);

    // render helper function
    void renderPlayer();
    void renderPlayerProjectiles();
    void renderEnemies();
    void renderEnemyProjectiles();
    void renderExplosions();
    void renderItems();
    void renderUI();

    // spawn projectiles
    void spawnPlayerProjectile();
    void spawnEnemyProjectile(Enemy* enemy);

    SDL_FPoint getDirection(Enemy* enemy);
    void enemyDeath(Enemy* enemy);
    void dropItem(Enemy* enemy);
    void getItem(Item* item);

   private:
    bool gameOver = false;
    int score = 0;

    Player player;
    Game& game;
    Mix_Music* bgm;
    TTF_Font* scoreFont;
    std::map<std::string, Mix_Chunk*> soundEffects;
    SDL_Texture* uiHealth;

    std::list<ProjectilePlayer*> playerProjectiles;
    std::list<Enemy*> enemies;
    std::list<ProjectileEnemy*> enemyProjectiles;
    std::list<Explosion*> explosions;
    std::list<Item*> items;

    // create template for objects to avoid repeated file reads
    ProjectilePlayer projectilePLayerTemplate;
    Enemy enemyTemplate;
    ProjectileEnemy projectileEnemyTemplate;
    Explosion explosionTemplate;
    Item itemHealthTemplate;
    Item itemShieldTemplate;
    Item itemTimeTemplate;

    // variables for random
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
};

#endif