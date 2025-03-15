#include <iostream>
#include <SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

class Game {
public:
    SDL_Renderer* renderer;
    SDL_Window* window;
    bool chayCtrinh;

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

    void render() {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < MAP_HEIGHT - 1; i++) {
            for (int j = 0; j < MAP_WIDTH - 1; j++) {
                SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &tile);
            }
        }
        SDL_RenderPresent(renderer);
    }

    void chay() {
        while (chayCtrinh) {
            render();
            SDL_Delay(16);
        }
    }

    ~Game() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();  //  Sửa lỗi ở đây
    }
};
