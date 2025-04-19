#include <iostream>
#include <SDL.h>
#include <vector>
#include <fstream>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Wall.h"
#include "Const.h"
#include "Tank.h"
#include "Bullet.h"
#include "EnemyTank.h"

enum GameMode {
    ONE_PLAYER,
    TWO_PLAYER
};

using namespace std;


  class Game {
public:
    SDL_Window* window;
    Mix_Music* backgroundMusic;
    Mix_Chunk* shootSound = nullptr;
    Mix_Chunk* enemyShootSound = nullptr;
    Mix_Chunk* dieSound = nullptr;
    SDL_Renderer* renderer;
    bool running;

    int currentLevel=1;
    const int maxLevel=20;
    vector<Wall> walls;
    vector<Stone> stones;
    vector<Bush> bushes;
    vector<Water> water;
    vector<Ice> ice;
    PlayerTank player1;
    PlayerTank player2;
    Base base;
    int enemyNumber = 1 ;
    vector<EnemyTank> enemies;
    TTF_Font* font;
    GameMode gameMode;

    // constructor
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


//    enemyShootSound = Mix_LoadWAV
    backgroundMusic = Mix_LoadMUS("Sound.mp3");
    shootSound = Mix_LoadWAV("playerTankshootSound.wav");
    if (!shootSound) {
        cerr << "Failed to load shoot sound! Error: " << Mix_GetError() << endl;
    }
    if (!backgroundMusic) {
        std::cerr << "Failed to load music! Error: " << Mix_GetError() << std::endl;
        running = false;
    } else {
        Mix_PlayMusic(backgroundMusic, 1);
    }



    if (TTF_Init() == -1) {
                cout << "TTF init failed: " << TTF_GetError() << endl;
            }
    font = TTF_OpenFont("PressStart2P.ttf", 24);
            if (!font) {
                cout << "Failed to load font: " << TTF_GetError() << std::endl;
            }




    Mix_VolumeMusic(10);
    generateWalls();
    player1 = PlayerTank(9 * TILE_SIZE, (MAP_HEIGHT-2) * TILE_SIZE,renderer,"resource/image/playerOne.png");
    player2 = PlayerTank(5 * TILE_SIZE, (MAP_HEIGHT-2) * TILE_SIZE,renderer,"resource/image/playerTwo.png");
    base = Base(7 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);
    spawnEnemies();



}

// Hàm hiển thị menu chính của trò chơi
void showMenu() {
    bool inMenu = true;
    int selectedOption = 0; // 0 = Start Game, 1 = Exit
    SDL_Event event;
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};

    while (inMenu) {
        // Vẽ nền menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Tạo các chuỗi menu
        string options[2] = {"Start Game", "Exit"};
        for (int i = 0; i < 2; ++i) {
            SDL_Color color = (i == selectedOption) ? yellow : white;
            SDL_Surface* surface = TTF_RenderText_Blended(font, options[i].c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dstRect = { SCREEN_WIDTH / 2 - surface->w / 2, 200 + i * 50, surface->w, surface->h };
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);

        // Xử lý sự kiện bàn phím trong menu
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inMenu = false;
                return;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + 2) % 2;
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % 2;
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selectedOption == 0) {
                            gameMode = ChooseMode(); // Lưu chế độ chơi
                            inMenu = false; // Thoát menu để vào game
                        } else {
                            running = false; // Thoát game
                            inMenu = false;
                        }
                        break;
                }
            }
        }
        SDL_Delay(100);
    }
}

GameMode ChooseMode() {
    bool inChooseMode = true;
    int selectedMode = 0; // 0 = 1P, 1 = 2P
    SDL_Event event;
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};

    while (inChooseMode) {
        // Vẽ nền menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Tạo các chuỗi menu
        string options[2] = {"1 Player", "2 Player"};
        for (int i = 0; i < 2; ++i) {
            SDL_Color color = (i == selectedMode) ? yellow : white;
            SDL_Surface* surface = TTF_RenderText_Blended(font, options[i].c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dstRect = { SCREEN_WIDTH / 2 - surface->w / 2, 200 + i * 50, surface->w, surface->h };
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);

        // Xử lý sự kiện bàn phím trong menu
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                return GameMode::ONE_PLAYER; // Hoặc có thể trả về một giá trị đặc biệt để thoát
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedMode = (selectedMode - 1 + 2) % 2;
                        break;
                    case SDLK_DOWN:
                        selectedMode = (selectedMode + 1) % 2;
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selectedMode == 0) {
                            cout << "Chon Mode 1 Nguoi Choi" << endl;
                            return GameMode::ONE_PLAYER;
                        } else {
                            cout << "Chon Mode 2 Nguoi Choi" << endl;
                            return GameMode::TWO_PLAYER;
                        }
                        break;
                }
            }
        }
        SDL_Delay(100);
    }
    return GameMode::ONE_PLAYER; // Mặc định nếu không có lựa chọn
}

//Hiện level bên phải màn hình
    void renderLevel(){
        SDL_Color white = {0, 0, 0};

        string p1 = "Level: " + to_string(currentLevel);

        SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, p1.c_str(), white);
        SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
        SDL_Rect dstRect1 = {570, 130, textSurface1->w, textSurface1->h}; // tuỳ chỉnh vị trí


        SDL_RenderCopy(renderer, textTexture1, NULL, &dstRect1);

        // Clean up
        SDL_FreeSurface(textSurface1);
        SDL_DestroyTexture(textTexture1);
    }


void renderGameOver(bool& gameRunning) {  // Truyền tham chiếu để thay đổi biến thực sự
    // Clear màn hình
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Hiển thị thông báo Game Over
    SDL_Color yellow = {255, 255, 0};
    string gameOver = "GAME OVER!";
    SDL_Surface* surface = TTF_RenderText_Blended(font, gameOver.c_str(), yellow);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {
        SCREEN_WIDTH / 2 - surface->w / 2,
        200,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Hiển thị
    SDL_RenderPresent(renderer);

    // Cập nhật trạng thái game
    SDL_Delay(2000);
    gameRunning = false;
}
//Hàm render các đối tượng
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

    base.render(renderer);
    player1.render(renderer);

    if(gameMode== TWO_PLAYER){
          player2.render(renderer);
    }


    for (auto& enemy : enemies) {


        enemy.render(renderer);
    }
    for (int i = 0; i < int(bushes.size()); i++) {
        bushes[i].render(renderer);
    }

      renderLevel();


    SDL_RenderPresent(renderer);


}


//Chạy code
    void run() {
    showMenu(); // Gọi menu trước khi chạy game
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

//Hàm tạo map
    void generateWalls() {
    walls.clear();
    stones.clear();
    bushes.clear();
    water.clear();
    ice.clear();
    //Mở file map
    string filename =  (currentLevel < 10 ? "0" : "") + to_string(currentLevel) + ".txt";
    filename = "resource/level/" + filename;
    ifstream file(filename);
    if (!file) {


        cerr << "Cannot open file " << filename << endl;
        running = false;
        return;
    }
//Đọc file map
    char a[28][26];
    for (int i = 0; i < 28; i++) {


        for (int j = 0; j < 26; j++) {
            file >> a[i][j];
        }
    }
//Quét các kí tự trong file map
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


//xử lí sự kiện bàn phím
void handleEvents() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    player1.shoot(renderer);
                    if (Mix_PlayChannel(-1, shootSound, 0) == -1) {
                        cerr << "Failed to play sound: " << Mix_GetError() << endl;
                    }
                    break;


                case SDLK_CAPSLOCK:
                    if(gameMode== TWO_PLAYER){
                    player2.shoot(renderer);
                    }
                    break;

                case SDLK_ESCAPE:
                    running = false;
                    break;
            }
        }
    }

    // Xử lý giữ phím để di chuyển mượt hơn
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // Player 1
    if (keystates[SDL_SCANCODE_UP]) {
        player1.move(0, -2, walls, stones, bushes, ice, water,base);
    } else if (keystates[SDL_SCANCODE_DOWN]) {
        player1.move(0, 2, walls, stones, bushes, ice, water,base);
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        player1.move(-2, 0, walls, stones, bushes, ice, water,base);
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        player1.move(2, 0, walls, stones, bushes, ice, water,base);
    }

    // Player 2
    if(gameMode== TWO_PLAYER){
                if (keystates[SDL_SCANCODE_W]) {
                    player2.move(0, -2, walls, stones, bushes, ice, water,base);
                } else if (keystates[SDL_SCANCODE_S]) {
                    player2.move(0, 2, walls, stones, bushes, ice, water,base);
                }
                if (keystates[SDL_SCANCODE_A]) {
                    player2.move(-2, 0, walls, stones, bushes, ice, water,base);
                } else if (keystates[SDL_SCANCODE_D]) {
                    player2.move(2, 0, walls, stones, bushes, ice, water,base);
                }
    }
}

//UPDATE
void update() {
    player1.updateBullets();
    player2.updateBullets();

    for (auto& enemy : enemies) {

        enemy.moveTowardPlayer(player1.x,player1.y,walls, stones, bushes, ice, water,base);
        if(gameMode== TWO_PLAYER){
               enemy.moveTowardPlayer(player2.x,player2.y,walls, stones, bushes, ice, water,base);
        }
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot(renderer);
        }
    }






//*************************************************************************************************************************************
//Đạn người chơi 1 bắn va chạm với tường
    for (auto& bullet : player1.bullets) {



    for (auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
            wall.active = false;
            bullet.active = false;
            break;
        }
    }
}
//Đạn người chơi 2 bắn va chạm với tường
if(gameMode== TWO_PLAYER){
    for (auto& bullet : player2.bullets) {


//Đạn kẻ địch bắn va chạm với tường
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
}
//Đạn người chơi 1 bắn va chạm với sắt
    for (auto& bullet : player1.bullets) {


    for (auto& stone : stones) {
        if (stone.active && SDL_HasIntersection(&bullet.rect, &stone.rect)) {

            bullet.active = false;
            break;
        }
    }
}
//Đạn người chơi 2 bắn va chạm với sắt
if(gameMode== TWO_PLAYER){
    for (auto& bullet : player2.bullets) {

    for (auto& stone : stones) {
        if (stone.active && SDL_HasIntersection(&bullet.rect, &stone.rect)) {

            bullet.active = false;
            break;
        }
    }
}
}
//Đạn kẻ địch bắn va chạm với sắt
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
//Người chơi bắn trúng kẻ địch

for (auto& bullet : player1.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            enemy.active = false;
            bullet.active = false;
        }
    }
}


if(gameMode== TWO_PLAYER){
for (auto& bullet : player2.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            enemy.active = false;
            bullet.active = false;
        }
    }
}
}
//**************************************************************************************************************************************




//**************************************************************************************************************************************
//Xoá các xe tăng địch đã bị đánh dấu là chết
enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](EnemyTank& e) { return !e.active; }), enemies.end());


//Nếu bắn chết hết
if (enemies.empty()) {
        SDL_Delay(50);
    if (currentLevel < maxLevel) {
        currentLevel++;
        cout << "Level up! Now loading level\n " << currentLevel << endl;
        generateWalls();
        spawnEnemies();
        // Reset lại vị trí người chơi
        player1 = PlayerTank(9 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerOne.png");
        player2 = PlayerTank(5 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerTwo.png");
        // clear đạn
        player1.bullets.clear();
        player2.bullets.clear();
        base.active=true;
    } else {
        cout << "WIN GAME! YOU BEAT ALL LEVELS!" << endl;
        running = false;
    }
}

//**************************************************************************************************************************************








//**************************************************************************************************************************************

//Dan ban vao nguoi choi
for (auto& enemy : enemies) {
    for (auto& bullet : enemy.bullets) {
        // Update
        if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
            running=false;

            return;
        }
    }
}


for (auto& enemy : enemies) {
    for (auto& bullet : enemy.bullets) {
        // Update
        if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
            running=false;

            return;
        }
    }
}

//**************************************************************************************************************************************









//**************************************************************************************************************************************
//Đạn người chơi 1 và kẻ địch va chạm nhau
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

//Đạn người chơi 2 và kẻ địch va chạm nhau
if(gameMode== TWO_PLAYER){
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
}
//**************************************************************************************************************************************

//Kiểm tra đạn người chơi bắn vào base
for (auto& bulletP : player1.bullets) {
    if (!bulletP.active) continue; // bỏ qua đạn đã vô hiệu
    if (SDL_HasIntersection(&bulletP.rect, &base.rect)) {
                bulletP.active = false;
                base.active = false;
                renderGameOver(running);

            }
}



}

// Thêm vào class Game (nếu chưa có)
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}


//Hàm sinh xe tăng địch
void spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        int attempts = 0; // Giới hạn số lần thử để tránh vòng lặp vô hạn
        const int maxAttempts = 100;

        while (!validPosition && attempts < maxAttempts) {
            attempts++;
            // Chọn vị trí ngẫu nhiên trong nửa trên của map
            ex = (rand() % (MAP_WIDTH - 7) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT / 2 - 2) + 1) * TILE_SIZE;

            validPosition = true; // Giả sử vị trí hợp lệ

            // Kiểm tra va chạm với tường
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }

            // Kiểm tra va chạm với đá
            for (const auto& stone : stones) {
                if (stone.active && stone.x == ex && stone.y == ey) {
                    validPosition = false;
                    break;
                }
            }

            // Kiểm tra va chạm với nước
            for (const auto& w : water) {
                if (w.active && w.x == ex && w.y == ey) {
                    validPosition = false;
                    break;
                }
            }


            // Kiểm tra va chạm với xe tăng người chơi
            if ((player1.x == ex && player1.y == ey) ||
                (player2.x == ex && player2.y == ey)) {
                validPosition = false;
            }

            // Kiểm tra va chạm với xe tăng địch khác (nếu có)
            for (const auto& enemy : enemies) {
                if (enemy.x == ex && enemy.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }

        if (validPosition) {
            enemies.push_back(EnemyTank(ex, ey, renderer));
        } else {
            std::cerr << "Không thể tìm vị trí spawn hợp lệ cho xe tăng địch sau " << maxAttempts << " lần thử.\n";
        }
    }
}


//Destructor
~Game() {
    Mix_FreeMusic(backgroundMusic);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

};


