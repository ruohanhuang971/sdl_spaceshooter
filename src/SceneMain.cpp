#include "SceneMain.h"

#include <SDL.h>
#include <SDL_image.h>

#include "Game.h"

SceneMain::SceneMain() : game(Game::getInstance()) {
}

SceneMain::~SceneMain() {
}

void SceneMain::init() {
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/space_ship.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);  // store width and height of texture in player struct
    player.width /= 2; // shrink player
    player.height /= 2;

    player.position.x = game.getWidth() / 2 - player.width / 2; // center player to bottom of screen
    player.position.y = game.getHeight() - player.height;
}

void SceneMain::handleEvents(SDL_Event* event) {
    event = nullptr;
}

void SceneMain::update(float deltaTime) {
    keyboardControls(deltaTime); // update based on user input on keyboard
}

void SceneMain::render() {
    // player postition is a float (for calculation precision), need to cast to int
    SDL_Rect playerRect = {static_cast<int>(player.position.x), 
                           static_cast<int>(player.position.y), 
                           player.width, 
                           player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::clean() {
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
}

void SceneMain::keyboardControls(float deltaTime) {
    /*  return array element with a value of 1 means that the key is pressed 
        and a value of 0 means that it is not. Indexes into this array are 
        obtained by using SDL_Scancode values
    */
    auto keyboardState = SDL_GetKeyboardState(NULL);

    // player movements
    if (keyboardState[SDL_SCANCODE_W]) { // if 'W' key is pressed
        player.position.y -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_A]) { // if 'A' key is pressed
        player.position.x -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_S]) { // if 'S' key is pressed
        player.position.y += player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_D]) { // if 'D' key is pressed
        player.position.x += player.speed * deltaTime;
    }

    // bounds checking
    if (player.position.x < 0) { // left
        player.position.x = 0; 
    }
    if (player.position.x + player.width > game.getWidth()) { // right
        player.position.x = game.getWidth() - player.width; 
    }
    if (player.position.y < 0) { // top
        player.position.y = 0; 
    }
    if (player.position.y + player.height > game.getHeight()) { // bottom
        player.position.y = game.getHeight() - player.height; 
    }
}
