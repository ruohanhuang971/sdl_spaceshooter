#include "SceneMain.h"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <random>

#include "Game.h"

SceneMain::SceneMain() : game(Game::getInstance()) {
}

SceneMain::~SceneMain() {
}

void SceneMain::init() {
    // init bgm
    bgm = Mix_LoadMUS("assets/music/Sci-Fi Music Pack Vol. 2/Loops/mp3/4 - Celestial Echoes (Loop).mp3");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);

    // load in sound effects
    soundEffects["player_shoot"] = Mix_LoadWAV("assets/music/spaceship/wave/laser/laser_shoot2.wav");
    soundEffects["enemy_shoot"] = Mix_LoadWAV("assets/music/spaceship/wave/laser/laser_shoot4.wav");
    soundEffects["player_explode"] = Mix_LoadWAV("assets/music/spaceship/wave/explosion/explosion2.wav");
    soundEffects["enemy_explode"] = Mix_LoadWAV("assets/music/spaceship/wave/explosion/explosion6.wav");
    soundEffects["hit"] = Mix_LoadWAV("assets/music/spaceship/wave/utility/eff6.wav");
    soundEffects["item"] = Mix_LoadWAV("assets/music/spaceship/wave/utility/eff5.wav");


    // init random
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    // load player texture
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/space_ship.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);  // store width and height of texture in player struct
    player.width /= 3;                                                            // shrink player
    player.height /= 3;
    // center player to bottom of screen
    player.position.x = game.getWidth() / 2 - player.width / 2;
    player.position.y = game.getHeight() - player.height;

    // init player projectile template
    projectilePLayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/player_projectile.png");
    SDL_QueryTexture(projectilePLayerTemplate.texture, NULL, NULL, &projectilePLayerTemplate.width, &projectilePLayerTemplate.height);
    projectilePLayerTemplate.width /= 4;
    projectilePLayerTemplate.height /= 4;

    // init enemy templates
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/enemy_ship.png");
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;

    // init enemy projectile templates
    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/enemy_projectile.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 2;
    projectileEnemyTemplate.height /= 2;

    // init explosion template
    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/explosion.png");
    SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrames = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.height /= 2;
    explosionTemplate.width = explosionTemplate.height;  // each frame is a square rather than the whole width of animation

    // init item templates
    itemHealthTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/powerup_health.png");
    SDL_QueryTexture(itemHealthTemplate.texture, NULL, NULL, &itemHealthTemplate.width, &itemHealthTemplate.height);
    itemHealthTemplate.height /= 3;
    itemHealthTemplate.width /= 3;

    itemShieldTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/powerup_shield.png");
    SDL_QueryTexture(itemShieldTemplate.texture, NULL, NULL, &itemShieldTemplate.width, &itemShieldTemplate.height);
    itemShieldTemplate.height /= 3;
    itemShieldTemplate.width /= 3;

    itemTimeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/powerup_time.png");
    SDL_QueryTexture(itemTimeTemplate.texture, NULL, NULL, &itemTimeTemplate.width, &itemTimeTemplate.height);
    itemTimeTemplate.height /= 3;
    itemTimeTemplate.width /= 3;
}

void SceneMain::handleEvents(SDL_Event* event) {
    event = nullptr;
}

void SceneMain::update(float deltaTime) {
    keyboardControls(deltaTime);  // update based on user input on keyboard
    updatePlayerProjectiles(deltaTime);
    spawnEnemy();
    updateEnemies(deltaTime);
    udpateEnemyProjectiles(deltaTime);
    updatePlayer(deltaTime);
    updateExplosion(deltaTime);
    updateItem(deltaTime);
}

void SceneMain::render() {
    renderPlayerProjectiles();

    renderEnemyProjectiles();

    renderEnemies();

    // render player (in front of projectiles)
    renderPlayer();

    renderItems();

    renderExplosions();
}

void SceneMain::clean() {
    // clean player
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }

    // clean bgm
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }

    // clean sound effects
    for (auto sound : soundEffects) {
        if (sound.second != nullptr) {
            Mix_FreeChunk(sound.second);
        }
    }
    soundEffects.clear();

    // clean template
    if (projectilePLayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectilePLayerTemplate.texture);
    }
    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }
    if (explosionTemplate.texture != nullptr) {
        SDL_DestroyTexture(explosionTemplate.texture);
    }
    if (itemHealthTemplate.texture != nullptr) {
        SDL_DestroyTexture(itemHealthTemplate.texture);
    }

    // clean player projectile list
    for (auto projectile : playerProjectiles) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    playerProjectiles.clear();

    // clean enemy list
    for (auto enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();

    // clean player projectile list
    for (auto enemyProjectile : enemyProjectiles) {
        if (enemyProjectile != nullptr) {
            delete enemyProjectile;
        }
    }
    enemyProjectiles.clear();

    // clean explosion
    for (auto explosion : explosions) {
        if (explosion != nullptr) {
            delete explosion;
        }
    }
    explosions.clear();

    // clean items
    for (auto item : items) {
        if (item != nullptr) {
            delete item;
        }
    }
    items.clear();
}

void SceneMain::keyboardControls(float deltaTime) {
    /*  return array element with a value of 1 means that the key is pressed
        and a value of 0 means that it is not. Indexes into this array are
        obtained by using SDL_Scancode values
    */
    if (gameOver) {
        return;
    }

    auto keyboardState = SDL_GetKeyboardState(NULL);

    // player movements
    if (keyboardState[SDL_SCANCODE_W]) {  // if 'W' key is pressed
        player.position.y -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_A]) {  // if 'A' key is pressed
        player.position.x -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_S]) {  // if 'S' key is pressed
        player.position.y += player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_D]) {  // if 'D' key is pressed
        player.position.x += player.speed * deltaTime;
    }

    // bounds checking
    if (player.position.x < 0) {  // left
        player.position.x = 0;
    }
    if (player.position.x + player.width > game.getWidth()) {  // right
        player.position.x = game.getWidth() - player.width;
    }
    if (player.position.y < 0) {  // top
        player.position.y = 0;
    }
    if (player.position.y + player.height > game.getHeight()) {  // bottom
        player.position.y = game.getHeight() - player.height;
    }

    // shoot projectile
    if (keyboardState[SDL_SCANCODE_SPACE]) {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastProjectileTime > player.coolDown) {  // cooldown between shots
            spawnPlayerProjectile();
            player.lastProjectileTime = currentTime;
        }
    }
}

void SceneMain::renderEnemies() {
    for (auto enemy : enemies) {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x),
                              static_cast<int>(enemy->position.y),
                              enemy->width,
                              enemy->height};
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}

void SceneMain::renderEnemyProjectiles() {
    for (auto projectile : enemyProjectiles) {
        SDL_Rect enemyProjectileRect = {static_cast<int>(projectile->position.x),
                                        static_cast<int>(projectile->position.y),
                                        projectile->width,
                                        projectile->height};

        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI;  // get angle in degrees
        angle -= 90;                                                                         // want turn texture 0 degree at actual 90 degree on game coordinates
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &enemyProjectileRect, angle, NULL, SDL_FLIP_VERTICAL);
    }
}

void SceneMain::renderExplosions() {
    for (auto explosion : explosions) {
        SDL_Rect src = {explosion->currentFrame * explosion->width * 2, 0,
                        explosion->width * 2, explosion->height * 2};
        SDL_Rect dest = {static_cast<int>(explosion->position.x), static_cast<int>(explosion->position.y),
                         explosion->width, explosion->height};
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dest);
    }
}

void SceneMain::renderItems() {
    for (auto item : items) {
        SDL_Rect itemRect = {static_cast<int>(item->position.x),
                             static_cast<int>(item->position.y),
                             item->width,
                             item->height};
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
    }
}

void SceneMain::spawnPlayerProjectile() {
    // create a new projectile and add it to the list of projectiles in the scene
    ProjectilePlayer* projectile = new ProjectilePlayer(projectilePLayerTemplate);  // use template to init new projectile
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;

    playerProjectiles.push_back(projectile);

    Mix_PlayChannel(0, soundEffects["player_shoot"], 0); // play sound
}

void SceneMain::spawnEnemyProjectile(Enemy* enemy) {
    // create projectile
    ProjectileEnemy* projectile = new ProjectileEnemy(projectileEnemyTemplate);  // use template to init new projectile
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;
    projectile->direction = getDirection(enemy);

    enemyProjectiles.push_back(projectile);

    Mix_PlayChannel(-1, soundEffects["enemy_shoot"], 0); // play sound
}

SDL_FPoint SceneMain::getDirection(Enemy* enemy) {
    SDL_FPoint enemyCenter;
    enemyCenter.x = enemy->position.x + enemy->width / 2;
    enemyCenter.y = enemy->position.y + enemy->height / 2;
    SDL_FPoint playerCenter;
    playerCenter.x = player.position.x + player.width / 2;
    playerCenter.y = player.position.y + player.height / 2;

    // calculate direction: player center - enemy center
    SDL_FPoint direction;
    direction.x = enemyCenter.x - playerCenter.x;
    direction.y = enemyCenter.y - playerCenter.y;

    // calculate unit vector for direction
    float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
    direction.x /= length;
    direction.y /= length;

    return direction;
}

void SceneMain::enemyDeath(Enemy* enemy) {
    // create explosion
    Explosion* explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    explosion->startTime = SDL_GetTicks();
    explosions.push_back(explosion);

    Mix_PlayChannel(-1, soundEffects["enemy_explode"], 0); // play sound

    if (dis(gen) < enemy->itemDropRate) {
        dropItem(enemy);
    }

    delete enemy;
}

void SceneMain::dropItem(Enemy* enemy) {
    Item* item = new Item(itemHealthTemplate);

    // auto perc = dis(gen);
    // if (perc < (1.0f / 3)) {
    //     item->texture
    // } else if (perc < (2.0f / 3)) {

    // }

    item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
    item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2;

    float angle = dis(gen) * 2 * M_PI;
    item->direction.x = cos(angle);
    item->direction.y = sin(angle);

    items.push_back(item);
}

void SceneMain::getItem(Item* item) {
    if (item->type == ItemType::Health) {
        if (player.currentHealth < player.maxHealth) {
            player.currentHealth++;
        }
    } else if (item->type == ItemType::Shield) {
        player.shield = true;
    } else {
    }

    Mix_PlayChannel(0, soundEffects["item"], 0); // play sound
}

void SceneMain::updatePlayerProjectiles(float deltaTime) {
    for (auto it = playerProjectiles.begin(); it != playerProjectiles.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;

        if (projectile->position.y + projectile->height < 0) {  // bounds checking
            delete projectile;
            it = playerProjectiles.erase(it);  // delete projectile
        } else {
            bool hit = false;
            for (auto enemy : enemies) {  // check if projectile hits the enemy
                SDL_Rect enemyRect = {    // area of enemy
                                      static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y),
                                      enemy->width, enemy->height};
                SDL_Rect projectileRect = {// area of projectile
                                           static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y),
                                           projectile->width, projectile->height};
                if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
                    hit = true;
                    enemy->currentHealth -= projectile->damage;  // decrease health

                    delete projectile;
                    it = playerProjectiles.erase(it);  // delete projectile
                    Mix_PlayChannel(0, soundEffects["hit"], 0); // play sound

                    break;
                }
            }
            if (!hit) {
                it++;  // move to next projectile
            }
        }
    }
}

void SceneMain::spawnEnemy() {
    if (dis(gen) > 1 / 60.0f) {  // determine if there should be a enemy spawn
        return;
    }

    Enemy* enemy = new Enemy(enemyTemplate);                          // generate enemy based on template
    enemy->position.x = dis(gen) * (game.getWidth() - enemy->width);  // get random spot at top of screen for enemy to spawn
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime) {
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto enemy = *it;
        enemy->position.y += enemy->speed * deltaTime;

        // bounds checking
        if (enemy->position.y > game.getHeight()) {  // if enemies leaves the bottom of the screen
            delete enemy;
            it = enemies.erase(it);  // delete
        } else {
            if (currentTime - enemy->lastProjectileTime > enemy->coolDown && !gameOver) {  // determine if enemy should shoot
                spawnEnemyProjectile(enemy);
                enemy->lastProjectileTime = currentTime;
            }
            if (enemy->currentHealth <= 0) {
                enemyDeath(enemy);
                it = enemies.erase(it);  // delete
            } else {
                it++;  // move to next projectile
            }
        }
    }
}

void SceneMain::udpateEnemyProjectiles(float deltaTime) {
    for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end();) {
        auto projectile = *it;
        projectile->position.x -= projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y -= projectile->speed * projectile->direction.y * deltaTime;

        // bounds checking
        if (projectile->position.y + projectile->height < 0 ||
            projectile->position.y > game.getHeight() ||
            projectile->position.x + projectile->width < 0 ||
            projectile->position.x > game.getWidth()) {
            delete projectile;
            it = enemyProjectiles.erase(it);  // delete
        } else {
            SDL_Rect playerRect = {// area of player
                                   static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                                   player.width, player.height};
            SDL_Rect projectileRect = {// area of projectile
                                       static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y),
                                       projectile->width, projectile->height};
            if (SDL_HasIntersection(&playerRect, &projectileRect) && !gameOver) {
                player.currentHealth -= projectile->damage;  // decrease health

                delete projectile;
                it = enemyProjectiles.erase(it);  // delete projectile
                Mix_PlayChannel(0, soundEffects["hit"], 0); // play sound

                break;
            } else {
                it++;  // move to next projectile
            }
        }
    }
}

void SceneMain::updatePlayer(float deltaTime) {
    if (gameOver) {
        return;
    }

    if (player.currentHealth <= 0) {
        // create explosion
        Explosion* explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
        explosion->startTime = SDL_GetTicks();
        explosions.push_back(explosion);

        Mix_PlayChannel(0, soundEffects["player_explode"], 0); // play sound

        gameOver = true;
        return;
    }

    for (auto enemy : enemies) {  // detect enemy collision
        SDL_Rect playerRect = {   // area of player
                               static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                               player.width, player.height};
        SDL_Rect enemyRect = {// area of projectile
                              static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y),
                              enemy->width, enemy->height};
        if (SDL_HasIntersection(&playerRect, &enemyRect) && !gameOver) {
            player.currentHealth -= 1;  // decrease health
            enemy->currentHealth = 0;
        }
    }
}

void SceneMain::updateExplosion(float deltaTime) {
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();) {
        Explosion* explosion = *it;
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->fps / 1000.0f;  // ms * 1000 * fps
        if (explosion->currentFrame >= explosion->totalFrames) {
            delete explosion;
            it = explosions.erase(it);  // delete explosion
        } else {
            it++;
        }
    }
}

void SceneMain::updateItem(float deltaTime) {
    for (auto it = items.begin(); it != items.end();) {
        Item* item = *it;
        item->position.x -= item->speed * item->direction.x * deltaTime;
        item->position.y -= item->speed * item->direction.y * deltaTime;

        // bounce when touching edge of screen up to 3 times
        if (item->bounceCount > 0) {
            if (item->position.x < 0 || item->position.x + item->width > game.getWidth()) {
                item->direction.x = -item->direction.x;
                item->bounceCount--;
            } else if (item->position.y < 0 || item->position.y + item->height > game.getHeight()) {
                item->direction.y = -item->direction.y;
                item->bounceCount--;
            }
        }

        if (item->position.y + item->height < 0 ||  // bounds checking
            item->position.y > game.getHeight() ||
            item->position.x + item->width < 0 ||
            item->position.x > game.getWidth()) {
            delete item;
            it = items.erase(it);  // delete
        } else {                   // collosion checking
            SDL_Rect playerRect = {// area of player
                                   static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                                   player.width, player.height};
            SDL_Rect itemRect = {// area of projectile
                                 static_cast<int>(item->position.x), static_cast<int>(item->position.y),
                                 item->width, item->height};
            if (SDL_HasIntersection(&playerRect, &itemRect) && !gameOver) {
                getItem(item);
                delete item;
                it = items.erase(it);
            } else {
                it++;
            }
        }
    }
}

void SceneMain::renderPlayer() {
    // player postition is a float (for calculation precision), need to cast to int
    if (!gameOver) {
        SDL_Rect playerRect = {static_cast<int>(player.position.x),
                               static_cast<int>(player.position.y),
                               player.width,
                               player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }
}

void SceneMain::renderPlayerProjectiles() {
    for (auto projectile : playerProjectiles) {
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x),
                                   static_cast<int>(projectile->position.y),
                                   projectile->width,
                                   projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}
