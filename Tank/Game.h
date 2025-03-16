#include <iostream>
#include <SDL.h>
#include <vector>
#include "Wall.h"
#include "Const.h"

class Game {
public:
    SDL_Renderer* renderer;
    SDL_Window* window;
    bool chayCtrinh;
    std::vector<Wall> walls;
    Game() {
        chayCtrinh = true;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Lỗi rồi, không chạy được: " << SDL_GetError() << std::endl;
            chayCtrinh = false;
        }

        window = SDL_CreateWindow("Tank-Bai tap giua ki", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Lỗi rồi, không chạy được: " << SDL_GetError() << std::endl;
            chayCtrinh = false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Lỗi rồi, không chạy được: " << SDL_GetError() << std::endl;
            chayCtrinh = false;
        }
    }
    void generateWalls();
    void render() {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 1; i < MAP_HEIGHT-1 ; i++) {
            for (int j = 1; j < MAP_WIDTH-1; j++) {
                SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &tile);
            }
        }
        SDL_RenderPresent(renderer);
    }

    void chay(){
    SDL_Event event;
    while (chayCtrinh){
        while (SDL_PollEvent(&event)) {  // Xử lý sự kiện
            if (event.type == SDL_QUIT) {  // Nếu nhấn nút đóng cửa sổ
                chayCtrinh = false;
            }
        }

        render(); // Vẽ màn hình

        SDL_Delay(16);  // Chờ một chút để tránh tốn CPU
    }
}

    ~Game() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();  //  Sửa lỗi ở đây
    }
};
