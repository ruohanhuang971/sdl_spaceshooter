#include "SceneEnd.h"

#include "Game.h"
#include "SceneMain.h"

void SceneEnd::init() {
    // load in & play music
    endBGM = Mix_LoadMUS("assets/music/Sci-Fi Music Pack Vol. 2/Loops/wav/3 - Lunar Serenity (Loop).wav");
    Mix_PlayMusic(endBGM, -1);

    if (!SDL_IsTextInputActive()) {
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to start text input: %s\n", SDL_GetError());
    }
}

void SceneEnd::handleEvents(SDL_Event* event) {
    if (isTyping) {
        if (event->type == SDL_TEXTINPUT) {
            name += event->text.text;
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                isTyping = false;
                SDL_StopTextInput();

                if (name == "") {
                    name = "Anon";
                }
                game.insertLearderBoard(game.getScore(), name);
            }
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                name.pop_back();
            }
        }
    } else {
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                Scene* sceneMain = new SceneMain();
                game.changeScene(sceneMain);
            }
        }
    }
}

void SceneEnd::update(float deltaTime) {
    timer += deltaTime;
    if (timer > BLINK_TIME) {
        timer -= BLINK_TIME;
    }
}

void SceneEnd::render() {
    if (isTyping) {
        renderPlayerInput();
    } else {
        renderLeaderBoard();
    }
}

void SceneEnd::clean() {
    // clean bgm
    if (endBGM != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(endBGM);
    }
}

void SceneEnd::renderPlayerInput() {
    // text
    std::string gameOver = "GAME OVER";
    game.renderTextCentered(gameOver, 0.3f, true);

    int score = game.getScore();
    std::string scoreText = "Your Score is: " + std::to_string(score);
    game.renderTextCentered(scoreText, 0.4f, false);

    std::string prompt = "Enter name to save score:";
    game.renderTextCentered(prompt, 0.6f, false);

    // player input
    if (name != "") {
        SDL_Point point = game.renderTextCentered(name, 0.7f, false);
        if (timer < BLINK_TIME / 2) {
            game.renderTextPoint("_", point.x, point.y, false);
        }
    } else {
        if (timer < BLINK_TIME / 2) {
            game.renderTextCentered("_", 0.7f, false);
        }
    }
}

void SceneEnd::renderLeaderBoard() {
    // title text
    std::string leaderBoardText = "LEADER BOARD";
    game.renderTextCentered(leaderBoardText, 0.1f, true);

    // leader board text
    int index = 1;
    float posX = 0.1 * game.getWidth();
    float posY = 0.2 * game.getHeight();
    for (auto item : game.getLeaderBoard()) {
        std::string name = std::to_string(index) + ". " + item.second;
        std::string score = std::to_string(item.first);

        game.renderTextPoint(name, posX, posY, false, true);
        game.renderTextPoint(score, posX, posY, false, false);
        posY += game.getHeight() / 20;
        index++;
    }

    // text
    std::string retryText = "press ENTER to restart";
    if (timer < BLINK_TIME / 2) {
        game.renderTextCentered(retryText, 0.8f, false);
    }
}
