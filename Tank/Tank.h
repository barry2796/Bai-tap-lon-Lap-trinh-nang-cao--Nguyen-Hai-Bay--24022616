#ifndef TANK_H
#define TANK_H
#include "Const.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Wall.h"
#include "Bullet.h"
#include <algorithm>
using namespace std;
class PlayerTank {
public:
    int x, y;

    int dirX, dirY;

    SDL_Rect rect;

    vector<Bullet> bullets;

    SDL_Texture* textureUp;

    SDL_Texture* textureRight;

    SDL_Texture* textureDown;

    SDL_Texture* textureLeft;

    enum Direction { UP, DOWN, LEFT, RIGHT };

    Direction dir;





    PlayerTank(int startX, int startY,SDL_Renderer* renderer) {
        x = startX;
        y = startY;
        textureUp = IMG_LoadTexture(renderer, "playerTankUp.png");

        rect = {x, y, TILE_SIZE*2, TILE_SIZE*2};
        dirX = 0;
        dirY = -1; // Default direction up                                                                                                                                                                                                                                     texture = loadTexture("player_tank.png",renderer);
    }
    PlayerTank() {}





    void move(int dx, int dy, const vector<Wall>& walls) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE*2, TILE_SIZE*2 };
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

    if (dx > 0) dir = RIGHT;
    else if (dx < 0) dir = LEFT;
    else if (dy > 0) dir = DOWN;
    else if (dy < 0) dir = UP;
}






    void shoot(SDL_Renderer* renderer) {
    bullets.push_back(Bullet(x + TILE_SIZE-7 , y + TILE_SIZE-7 ,
        this->dirX, this->dirY,renderer));
}





void updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
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

    SDL_RenderCopyEx(renderer, textureUp, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);


    //SDL_RenderCopy(renderer, textureUp, NULL, &rect);
    //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_RenderFillRect(renderer, &rect);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}







};

#endif // TANK_H
