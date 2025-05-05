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
#include "boss.h"



using namespace std;


  class Game {
public:
    SDL_Window* window;
    SDL_Texture* heartTexture;
    Mix_Music* backgroundMusic = nullptr;
    Mix_Music* bossBackgroundMusic = nullptr;
    Mix_Chunk* shootSound = nullptr;
    Mix_Chunk* enemyShootSound = nullptr;
    Mix_Chunk* dieSound = nullptr;
    Mix_Chunk* selectSound = nullptr;
    Mix_Chunk* gameOverSound = nullptr;
    Mix_Chunk* chooseSound = nullptr;
    Mix_Chunk* chooseSound2 = nullptr;
    Mix_Chunk* victorySound = nullptr;
    Mix_Chunk* bossExplosionSound=nullptr;
    SDL_Renderer* renderer;
    bool running;
    bool isBossLevel=false;
    int scoreP1=0;
    int scoreP2=0;
    int currentLevel=1;
    const int maxLevel=2;
    vector<Wall> walls;
    vector<Stone> stones;
    vector<Bush> bushes;
    vector<Water> water;
    vector<Ice> ice;
    vector<wallExplosion> wallExplosions;
    vector<tankExplosion> tankExplosions;
    vector<bossExplosion> bossExplosions;
    PlayerTank player1;
    PlayerTank player2;
    Base base;
    int enemyNumber = 10 ;
    int enemiesSpawned = 0;
    Uint32 lastSpawnTime = 0;

    vector<EnemyTank> enemies;
    TTF_Font* font;
    GameMode gameMode;
    Boss boss = Boss(0, 0, nullptr);
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
    backgroundMusic = Mix_LoadMUS("resource/sound/Sound.mp3");
    shootSound = Mix_LoadWAV("resource/sound/tankShotSound.wav");
    selectSound = Mix_LoadWAV("resource/sound/selectSound.wav");
    gameOverSound = Mix_LoadWAV("resource/sound/gameOver.wav");
    chooseSound = Mix_LoadWAV("resource/sound/chooseSound1.wav");
    chooseSound2 = Mix_LoadWAV("resource/sound/chooseSound2.wav");
    victorySound = Mix_LoadWAV("resource/sound/victory.wav");
    bossExplosionSound = Mix_LoadWAV("resource/sound/bossExplosionSound.wav");

    if (!gameOverSound) {
        cerr << "Failed to load shoot sound! Error: " << Mix_GetError() << endl;
    }
    if (!shootSound) {
        cerr << "Failed to load shoot sound! Error: " << Mix_GetError() << endl;
    }
    if (!selectSound) {
        cerr << "Failed to load shoot sound! Error: " << Mix_GetError() << endl;
    }
    if (!backgroundMusic) {
        std::cerr << "Failed to load music! Error: " << Mix_GetError() << std::endl;
        running = false;
    }
    if(!isBossLevel){
        Mix_HaltMusic();
    backgroundMusic = Mix_LoadMUS("resource/sound/Sound.mp3");
    Mix_PlayMusic(backgroundMusic, -1);
    }


    if (TTF_Init() == -1) {
                cout << "TTF init failed: " << TTF_GetError() << endl;
            }
    font = TTF_OpenFont("PressStart2P.ttf", 24);
            if (!font) {
                cout << "Failed to load font: " << TTF_GetError() << std::endl;
            }


    SDL_Surface* heartSurface = IMG_Load("resource/image/heart.png");
    if (!heartSurface) {
            cout<<"loi";
        running=false;
    }
    heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
    SDL_FreeSurface(heartSurface);



    Mix_VolumeMusic(100);
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
                    case SDLK_UP:{
                        selectedOption = (selectedOption - 1 + 2) % 2;
                        if (Mix_PlayChannel(-1, selectSound, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                        }
                        break;
                    }
                    case SDLK_DOWN:{
                        selectedOption = (selectedOption + 1) % 2;
                        if (Mix_PlayChannel(-1, selectSound, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                        }
                        break;
                    }
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selectedOption == 0) {
                            if (Mix_PlayChannel(-1, chooseSound, 0) == -1) {
                                cerr << "Failed to play sound: " << Mix_GetError() << endl;
                            }
                            gameMode = ChooseMode(); // Lưu chế độ chơi
                            inMenu = false; // Thoát menu để vào game
                        } else {
                            if (Mix_PlayChannel(-1, chooseSound, 0) == -1) {
                                cerr << "Failed to play sound: " << Mix_GetError() << endl;
                            }
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
                    case SDLK_UP:{
                        selectedMode = (selectedMode - 1 + 2) % 2;
                        if (Mix_PlayChannel(-1, selectSound, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                        }
                        break;
                    }
                    case SDLK_DOWN:{
                        if (Mix_PlayChannel(-1, selectSound, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                        }
                        selectedMode = (selectedMode + 1) % 2;
                        break;
                    }
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selectedMode == 0) {
                            if (Mix_PlayChannel(-1, chooseSound2, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                            }
                            cout << "Chon Mode 1 Nguoi Choi" << endl;
                            return GameMode::ONE_PLAYER;
                        } else {
                            if (Mix_PlayChannel(-1, chooseSound2, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                            }
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
void renderLevel() {
    SDL_Color black = {0, 0, 0};

    // Render Level Text
    string levelText = "Level: " + to_string(currentLevel);
    SDL_Surface* levelSurface = TTF_RenderText_Blended(font, levelText.c_str(), black);
    SDL_Texture* levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);
    SDL_Rect levelRect = {570, 130, levelSurface->w, levelSurface->h};
    SDL_RenderCopy(renderer, levelTexture, NULL, &levelRect);
    SDL_FreeSurface(levelSurface);
    SDL_DestroyTexture(levelTexture);

    // Render Hearts for Player 1
    SDL_Surface* p1Surface = TTF_RenderText_Blended(font, "P1:", black);
    SDL_Texture* p1Texture = SDL_CreateTextureFromSurface(renderer, p1Surface);
    SDL_Rect p1Rect = {570, 170, p1Surface->w, p1Surface->h};
    SDL_RenderCopy(renderer, p1Texture, NULL, &p1Rect);
    SDL_FreeSurface(p1Surface);
    SDL_DestroyTexture(p1Texture);
    int heartWidth = 24, heartHeight = 24; // tuỳ kích thước ảnh của bạn
    for (int i = 0; i < player1.remainingLives; ++i) {
        SDL_Rect heartRect = {650 + i * (heartWidth + 5), 170, heartWidth, heartHeight};
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }

    // Render Hearts for Player 2 nếu chơi 2 người
    if (gameMode == TWO_PLAYER) {
        SDL_Surface* p2Surface = TTF_RenderText_Blended(font, "P2:", black);
        SDL_Texture* p2Texture = SDL_CreateTextureFromSurface(renderer, p2Surface);
        SDL_Rect p2Rect = {570, 210, p2Surface->w, p2Surface->h};
        SDL_RenderCopy(renderer, p2Texture, NULL, &p2Rect);
        SDL_FreeSurface(p2Surface);
        SDL_DestroyTexture(p2Texture);
        for (int i = 0; i < player2.remainingLives; ++i) {
            SDL_Rect heartRect = {650 + i * (heartWidth + 5), 210, heartWidth, heartHeight};
            SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
        }
    }

    //hiện điểm người chơi 1
    string p1ScoreText = "P1: " + to_string(scoreP1);
    SDL_Surface* p1ScoreSurface = TTF_RenderText_Blended(font, p1ScoreText.c_str(), black);
    SDL_Texture* p1ScoreTexture = SDL_CreateTextureFromSurface(renderer, p1ScoreSurface);
    SDL_Rect p1ScoreRect = {570, 240, p1ScoreSurface->w, p1ScoreSurface->h};
    SDL_RenderCopy(renderer, p1ScoreTexture, NULL, &p1ScoreRect);
    SDL_FreeSurface(p1ScoreSurface);
    SDL_DestroyTexture(p1ScoreTexture);

    if (gameMode==TWO_PLAYER){
            string p2ScoreText = "P2: " + to_string(scoreP2);
    SDL_Surface* p2ScoreSurface = TTF_RenderText_Blended(font, p2ScoreText.c_str(), black);
    SDL_Texture* p2ScoreTexture = SDL_CreateTextureFromSurface(renderer, p2ScoreSurface);
    SDL_Rect p2ScoreRect = {570, 270, p2ScoreSurface->w, p2ScoreSurface->h};
    SDL_RenderCopy(renderer, p2ScoreTexture, NULL, &p2ScoreRect);
    SDL_FreeSurface(p2ScoreSurface);
    SDL_DestroyTexture(p2ScoreTexture);

    }
    if (isBossLevel) {
        // Thông số thanh máu
        int barX = 50;
        int barY = 10;
        int barWidth = 300;
        int barHeight = 20;

        // Tính phần trăm máu boss còn lại
        int bossMaxLives = 20; // <-- gán đúng với số mạng gốc của boss
        float hpRatio = static_cast<float>(boss.remainingLives) / bossMaxLives;

        // Vẽ nền xám
        SDL_Rect backRect = { barX, barY, barWidth, barHeight };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // xám
        SDL_RenderFillRect(renderer, &backRect);

        // Vẽ thanh đỏ thể hiện máu
        SDL_Rect hpRect = { barX, barY, static_cast<int>(barWidth * hpRatio), barHeight };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // đỏ
        SDL_RenderFillRect(renderer, &hpRect);

        // Vẽ chữ "BOSS"
        SDL_Color white = {255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, "BOSS", white);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { barX + barWidth + 10, barY, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }


}


void renderGameOver(bool& gameRunning) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load ảnh Game Over
    SDL_Texture* gameOverImage = IMG_LoadTexture(renderer, "resource/image/gameOver.png");
    if (!gameOverImage) {
        std::cerr << "Failed to load gameOver.png: " << IMG_GetError() << std::endl;
        return;
    }

    int imgW, imgH;
    SDL_QueryTexture(gameOverImage, NULL, NULL, &imgW, &imgH);

    SDL_Rect dstRect = {
        SCREEN_WIDTH / 2 - imgW / 2,
        SCREEN_HEIGHT / 2 - imgH / 2,
        imgW,
        imgH
    };

    SDL_RenderCopy(renderer, gameOverImage, NULL, &dstRect);
    SDL_RenderPresent(renderer);

    // Phát âm thanh Game Over nếu có
    if (Mix_PlayChannel(-1, gameOverSound, 0) == -1) {
        std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
    }

    SDL_Delay(2000); // Đợi 2 giây

    SDL_DestroyTexture(gameOverImage); // Giải phóng texture

    restartGame();
}



void restartGame() {
    bool inRestart = true;
    int selectedOption = 0; // 0 = Continue, 1 = Exit
    SDL_Event event;
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};

    while (inRestart) {
        // Vẽ menu nền
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        string options[2] = {"Continue", "Exit"};
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

        // Xử lý phím
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inRestart = false;
                return;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + 2) % 2;
                        if (Mix_PlayChannel(-1, selectSound, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                        }
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % 2;
                        if (Mix_PlayChannel(-1, selectSound, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                        }
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selectedOption == 0) {
                            if (Mix_PlayChannel(-1, chooseSound2, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                            }
                            // Reset game
                            currentLevel = 1;
                            isBossLevel=false;
                            int enemiesSpawned = 0;
                            Uint32 lastSpawnTime = 0;
                            scoreP1=0;
                            scoreP2=0;
                            player1 = PlayerTank(9 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerOne.png");
                            player1.remainingLives = 3;
                            player1.bullets.clear();

                            if (gameMode == TWO_PLAYER) {
                                player2 = PlayerTank(5 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerTwo.png");
                                player2.remainingLives = 3;
                                player2.bullets.clear();
                            }

                            base.active = true;
                            generateWalls();
                            spawnEnemies();
                            inRestart = false;
                            running = true;
                        } else {
                            if (Mix_PlayChannel(-1, chooseSound2, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                            }
                            running = false;
                            inRestart = false;
                        }
                        break;
                }
            }
        }

        SDL_Delay(100);
    }
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
    for (auto it = wallExplosions.begin(); it != wallExplosions.end();) {
        if (it->isFinished()) {
            it = wallExplosions.erase(it);
        } else {
            it->render(renderer);
            ++it;
        }
    }
    for (auto it = tankExplosions.begin(); it != tankExplosions.end();) {
        if (it->isFinished()) {
            it = tankExplosions.erase(it);
        } else {
            it->render(renderer);
            ++it;
        }
    }
    for (auto it = bossExplosions.begin(); it != bossExplosions.end();) {
        if (it->isFinished()) {
            it = bossExplosions.erase(it);
        } else {
            it->render(renderer);
            ++it;
        }
    }
    if(isBossLevel){
      boss.render(renderer);
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

void generateBossLevel() {
    walls.clear();
    stones.clear();
    bushes.clear();
    water.clear();
    ice.clear();

    string filename = "resource/level/bossLevel.txt";
    ifstream file(filename);
    if (!file) {
        cerr << "Cannot open boss level file: " << filename << endl;
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
                        if (Mix_PlayChannel(-1, shootSound, 0) == -1) {
                            cerr << "Failed to play sound: " << Mix_GetError() << endl;
                        }
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
    if(isBossLevel){
        boss.update(/*&player1, &player2, gameMode*/);


        // Kiểm tra va chạm đạn player với boss
        for (auto& bullet : player1.bullets) {
            if (bullet.active && SDL_HasIntersection(&bullet.rect, &boss.rect)) {
                boss.remainingLives--;
                bullet.active = false;
                if (boss.remainingLives <= 0) {

                    boss.isActive = false;
                    scoreP1 += 500; // Thưởng điểm khi đánh bại boss
                }
            }
        }

        if(gameMode == TWO_PLAYER) {
            for (auto& bullet : player2.bullets) {
                if (bullet.active && SDL_HasIntersection(&bullet.rect, &boss.rect)) {
                    boss.remainingLives--;
                    bullet.active = false;
                    if (boss.remainingLives <= 0) {

                        boss.isActive = false;
                        scoreP2 += 500; // Thưởng điểm khi đánh bại boss
                    }
                }
            }
        }

        // Kiểm tra va chạm đạn boss với player
        for (auto& bullet : boss.bullets) {
            if (bullet->active && SDL_HasIntersection(&bullet->rect, &player1.rect)) {
                if(player1.remainingLives > 1) {
                    player1.remainingLives--;
                    bullet->active = false;
                } else {
                    renderGameOver(running);
                }
            }

            if(gameMode == TWO_PLAYER && bullet->active && SDL_HasIntersection(&bullet->rect, &player2.rect)) {
                if(player2.remainingLives > 1) {
                    player2.remainingLives--;
                    bullet->active = false;
                } else {
                    renderGameOver(running);
                }
            }

            // Kiểm tra va chạm với tường
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet->rect, &wall.rect)) {
                    wall.active = false;
                    bullet->active=false;
                    wallExplosions.emplace_back(renderer, wall.rect.x, wall.rect.y);
                    Mix_PlayChannel(-1, wallExplosions.back().explosionSound, 0);
                    break;
                }
            }

            // Kiểm tra va chạm với đá
            for (auto& stone : stones) {
                if (stone.active && SDL_HasIntersection(&bullet->rect, &stone.rect)) {
                    bullet->active = false;
                    break;
                }
            }

            // Kiểm tra va chạm với base
            if (bullet->active && SDL_HasIntersection(&bullet->rect, &base.rect)) {
                bullet->active = false;
                base.active = false;
                renderGameOver(running);
            }
        }

        for (auto& bigBullet : boss.bigBullets) {
                if (bigBullet->active && SDL_HasIntersection(&bigBullet->rect, &player1.rect)) {
                    if (player1.remainingLives > 1) {
                        player1.remainingLives--;
                        bigBullet->active = false;
                    } else {
                        renderGameOver(running);
                    }
                }

                if (gameMode == TWO_PLAYER && bigBullet->active && SDL_HasIntersection(&bigBullet->rect, &player2.rect)) {
                    if (player2.remainingLives > 1) {
                        player2.remainingLives--;
                        bigBullet->active = false;
                    } else {
                        renderGameOver(running);
                    }
                }

                // Va chạm với tường
                for (auto& wall : walls) {
                    if (wall.active && SDL_HasIntersection(&bigBullet->rect, &wall.rect)) {
                        wall.active = false;

                        wallExplosions.emplace_back(renderer, wall.rect.x, wall.rect.y);
                        Mix_PlayChannel(-1, wallExplosions.back().explosionSound, 0);
                        break;
                    }
                }

                // Va chạm với đá
                for (auto& stone : stones) {
                    if (stone.active && SDL_HasIntersection(&bigBullet->rect, &stone.rect)) {
                        bigBullet->active = false;
                        break;
                    }
                }

                // Va chạm với base
                if (bigBullet->active && SDL_HasIntersection(&bigBullet->rect, &base.rect)) {
                    bigBullet->active = false;
                    base.active = false;
                    renderGameOver(running);
                }
            }

        // Xóa đạn không active của boss
        boss.bullets.erase(std::remove_if(boss.bullets.begin(), boss.bullets.end(),
            [](const std::unique_ptr<Bullet>& b) { return !b->active; }), boss.bullets.end());
        boss.bigBullets.erase(std::remove_if(boss.bigBullets.begin(), boss.bigBullets.end(),
            [](const std::unique_ptr<BigBullet>& b) { return !b->active; }), boss.bigBullets.end());
        // Kiểm tra nếu boss bị tiêu diệt
        if (!boss.isActive) {
                bossExplosions.emplace_back(renderer, boss.rect.x, boss.rect.y);
                if (bossExplosionSound) {
                    Mix_PlayChannel(-1, bossExplosionSound, 0); // phát 1 lần
                }
                Uint32 explosionStart = SDL_GetTicks();
                const Uint32 explosionDuration = 1500;

                // Vòng lặp render vụ nổ
                while (SDL_GetTicks() - explosionStart < explosionDuration) {
                    SDL_Event e;
                    while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT) {
                            running = false;
                            return;
                        }
                    }

                    render(); // <- dùng chính hàm render() của bạn để vẽ toàn bộ frame
                    SDL_Delay(16); // ~60 FPS
                }
                //Tạo nền mờ (màu đen với độ trong suốt)
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Màu đen mờ
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &overlay);

            // Tải ảnh victory
                SDL_Texture* victoryImage = IMG_LoadTexture(renderer, "resource/image/victory.png");

                // Lấy kích thước ảnh
                int imgW, imgH;
                SDL_QueryTexture(victoryImage, NULL, NULL, &imgW, &imgH);

                // Tính vị trí để căn giữa
                SDL_Rect victoryRect = {
                    SCREEN_WIDTH / 2 - imgW / 2,
                    SCREEN_HEIGHT / 2 - imgH / 2,
                    imgW,
                    imgH
                };

                // Render ảnh
                SDL_RenderCopy(renderer, victoryImage, NULL, &victoryRect);


            //Tạo dòng mô tả nhỏ hơn
            TTF_Font* smallFont = TTF_OpenFont("PressStart2P.ttf", 24);
            SDL_Color white = {255, 255, 255, 255};

            SDL_Surface* descSurface = TTF_RenderText_Blended(smallFont, "You have defeated the final boss!", white);
            SDL_Texture* descTexture = SDL_CreateTextureFromSurface(renderer, descSurface);

            SDL_Rect descRect = {
                SCREEN_WIDTH/2 - descSurface->w/2,
                SCREEN_HEIGHT/2 + 30,
                descSurface->w,
                descSurface->h
            };
            SDL_RenderCopy(renderer, descTexture, NULL, &descRect);

            //Hiển thị hướng dẫn tiếp tục
            SDL_Surface* continueSurface = TTF_RenderText_Blended(smallFont, "Press ENTER to continue", white);
            SDL_Texture* continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);

            SDL_Rect continueRect = {
                SCREEN_WIDTH/2 - continueSurface->w/2,
                SCREEN_HEIGHT - 100,
                continueSurface->w,
                continueSurface->h
            };
            SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);

            //Cập nhật màn hình
            SDL_RenderPresent(renderer);

            //Phát âm thanh chiến thắng
            if (victorySound) {
                Mix_PlayChannel(-1, victorySound, 0);
            }

            //Chờ người chơi nhấn ENTER
            bool waiting = true;
            SDL_Event e;
            while (waiting && running) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                        waiting = false;
                    } else if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                            waiting = false;
                        }
                    }
                }
                SDL_Delay(16);
            }

            //Giải phóng bộ nhớ
            SDL_DestroyTexture(victoryImage);
            SDL_FreeSurface(descSurface);
            SDL_DestroyTexture(descTexture);
            SDL_FreeSurface(continueSurface);
            SDL_DestroyTexture(continueTexture);
            TTF_CloseFont(smallFont);

            running = false; // Thoát game
        }
    }
    player1.updateBullets();
    if(gameMode==TWO_PLAYER){
        player2.updateBullets();
    }

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
            // Hiệu ứng nổ tại vị trí tường
            wallExplosions.emplace_back(renderer, wall.rect.x, wall.rect.y);

            // Phát âm thanh nếu cần
            Mix_PlayChannel(-1, wallExplosions.back().explosionSound, 0);

            break;
        }
    }
}
//Đạn người chơi 2 bắn va chạm với tường
if(gameMode== TWO_PLAYER){
        for (auto& bullet : player2.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    // Hiệu ứng nổ tại vị trí tường
                    wallExplosions.emplace_back(renderer, wall.rect.x, wall.rect.y);

                    // Phát âm thanh nếu cần
                    Mix_PlayChannel(-1, wallExplosions.back().explosionSound, 0);

                    break;
                }
        }
    }

}
    for (auto& enemy : enemies) {

        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    // Hiệu ứng nổ tại vị trí tường
                    wallExplosions.emplace_back(renderer, wall.rect.x, wall.rect.y);

                    // Phát âm thanh nếu cần
                    Mix_PlayChannel(-1, wallExplosions.back().explosionSound, 0);

                    break;
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
//Người chơi bắn trúng kẻ địch

for (auto& bullet : player1.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            //hiệu ứng nổ xe tăng
            tankExplosions.emplace_back(renderer, enemy.rect.x, enemy.rect.y);
            Mix_PlayChannel(-1, tankExplosions.back().explosionSound, 0);
            scoreP1+=100;
            enemy.active = false;
            bullet.active = false;
        }
    }
}


if(gameMode== TWO_PLAYER){
for (auto& bullet : player2.bullets) {
    for (auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
            //hiệu ứng nổ xe tăng
            tankExplosions.emplace_back(renderer, enemy.rect.x, enemy.rect.y);
            Mix_PlayChannel(-1, tankExplosions.back().explosionSound, 0);
            scoreP2+=100;
            enemy.active = false;
            bullet.active = false;
        }
    }
}
}

//**************************************************************************************************************************************
//Xoá các xe tăng địch đã bị đánh dấu là chết
enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](EnemyTank& e) { return !e.active; }), enemies.end());


//Nếu bắn chết hết
if (enemies.empty() /*&& enemiesSpawned >= enemyNumber*/) {
        SDL_Delay(50);
    if (currentLevel < maxLevel) {
    currentLevel++;
    cout << "Level up! Now loading level\n " << currentLevel << endl;
    generateWalls();
    spawnEnemies();
    player1 = PlayerTank(9 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerOne.png");
    player1.bullets.clear();
    if(gameMode==TWO_PLAYER){
        player2 = PlayerTank(5 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerTwo.png");
        player2.bullets.clear();
    }
    base.active = true;
    }
        else if (!isBossLevel) {
        // Lần đầu đến boss
        isBossLevel = true;
        Mix_HaltMusic();
        bossBackgroundMusic = Mix_LoadMUS("resource/sound/bossTheme.mp3");
        Mix_PlayMusic(bossBackgroundMusic, -1);

        // Vẽ nền đen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu đen
        SDL_RenderClear(renderer);

        // Hiển thị dòng chữ "Entering BOSS LEVEL!"
        TTF_Font* font = TTF_OpenFont("PressStart2P.ttf", 32);
        if (font) {
            SDL_Color color = {255, 0, 0}; // Màu đỏ
            SDL_Surface* surface = TTF_RenderText_Solid(font, "Entering BOSS LEVEL!", color);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    SDL_Rect dstRect = {
                        SCREEN_WIDTH / 2 - surface->w / 2,
                        SCREEN_HEIGHT / 2 - surface->h / 2,
                        surface->w, surface->h
                    };
                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(2000); // Hiển thị trong 2 giây

                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
            TTF_CloseFont(font);
        }

        generateBossLevel(); // <-- Tạo hàm riêng để load map boss
        boss = Boss(TILE_SIZE * 5, TILE_SIZE *5, renderer);
        player1 = PlayerTank(9 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerOne.png");
        player1.bullets.clear();
        if(gameMode == TWO_PLAYER) {
            player2 = PlayerTank(5 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "resource/image/playerTwo.png");
            player2.bullets.clear();
        }
        base.active = true;
    }


   /* else {
        cout << "WIN GAME! YOU BEAT THE BOSS!" << endl;
        running = false;
    }*/

}//**************************************************************************************************************************************
//Dan ban vao nguoi choi
for (auto& enemy : enemies) {
    for (auto& bullet : enemy.bullets) {
        // Update
        if (SDL_HasIntersection(&bullet.rect, &player1.rect)) {
            if(player1.remainingLives>1){
                    player1.remainingLives--;
                    bullet.active=false;
            }else{
            renderGameOver(running);}

            return;
        }
    }
}
if(gameMode==TWO_PLAYER){
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            // Update
            if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
                if(player2.remainingLives>1){
                    player2.remainingLives--;
                    bullet.active=false;
            }else{
            renderGameOver(running);}
                return;
            }
        }
    }
}
//**************************************************************************************************************************************
//Đạn người chơi 1 và kẻ địch va chạm nhau
for (auto& bulletP : player1.bullets) {
    if (!bulletP.active) continue; // bỏ qua đạn đã vô hiệu

    // Kiểm tra với đạn enemy thường
    for (auto& enemy : enemies) {
        for (auto& bulletE : enemy.bullets) {
            if (!bulletE.active) continue;
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

