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
    SDL_Texture* enemyTankTexture;
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction dir;


    EnemyTank(int startX, int startY,SDL_Renderer* renderer) {
        moveDelay = 15; // Delay for movement
        shootDelay = 5; // Delay for shooting
        x = startX;
        y = startY;
        enemyTankTexture = IMG_LoadTexture(renderer,"enemyTank.png");
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = 1;
        active = true;
    }



 /*
void move(const vector<Wall>& walls) {
    if (--moveDelay > 0) return; // đợi đến khi hết delay mới cho di chuyển

    moveDelay = 60; // càng cao thì tank càng di chuyển chậm (ví dụ 30 là chậm vừa phải)


    // Random chọn hướng
    int direction = rand() % 4;
    int dx = 0, dy = 0;

    switch (direction) {
        case 0: dy = -5; break; // UP
        case 1: dy = 5; break;  // DOWN
        case 2: dx = -5; break; // LEFT
        case 3: dx = 5; break;  // RIGHT
    }

    // Thử di chuyển như hàm move() thường
    int newX = x + dx;
    int newY = y + dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (int i = 0; i < walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return; // Va vào tường -> không di chuyển
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }

    // Cập nhật hướng
    dirX = dx;
    dirY = dy;

    if (dx < 0) dir = RIGHT;
    else if (dx > 0) dir = LEFT;
    else if (dy < 0) dir = DOWN;
    else if (dy > 0) dir = UP;
}
*/


void moveTowardPlayer(int playerX, int playerY, const vector<Wall>& walls) {
    if (--moveDelay > 0) return;

    moveDelay = 60; // delay càng cao thì tank càng chậm

    int dx = 0, dy = 0;

    // Ưu tiên hướng có khoảng cách lớn hơn
    if (abs(playerX - x) > abs(playerY - y)) {
        dx = (playerX > x) ? 5 : -5;
    } else {
        dy = (playerY > y) ? 5 : -5;
    }

    // kiểm tra va chạm với tường
    int newX = x + dx;
    int newY = y + dy;
    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };

    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
            return; // va tường, không di chuyển
        }
    }

    // cập nhật vị trí
    x = newX;
    y = newY;
    rect.x = x;
    rect.y = y;
    dirX = dx;
    dirY = dy;

    if (dx < 0) dir = RIGHT;
    else if (dx > 0) dir = LEFT;
    else if (dy < 0) dir = DOWN;
    else if (dy > 0) dir = UP;
}




    void shoot(SDL_Renderer* renderer) {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                             this->dirX, this->dirY,renderer));
}

void updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void render(SDL_Renderer* renderer) {

    double angle = 0;
    switch (dir) {
        case UP: angle = 0; break;
        case RIGHT: angle = 90; break;
        case DOWN: angle = 180; break;
        case LEFT: angle = 270; break;
    }

    SDL_RenderCopyEx(renderer, enemyTankTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);


    //SDL_RenderCopy(renderer, textureUp, NULL, &rect);
    //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_RenderFillRect(renderer, &rect);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

};
#endif



