#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Scene.h"

class SceneMain : public Scene { // inherit from Scene class
    public:
    SceneMain();
    ~SceneMain();
    
    void init() override;
    void handleEvents(SDL_Event* event) override;
    void update() override;
    void render() override;
    void clean() override;
};

#endif