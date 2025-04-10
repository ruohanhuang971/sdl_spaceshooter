#include "Game.h"
#include "SceneTitle.h"

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

    // init tff
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not open audio. SDL_ttf Error: %s\n", TTF_GetError());
        isRunning = false;
    }
    // init fonts
    titleFont = TTF_OpenFont("assets/ttf/80s-retro-future.ttf", 64);
    textFont = TTF_OpenFont("assets/ttf/80s-retro-future.ttf", 32);
    if (titleFont == nullptr || textFont == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont Error: %s\n", TTF_GetError());
        isRunning = false;
    }

    // init backgrounds
    nearStars.texture = IMG_LoadTexture(renderer, "assets/textures/background_big.png");
    SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
    farStars.texture = IMG_LoadTexture(renderer, "assets/textures/background_small.png");
    SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
    farStars.speed *= 2;

    // set currentScene
    currentScene = new SceneTitle();
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

    if (nearStars.texture != nullptr) {
        SDL_DestroyTexture(nearStars.texture);
    }
    if (farStars.texture != nullptr) {
        SDL_DestroyTexture(farStars.texture);
    }

    if (titleFont != nullptr) {
        TTF_CloseFont(titleFont);
    }
    if (textFont != nullptr) {
        TTF_CloseFont(textFont);
    }

    IMG_Quit();

    Mix_CloseAudio();
    Mix_Quit();

    TTF_Quit();

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

    backgroundUpdate(deltaTime);
}

// clear, render, show new render
void Game::render() {
    SDL_RenderClear(renderer);
    backgroundRender();
    currentScene->render();
    SDL_RenderPresent(renderer);
}

void Game::backgroundUpdate(float deltaTime) {
    nearStars.offset += nearStars.speed * deltaTime;
    if (nearStars.offset >= 0) {  // top left at top edge of canvas
        nearStars.offset -= nearStars.height;
    }

    farStars.offset += farStars.speed * deltaTime;
    if (farStars.offset >= 0) {  // top left at top edge of canvas
        farStars.offset -= farStars.height;
    }
}

void Game::backgroundRender() {
    for (int posY = static_cast<int>(farStars.offset); posY < getHeight(); posY += farStars.height) {
        for (int posX = 0; posX < getWidth(); posX += farStars.width) {
            SDL_Rect bgRect = {posX, posY, farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, nullptr, &bgRect);
        }
    }

    for (int posY = static_cast<int>(nearStars.offset); posY < getHeight(); posY += nearStars.height) {
        for (int posX = 0; posX < getWidth(); posX += nearStars.width) {
            SDL_Rect bgRect = {posX, posY, nearStars.width, nearStars.height};
            SDL_RenderCopy(renderer, nearStars.texture, nullptr, &bgRect);
        }
    }
}

void Game::renderTextCentered(std::string text, float posY, bool isTitle) {
    SDL_Color scoreColor = {255, 255, 255, 255};
    SDL_Surface* surface;
    if (isTitle) {
        surface = TTF_RenderUTF8_Solid(titleFont, text.c_str(), scoreColor);
    } else {
        surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), scoreColor);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    int y = static_cast<int>(getHeight() - surface->h) * posY;
    SDL_Rect scoreRect = {getWidth() / 2 - surface->w / 2, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
    // free score
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
