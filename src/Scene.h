#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>

// need to be implemented by children
class Scene {
   public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void handleEvents(SDL_Event* event) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
};

#endif