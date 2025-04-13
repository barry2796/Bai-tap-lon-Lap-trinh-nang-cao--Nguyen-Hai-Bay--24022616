#include <iostream>
#include <SDL.h>
#include <vector>
#include <fstream>
#include <SDL_mixer.h>
#include "Wall.h"
#include "Const.h"
#include "Tank.h"
#include "Bullet.h"
#include "EnemyTank.h"



using namespace std;


class Game {
public:
    SDL_Window* window;
    Mix_Music* backgroundMusic;
    SDL_Renderer* renderer;
    bool running;

    int currentLevel=1;
    const int maxLevel=2;
    vector<Wall> walls;
    vector<Stone> stones;
    vector<Bush> bushes;
    vector<Water> water;
    vector<Ice> ice;
    PlayerTank player1;
    PlayerTank player2;
    int enemyNumber = 1;
    vector<EnemyTank> enemies;


    // function
    Game() {
    running = true;
    if (SDL_Init(SDL_INIT_AUDIO) < 0 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << std::endl;
    running = false;
    return;
}
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
    backgroundMusic = Mix_LoadMUS("Soundtrack.mp3");
   if (!backgroundMusic) {
        std::cerr << "Failed to load music! Error: " << Mix_GetError() << std::endl;
        running = false;
    } else {
        Mix_PlayMusic(backgroundMusic, 1);
    }
    Mix_VolumeMusic(60);
    generateWalls();
    player1 = PlayerTank(9 * TILE_SIZE, (MAP_HEIGHT-2) * TILE_SIZE,renderer,"playerTwo.png");
    player2 = PlayerTank(5 * TILE_SIZE, (MAP_HEIGHT-2) * TILE_SIZE,renderer,"playerTwo.png");
    spawnEnemies();


}

    void render() {

    //Nền đang là nên màu xám
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
    SDL_RenderClear(renderer); // delete color


    // Vẽ background màu đen trên nền màu xám
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        for (int j = 1; j < MAP_WIDTH - 6 ; j++) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    for (int i = 0; i < int(walls.size()); i++) {
        walls[i].render(renderer);
    }
    for (int i = 0; i < int(stones.size()); i++) {
        stones[i].render(renderer);
    }

    for (int i = 0; i < int(water.size()); i++) {
        water[i].render(renderer);
    }
    for (int i = 0; i < int(ice.size()); i++) {
        ice[i].render(renderer);
    }

    player1.render(renderer);
    player2.render(renderer);


    for (auto& enemy : enemies) {
        enemy.render(renderer);
    }
    for (int i = 0; i < int(bushes.size()); i++) {
        bushes[i].render(renderer);
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
    walls.clear();
    stones.clear();
    bushes.clear();
    water.clear();
    ice.clear();

    string filename = (currentLevel < 10 ? "0" : "") + to_string(currentLevel) + ".txt";
    ifstream file(filename);
    if (!file) {
        cerr << "Cannot open file " << filename << endl;
        running = false;
        return;
    }

    char a[28][26];
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 26; j++) {
            file >> a[i][j];
        }
    }

    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 26; j++) {
            int x = (j + 2) * WALL_TILE_SIZE;
            int y = (i + 1) * WALL_TILE_SIZE;
            switch (a[i][j]) {
                case '#': walls.emplace_back(x, y, renderer); break;
                case '@': stones.emplace_back(x, y, renderer); break;
                case '%': bushes.emplace_back(x, y, renderer); break;
                case '~': water.emplace_back(x, y, renderer); break;
                case '-': ice.emplace_back(x, y, renderer); break;
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
                case SDLK_UP: {
                    if(player1.isIce){
                            player1.move(0, -5, walls,stones,bushes,ice,water);
                            break;
                       }
                    else {
                            player1.move(0, -5, walls,stones,bushes,ice,water);
                            break;
                       }
                    }
                case SDLK_DOWN: {
                    if(player1.isIce){
                            player1.move(0, 5, walls,stones,bushes,ice,water);
                            break;
                       }
                    else {
                            player1.move(0, 5, walls,stones,bushes,ice,water);
                            break;
                       }
                    }
                case SDLK_LEFT: {
                    if(player1.isIce){
                            player1.move(-5, 0, walls,stones,bushes,ice,water);
                            break;
                       }
                    else {
                            player1.move(-5, 0, walls,stones,bushes,ice,water);
                            break;
                       }
                    }
                case SDLK_RIGHT: {
                    if(player1.isIce){
                            player1.move(5, 0, walls,stones,bushes,ice,water);
                            break;
                       }
                    else {
                            player1.move(5, 0, walls,stones,bushes,ice,water);
                            break;
                       }
                    }
                case SDLK_SPACE: player1.shoot(renderer); break;
                case SDLK_ESCAPE: running=false;
                case SDLK_w: player2.move(0, -10, walls,stones,bushes,ice,water); break;
                case SDLK_s: player2.move(0, 10, walls,stones,bushes,ice,water); break;
                case SDLK_a: player2.move(-10, 0, walls,stones,bushes,ice,water); break;
                case SDLK_d: player2.move(10, 0, walls,stones,bushes,ice,water); break;
                case SDLK_CAPSLOCK: player2.shoot(renderer); break;
            }
         }
    }
}


void update() {
    player1.updateBullets();
    player2.updateBullets();

    for (auto& enemy : enemies) {

        enemy.moveTowardPlayer(player1.x,player1.y,walls,stones,bushes,ice,water);
        enemy.moveTowardPlayer(player2.x,player2.y,walls,stones,bushes,ice,water);

        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot(renderer);
        }
    }






//*************************************************************************************************************************************
//Kiem tra va cham giua dan va tuong



    for (auto& bullet : player1.bullets) {
    for (auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
            wall.active = false;
            bullet.active = false;
            break;
        }
    }
}

    for (auto& bullet : player2.bullets) {
    for (auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
            wall.active = false;
            bullet.active = false;
            break;
        }
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

    for (auto& bullet : player1.bullets) {
    for (auto& stone : stones) {
        if (stone.active && SDL_HasIntersection(&bullet.rect, &stone.rect)) {

            bullet.active = false;
            break;
        }
    }
}

    for (auto& bullet : player2.bullets) {
    for (auto& stone : stones) {
        if (stone.active && SDL_HasIntersection(&bullet.rect, &stone.rect)) {

            bullet.active = false;
            break;
        }
    }
}


    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            for (auto& stone : stones) {
                if (stone.active && SDL_HasIntersection(&bullet.rect, &stone.rect)) {

                bullet.active = false;
                break;
              }
            }
        }
    }
//**************************************************************************************************************************************







//**************************************************************************************************************************************
//Kiem tra va cham giua dan va enemy

for (auto& bullet : player1.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            enemy.active = false;
            bullet.active = false;
        }
    }
}
for (auto& bullet : player2.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            enemy.active = false;
            bullet.active = false;
        }
    }
}
//**************************************************************************************************************************************




//**************************************************************************************************************************************
//Xe tang dich trung dan thi chet

enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](EnemyTank& e) { return !e.active; }), enemies.end());

if (enemies.empty()) {
    if (currentLevel < maxLevel) {
        currentLevel++;
        cout << "Level up! Now loading level\n " << currentLevel << endl;
        generateWalls();
        spawnEnemies();
        // Reset lại vị trí người chơi
        player1 = PlayerTank(9 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "playerTwo.png");
        player2 = PlayerTank(5 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "playerTwo.png");
        // clear đạn
        player1.bullets.clear();
        player2.bullets.clear();
    } else {
        cout << "WIN GAME! YOU BEAT ALL LEVELS!" << endl;
        running = false;
    }
}

//**************************************************************************************************************************************








//**************************************************************************************************************************************
int nguoichoiconlai=2;
//Dan ban vao nguoi choi
for (auto& enemy : enemies) {
    for (auto& bullet : enemy.bullets) {
        // Update
        if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
            running=false;
            nguoichoiconlai--;
            return;
        }
    }
}


for (auto& enemy : enemies) {
    for (auto& bullet : enemy.bullets) {
        // Update
        if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
            running=false;
            nguoichoiconlai--;
            return;
        }
    }
}
if(nguoichoiconlai==0) {running=false; cout<<"Lost";}
//**************************************************************************************************************************************









//**************************************************************************************************************************************
//Dan ban vao dan
for (auto& bulletP : player1.bullets) {
    if (!bulletP.active) continue; // bỏ qua đạn đã vô hiệu
    for (auto& enemy : enemies) {
        for (auto& bulletE : enemy.bullets) {
            if (!bulletE.active) continue; // bỏ qua đạn đã vô hiệu
            if (SDL_HasIntersection(&bulletP.rect, &bulletE.rect)) {
                bulletP.active = false;
                bulletE.active = false;
            }
        }
    }
}


for (auto& bulletP : player2.bullets) {
    if (!bulletP.active) continue; // bỏ qua đạn đã vô hiệu
    for (auto& enemy : enemies) {
        for (auto& bulletE : enemy.bullets) {
            if (!bulletE.active) continue; // bỏ qua đạn đã vô hiệu
            if (SDL_HasIntersection(&bulletP.rect, &bulletE.rect)) {
                bulletP.active = false;
                bulletE.active = false;
            }
        }
    }
}
for (auto& bulletP : player2.bullets) {
    if (!bulletP.active) continue;

    for (auto& bulletP1 : player1.bullets) {
        if (!bulletP1.active) continue;

        if (SDL_HasIntersection(&bulletP.rect, &bulletP1.rect)) {
            bulletP.active = false;
            bulletP1.active = false;
        }
    }
}

//**************************************************************************************************************************************


}






void spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            //vùng spawn là nửa trên của map, quá ngon, người chơi chỉ cần đứng dưới spam là win
            ex = (rand() % (MAP_WIDTH -7) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT/2 - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
            for (const auto& stone : stones) {
                if (stone.active && stone.x == ex && stone.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }
        enemies.push_back(EnemyTank(ex, ey,renderer));
    }
}



~Game() {
    Mix_FreeMusic(backgroundMusic);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

};


