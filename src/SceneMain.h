#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Objects.h"
#include "Scene.h"

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

   private:
    Player player;
    Game& game;
};

#endif