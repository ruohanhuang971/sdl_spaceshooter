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

    // SDL_image init
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
        isRunning = false;
    }

    // SDL_mixer init
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize. SDL_mixer Error: %s\n", Mix_GetError());
        isRunning = false;
    }
    // open mixer channel
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not open audio. SDL_mixer Error: %s\n", Mix_GetError());
        isRunning = false;
    }

    // set currentScene
    currentScene = new SceneMain();
    currentScene->init();
}

// handles main game loop
void Game::run() {
    while (isRunning) {  // while game is still running
        auto start = SDL_GetTicks();

        SDL_Event event;

        handleEvents(&event);
        update(deltaTime);
        render();

        auto end = SDL_GetTicks();
        auto elapsed = end - start;  // count how long 1 frame takes to run
        if (elapsed < frameTime) {
            SDL_Delay(frameTime - elapsed);   // then total time must be equal to frameTime
            deltaTime = frameTime / 1000.0f;  // convert deltaTime from ms to seconds
        } else {
            deltaTime = elapsed / 1000.0f;  // convert deltaTime from ms to seconds
        }
    }
}

void Game::clean() {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }

    IMG_Quit();

    Mix_CloseAudio();
    Mix_Quit();

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
void Game::update(float deltaTime) {
    currentScene->update(deltaTime);
}

// clear, render, show new render
void Game::render() {
    SDL_RenderClear(renderer);
    currentScene->render();
    SDL_RenderPresent(renderer);
}
