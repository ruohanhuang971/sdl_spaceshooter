#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Objects.h"
#include "Scene.h"

#include <list>

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

    void keyboardControls(float deltaTime);
    void updatePlayerProjectiles(float deltaTime);
    
    void renderPlayerProjectiles();

    void playerShoot();

   private:
    Player player;
    Game& game;

    // create template for objects to avoid repeated file reads
    ProjectilePlayer projectilePLayerTemplate;

    std::list<ProjectilePlayer*> playerProjectiles;
};

#endif