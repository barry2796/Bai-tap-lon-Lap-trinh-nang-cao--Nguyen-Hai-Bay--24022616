#   ifndef ENEMYTANK_H
#define ENEMYTANK_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include "const.h"
using namespace std;
class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    vector<Bullet> bullets;

    EnemyTank(int startX, int startY) {
        moveDelay = 15; // Delay for movement
        shootDelay = 5; // Delay for shooting
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = 1;
        active = true;
    }

    void move(const vector<Wall>& walls) {
    if (--moveDelay > 0) return;
    moveDelay = 15;
    int r = rand() % 4;
    if (r == 0) { // Up
        this->dirX = 0;
        this->dirY = -5;
    }
    else if (r == 1) { // Down
        this->dirX = 0;
        this->dirY = 5;
    }
    else if (r == 2) { // Left
        this->dirY = 0;
        this->dirX = -5;
    }
    else if (r == 3) { // Right
        this->dirY = 0;
        this->dirX = 5;
    }

}
    void shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 5;
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

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}


};
#endif



