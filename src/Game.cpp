#include "Game.h"

Game::Game() {
}

Game::~Game() {
    clean();
}

void Game::init() {
    // SDL init
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    // Window init
    window = SDL_CreateWindow("SDL_Spaceshooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not initialize. SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    // Renderer init
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer could not initialize. SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    // set currentScene
    currentScene = new SceneMain();
}

// handles main game loop
void Game::run() {
    while (isRunning) {  // while game is still running
        SDL_Event event;

        handleEvents(&event);
        update();
        render();
    }
}

void Game::clean() {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene* scene) {
    if (currentScene != nullptr) {  // is currentScene is empty -> clean it + delete
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;  // set currentScene to new scene
    currentScene->init();  // init new scene
}

void Game::handleEvents(SDL_Event* event) {
    while (SDL_PollEvent(event)) {      // get new game event
        if (event->type == SDL_QUIT) {  // if new game event is quit
            isRunning = false;
        }
        currentScene->handleEvents(event);
    }
}

// update game logic
void Game::update() {
    currentScene->update();
}

// clear, render, show new render
void Game::render() {
    SDL_RenderClear(renderer);
    currentScene->render();
    SDL_RenderPresent(renderer);
}
