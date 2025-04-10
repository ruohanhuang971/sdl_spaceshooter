#include "SceneTitle.h"

#include "Game.h"
#include "SceneMain.h"

void SceneTitle::init() {
    // load in & play music
    titleBGM = Mix_LoadMUS("assets/music/Sci-Fi Music Pack Vol. 2/Loops/wav/1 - Astro Reverie (Loop).wav");
    Mix_PlayMusic(titleBGM, -1);
}

void SceneTitle::handleEvents(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
            Scene* sceneMain = new SceneMain();
            game.changeScene(sceneMain);
        }
    }
}

void SceneTitle::update(float deltaTime) {
    timer += deltaTime;
    if (timer >= BLINK_TIME) {
        timer -= BLINK_TIME;
    }
}

void SceneTitle::render() {
    std::string titleText = "SPACE SHOOTERS";
    game.renderTextCentered(titleText, 0.4f, true);

    if (timer <= (BLINK_TIME / 2)) {
        std::string bottomText = "Press ENTER to continue";
        game.renderTextCentered(bottomText, 0.6f, false);
    }
}

void SceneTitle::clean() {
    // clean bgm
    if (titleBGM != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(titleBGM);
    }
}
