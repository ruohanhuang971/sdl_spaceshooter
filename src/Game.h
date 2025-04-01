#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>

#include "SceneMain.h"

class Game {
   public:
    /*
     since there will only be one instance of the Game class
     this allow other classes to access all the
     function/variable in the Game class
    */
    static Game& getInstance() {
        static Game instance;
        return instance;
    }

    ~Game();       // destructor
    void init();   // initalize game loop
    void run();    // main game loop logic
    void clean();  // clean up before exit
    void changeScene(Scene* scene);
    void handleEvents(SDL_Event* event);
    void update(float deltaTime);
    void render();

    // getters
    SDL_Window* getWindow() {return window;}
    SDL_Renderer* getRenderer() {return renderer;}
    int getWidth() {return WINDOW_WIDTH;}
    int getHeight() {return WINDOW_HEIGHT;}

   private:
    Game();  // constructor

    // init/delete Game instance
    Game(const Game&) = delete;
    Game& operation(const Game&) = delete;

    bool isRunning = true;
    Scene* currentScene = nullptr;  // keep track of current scene
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 800;

    // ensure update rate remain constant no matter the
    // speed of the computer
    int FPS = 60;
    Uint32 frameTime = 1000 / FPS; // 1000ms / framePerSecond
    float deltaTime;
};

#endif