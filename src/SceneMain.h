#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Objects.h"
#include "Scene.h"

#include <list>
#include <random>

/*
    prevent problems when SceneMain include Game
    and Game also include SceneMain -> have include
    in SceneMain.cpp instead
*/
class Game; // forward declaration

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
    
    // render helper function
    void renderPlayerProjectiles();
    void renderEnemies();
    void renderEnemyProjectiles();

    // spawn projectiles
    void spawnPlayerProjectile();
    void spawnEnemyProjectile(Enemy* enemy);

    SDL_FPoint getDirection(Enemy* enemy);

   private:
    Player player;
    Game& game;
    std::list<ProjectilePlayer*> playerProjectiles;
    std::list<Enemy*> enemies;
    std::list<ProjectileEnemy*> enemyProjectiles;

    // create template for objects to avoid repeated file reads
    ProjectilePlayer projectilePLayerTemplate;
    Enemy enemyTemplate;
    ProjectileEnemy projectileEnemyTemplate;
    
    // variables for random
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
};

#endif