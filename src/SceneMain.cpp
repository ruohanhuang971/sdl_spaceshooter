#include "SceneMain.h"

#include <SDL.h>
#include <SDL_image.h>

#include <random>

#include "Game.h"

SceneMain::SceneMain() : game(Game::getInstance()) {
}

SceneMain::~SceneMain() {
}

void SceneMain::init() {
    // init random
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    // load player texture
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/space_ship.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);  // store width and height of texture in player struct
    player.width /= 2;                                                            // shrink player
    player.height /= 2;
    // center player to bottom of screen
    player.position.x = game.getWidth() / 2 - player.width / 2;  
    player.position.y = game.getHeight() - player.height;

    // init player projectile template
    projectilePLayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/textures/player_projectile.png");
    SDL_QueryTexture(projectilePLayerTemplate.texture, NULL, NULL, &projectilePLayerTemplate.width, &projectilePLayerTemplate.height);
    projectilePLayerTemplate.width /= 4;
    projectilePLayerTemplate.height /= 2;
    
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
}

void SceneMain::render() {
    // render player projectiles
    renderPlayerProjectiles();

    // render enemy projectiles
    renderEnemyProjectiles();

    //render enemies
    renderEnemies();

    // render player (in front of projectiles)
    // player postition is a float (for calculation precision), need to cast to int
    SDL_Rect playerRect = {static_cast<int>(player.position.x),
                           static_cast<int>(player.position.y),
                           player.width,
                           player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::clean() {
    // clean player
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }

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
}

void SceneMain::keyboardControls(float deltaTime) {
    /*  return array element with a value of 1 means that the key is pressed
        and a value of 0 means that it is not. Indexes into this array are
        obtained by using SDL_Scancode values
    */
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
        
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI; // get angle in degrees
        angle -= 90; // want turn texture 0 degree at actual 90 degree on game coordinates
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &enemyProjectileRect, angle, NULL, SDL_FLIP_VERTICAL);
    }
}

void SceneMain::spawnPlayerProjectile() {
    // create a new projectile and add it to the list of projectiles in the scene
    ProjectilePlayer* projectile = new ProjectilePlayer(projectilePLayerTemplate);  // use template to init new projectile
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;

    playerProjectiles.push_back(projectile);
}

void SceneMain::spawnEnemyProjectile(Enemy* enemy) {
    // create projectile
    ProjectileEnemy* projectile = new ProjectileEnemy(projectileEnemyTemplate); // use template to init new projectile
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;
    projectile->direction = getDirection(enemy);

    enemyProjectiles.push_back(projectile);
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

void SceneMain::updatePlayerProjectiles(float deltaTime) {
    for (auto it = playerProjectiles.begin(); it != playerProjectiles.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;

        // bounds checking
        if (projectile->position.y + projectile->height < 0) {  // left
            delete projectile;
            it = playerProjectiles.erase(it);  // delete
        } else {
            it++;  // move to next projectile
        }
    }
}

void SceneMain::spawnEnemy() {
    if (dis(gen) > 1 / 60.0f) { // determine if there should be a enemy spawn
        return;
    }

    Enemy* enemy = new Enemy(enemyTemplate); // generate enemy based on template
    enemy->position.x = dis(gen) * (game.getWidth() - enemy->width); // get random spot at top of screen for enemy to spawn
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
            if (currentTime - enemy->lastProjectileTime > enemy->coolDown) { // determine if enemy should shoot
                spawnEnemyProjectile(enemy);
                enemy->lastProjectileTime = currentTime;
            }
            it++;  // move to next projectile
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
            it++;  // move to next projectile
        }
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
