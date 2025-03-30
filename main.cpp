#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

int main(int, char**){
    // initiate SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // initiate window
    SDL_Window* window = SDL_CreateWindow("Hello World", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    // initiate render
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //initiate image
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    // load image
    SDL_Texture* texture = IMG_LoadTexture(renderer, "assets/textures/thumb_up.png");

    // initiate sound
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return 1;    
    }
    // load sound
    Mix_Music* music = Mix_LoadMUS("assets/music/Space_Background.wav");
    // play sound
    Mix_PlayMusic(music, -1);

    // initiate ttf
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;   
    }
    // load font
    TTF_Font* font = TTF_OpenFont("assets/ttf/80s-retro-future.ttf", 24);
    SDL_Color color = {0, 0, 0};
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "SO YOU ARE FINALLY AWAKE", color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surface);

    // update window
    while(true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }

         // clear render
        SDL_RenderClear(renderer);
        
        // draw rectangle
        SDL_Rect rect = {100, 100, 200, 200};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // change color
        SDL_RenderFillRect(renderer, &rect);

        // draw photo
        SDL_Rect rectPhoto = {300, 300, 200, 200};
        SDL_RenderCopy(renderer, texture, NULL, &rectPhoto);

        // draw text
        SDL_Rect rectTxt = {200, 300, surface->w, surface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &rectTxt);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // reset color
        
        // render to screen
        SDL_RenderPresent(renderer);
    }

    // clean up and exit
    SDL_DestroyTexture(texture);
    IMG_Quit();

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
