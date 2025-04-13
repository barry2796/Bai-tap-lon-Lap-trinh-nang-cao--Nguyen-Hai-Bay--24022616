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
    double angle; // Thêm góc xoay

    Bullet(double startX, double startY, int dirX, int dirY, SDL_Renderer* renderer) {
        x = startX;
        y = startY;
        dx = dirX;
        dy = dirY;
        active = true;

        // Load texture
        BulletTexture = IMG_LoadTexture(renderer, "Bullet2.png");
        if (!BulletTexture) {
            std::cerr << "Failed to load bullet texture: " << IMG_GetError() << std::endl;
        }

        rect = {x, y, BULLET_SIZE, BULLET_SIZE};

        // Gán góc xoay dựa trên hướng
        if (dx >0) angle = 90;
        else if (dx <0) angle = 270;
        else if (dy >0) angle = 180;
        else angle = 0;
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
            SDL_RenderCopyEx(renderer, BulletTexture, NULL, &rect, angle, nullptr, SDL_FLIP_NONE);
        }
    }
};

#endif
