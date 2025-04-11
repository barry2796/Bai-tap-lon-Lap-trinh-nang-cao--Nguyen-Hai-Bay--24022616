#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Const.h"

#define BULLET_SIZE 10

class Bullet {
public:
    int x, y;
    int dx, dy;
    SDL_Rect rect;
    bool active;
    SDL_Texture* BulletTexture;

    Bullet(int startX, int startY, int dirX, int dirY, SDL_Renderer* renderer) {
        x = startX;
        y = startY;
        dx = dirX;
        dy = dirY;
        active = true;
        BulletTexture = IMG_LoadTexture(renderer, "Bullet2.png");
        if (!BulletTexture) {
            std::cerr << "Failed to load bullet texture: " << IMG_GetError() << std::endl;
        }
        rect = {x, y, BULLET_SIZE, BULLET_SIZE};
    }

    void move() {
        x += dx;
        y += dy;
        rect.x = x;
        rect.y = y;

        if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE ||
            y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE) {
            active = false;
        }
    }

    void render(SDL_Renderer* renderer) {
        if (active) {
            SDL_RenderCopy(renderer, BulletTexture, NULL, &rect);
        }
    }


};

#endif
