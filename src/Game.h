#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "SceneMain.h"

class Game {
   public:
    Game();        // constructor
    ~Game();       // destructor
    void init();   // initalize game loop
    void run();    // main game loop logic
    void clean();  // clean up before exit
    void changeScene(Scene* scene);

    void handleEvents(SDL_Event* event);
    void update();
    void render();
   private:
    bool isRunning = true;
    Scene* currentScene = nullptr;  // keep track of current scene
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 800;
};

#endif