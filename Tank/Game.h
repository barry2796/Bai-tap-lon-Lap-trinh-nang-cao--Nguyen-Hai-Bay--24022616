#include <iostream>
#include <SDL.h>
#include <vector>
#include "Wall.h"
#include "Const.h"
#include "Tank.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include <fstream>
class Game {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector<Wall> walls;
    PlayerTank player;
    int enemyNumber = 3;
    vector<EnemyTank> enemies;


    // function
    Game() {
    running = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }

    window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // ten cua so, x, y, width, height, cua so mo ngay sau khi tao
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // ve tren window, dung bo ve mac dinh, dung gpu tang toc
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }


    generateWalls();
    player = PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT-5) * TILE_SIZE,renderer);
    spawnEnemies();


}

    void render() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
    SDL_RenderClear(renderer); // delete color

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 2; i < MAP_HEIGHT - 3; i++) {
        for (int j = 2; j < MAP_WIDTH - 1 && j < 26; j++) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    for (int i = 0; i < int(walls.size()); i++) {
        walls[i].render(renderer);
    }

    player.render(renderer);

    for (auto& enemy : enemies) {
        enemy.render(renderer);
    }

    SDL_RenderPresent(renderer);
}



    void run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

    void generateWalls() {


    ifstream file("Map.txt");
    char a[26][26];
     for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            file >> a[i][j];  // Ä‘á»c tá»«ng kÃ½ tá»±
        }
    }
    /*(for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE,renderer);
            walls.push_back(w);
        }
    }*/
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 28; j++) {
            if (a[i][j] == '#') {  // vÃ­ dá»¥: dáº¥u '#' Ä‘áº¡i diá»‡n cho tÆ°á»ng
                Wall w = Wall((j+2) * TILE_SIZE, (i+1) * TILE_SIZE, renderer);
                walls.push_back(w);
            }
        }
    }
}

void handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: player.move(0, -10, walls); break;
                case SDLK_DOWN: player.move(0, 10, walls); break;
                case SDLK_LEFT: player.move(-10, 0, walls); break;
                case SDLK_RIGHT: player.move(10, 0, walls); break;
                case SDLK_SPACE: player.shoot(renderer); break;
                case SDLK_ESCAPE: running=false;
            }
        }
    }
}


void update() {
    player.updateBullets();

    for (auto& enemy : enemies) {
        //enemy.move(walls);
        enemy.moveTowardPlayer(player.x,player.y,walls);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot(renderer);
        }
    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
    }

    for (auto& bullet : player.bullets) {
    for (auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
            wall.active = false;
            bullet.active = false;
            break;
        }
    }
}

for (auto& bullet : player.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            enemy.active = false;
            bullet.active = false;
        }
    }
}

enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](EnemyTank& e) { return !e.active; }), enemies.end());

if (enemies.empty()) {
    running = false;
}

for (auto& enemy : enemies) {
    for (auto& bullet : enemy.bullets) {
        // Update
        if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
            running = false;
            return;
        }
    }
}


}



void spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            //vùng spawn là nửa trên của map, quá ngon, người chơi chỉ cần đứng dưới spam là win
            ex = (rand() % (14 - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT/2 - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }
        enemies.push_back(EnemyTank(ex, ey,renderer));
    }
}



~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

};


