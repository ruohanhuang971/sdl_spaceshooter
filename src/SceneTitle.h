#ifndef SCENETITLE_H
#define SCENETITLE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Scene.h"

class SceneTitle : public Scene {
   public:
    SceneTitle() = default;
    ~SceneTitle() = default;

    void init() override;
    void handleEvents(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

   private:
    float timer = 0.0f;
    float BLINK_TIME = 1.0f;
    Mix_Music* titleBGM;
};

#endif