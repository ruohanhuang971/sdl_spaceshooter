#ifndef SCENEEND_H
#define SCENEEND_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

#include "Scene.h"

class SceneEnd : public Scene {
   public:
    void init() override;
    void handleEvents(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

    // render functions
    void renderPlayerInput();
    void renderLeaderBoard();

   private:
    bool isTyping = true; // if player is typing their name
    std::string name = "";

    float timer = 0.0f;
    float BLINK_TIME = 1.0f;
    Mix_Music* endBGM;
};

#endif