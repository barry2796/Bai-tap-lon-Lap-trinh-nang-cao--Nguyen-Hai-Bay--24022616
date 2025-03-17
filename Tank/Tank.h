#ifndef TANK_H
#define TANK_H
#include "Const.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Wall.h"
#include "Bullet.h"
using namespace std;
class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Texture* TankTexture=nullptr;
    SDL_Rect rect;
    vector<Bullet> bullets;
    SDL_Renderer* renderer;
    PlayerTank(SDL_Renderer* ren, int startX, int startY) {
    renderer = ren;
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = -1;  // Default direction up
    loadTexture(renderer, "D:/tap tanh code game/Bai tap lon Lap trinh nang cao- Nguyen Hai Bay- 24022616/Image/playerOne2.png");
}

    PlayerTank() {}
    ~PlayerTank() {
    if (TankTexture) {
        SDL_DestroyTexture(TankTexture);
        TankTexture = nullptr;
    }
}


    void move(int dx, int dy, const vector<Wall>& walls) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (int i = 0; i < walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return; // Prevent movement if colliding with a wall
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}


    void shoot() {
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
        this->dirX, this->dirY));
}

void updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](Bullet &b) { return !b.active; }), bullets.end());
}
    void loadTexture(SDL_Renderer*renderer,const char* filename) {
        SDL_Surface* tempSurface = IMG_Load(filename);
        if (!tempSurface) {
            std::cerr << "Không thể load ảnh: " << IMG_GetError() << std::endl;
            return;
        }
        TankTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }

void render(SDL_Renderer* renderer) {
    if (TankTexture) {
            SDL_RenderCopy(renderer, TankTexture, NULL, &rect);
        } else {
            // Nếu không có ảnh, vẽ hình chữ nhật mặc định
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
            }
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}


};

#endif // TANK_H
