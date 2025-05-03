#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Const.h"

#include <cmath>

#define BULLET_SIZE 10
const int speed = 5;
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
        BulletTexture = IMG_LoadTexture(renderer, "resource/image/Bullet2.png");
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
        x += dx*speed;
        y += dy*speed;
        rect.x = x;
        rect.y = y;

        if (x < MAP_LEFT_BOUND || x + BULLET_SIZE > MAP_RIGHT_BOUND ||
    y < MAP_TOP_BOUND || y + BULLET_SIZE > MAP_BOTTOM_BOUND) {
    active = false;
}

    }

    void render(SDL_Renderer* renderer) const {
        if (active) {
            SDL_RenderCopyEx(renderer, BulletTexture, NULL, &rect, angle, nullptr, SDL_FLIP_NONE);
        }
    }
};

class BigBullet {
public:
    int x, y;
    int dx, dy;
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;
    double angle;
    int speed = 4; // chậm hơn đạn thường
    int size = BULLET_SIZE * 3; // to hơn đạn thường

    BigBullet(double startX, double startY, int dirX, int dirY, SDL_Renderer* renderer) {
        x = startX;
        y = startY;
        dx = dirX;
        dy = dirY;
        active = true;

        // Tải texture
        texture = IMG_LoadTexture(renderer, "resource/image/BigBullet.png");
        if (!texture) {
            std::cerr << "Failed to load big bullet texture: " << IMG_GetError() << std::endl;
        }

        // Thiết lập texture blend mode để có thể điều chỉnh alpha (độ trong suốt)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        // Đảm bảo nền trắng trở thành trong suốt (alpha channel)
        SDL_Surface* tempSurface = IMG_Load("resource/image/BigBullet.png");
        if (tempSurface) {
            SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 255, 255, 255)); // Đặt màu trắng là trong suốt
            texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface); // Giải phóng surface tạm
        } else {
            std::cerr << "Failed to load surface for BigBullet: " << SDL_GetError() << std::endl;
        }

        rect = { x, y, size, size };

        // Gán góc xoay dựa trên hướng
        if (dx > 0) angle = 90;
        else if (dx < 0) angle = 270;
        else if (dy > 0) angle = 180;
        else angle = 0;
    }

    void move() {
        x += dx * speed;
        y += dy * speed;
        rect.x = x;
        rect.y = y;

        // Biến mất khi ra khỏi màn hình
        if (x < MAP_LEFT_BOUND || x + size > MAP_RIGHT_BOUND ||
    y < MAP_TOP_BOUND || y + size > MAP_BOTTOM_BOUND) {
    active = false;
}

    }

    void render(SDL_Renderer* renderer) const {
        if (active) {
            SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, nullptr, SDL_FLIP_NONE);
        }
    }
};


class wallExplosion {
public:
    wallExplosion(SDL_Renderer* renderer, int x, int y)
        : startTime(SDL_GetTicks()), duration(500), frameCount(5), currentFrame(0), frameHeight(30)
    {
        explosionTexture = IMG_LoadTexture(renderer, "resource/image/wallExplosion.png");
        //explosionSound = Mix_LoadWAV("resource/sound/wallExplosionSound.wav");

        dstRect = { x, y,  WALL_TILE_SIZE,  WALL_TILE_SIZE };
        srcRect = { 0, 0,  30,  30 };

        frameDelay = duration / frameCount;

        // Phát âm thanh nếu muốn
        if (explosionSound) {
            Mix_PlayChannel(-1, explosionSound, 0);
        }
    }

    ~wallExplosion() {
        if (explosionTexture) SDL_DestroyTexture(explosionTexture);
        if (explosionSound) Mix_FreeChunk(explosionSound);
    }

    void render(SDL_Renderer* renderer) {
        Uint32 elapsed = SDL_GetTicks() - startTime;

        if (elapsed >= duration) return;

        currentFrame = elapsed / frameDelay;
        srcRect.y = currentFrame * frameHeight;

        SDL_RenderCopy(renderer, explosionTexture, &srcRect, &dstRect);
    }

    bool isFinished() const {
        return SDL_GetTicks() - startTime >= duration;
    }

    Mix_Chunk* getSound() const {
        return explosionSound;
    }
    Mix_Chunk* explosionSound = nullptr;
private:
    SDL_Texture* explosionTexture = nullptr;

    SDL_Rect dstRect;
    SDL_Rect srcRect;

    Uint32 startTime;
    Uint32 duration;
    int frameCount;
    int currentFrame;
    int frameHeight;
    int frameDelay;
};
class tankExplosion {
public:
    tankExplosion(SDL_Renderer* renderer, int x, int y)
        : startTime(SDL_GetTicks()), duration(500), frameCount(7), currentFrame(0), frameHeight(58)
    {
        explosionTexture = IMG_LoadTexture(renderer, "resource/image/tankExplosion.png");
        explosionSound = Mix_LoadWAV("resource/sound/wallExplosionSound.wav");

        dstRect = { x, y,  TILE_SIZE,  TILE_SIZE };
        srcRect = { 0, 0,  58,  58 };

        frameDelay = duration / frameCount;

        // Phát âm thanh nếu muốn
        if (explosionSound) {
            Mix_PlayChannel(-1, explosionSound, 0);
        }
    }

    ~tankExplosion() {
        if (explosionTexture) SDL_DestroyTexture(explosionTexture);
        if (explosionSound) Mix_FreeChunk(explosionSound);
    }

    void render(SDL_Renderer* renderer) {
        Uint32 elapsed = SDL_GetTicks() - startTime;

        if (elapsed >= duration) return;

        currentFrame = elapsed / frameDelay;
        srcRect.y = currentFrame * frameHeight;

        SDL_RenderCopy(renderer, explosionTexture, &srcRect, &dstRect);
    }

    bool isFinished() const {
        return SDL_GetTicks() - startTime >= duration;
    }

    Mix_Chunk* getSound() const {
        return explosionSound;
    }
    Mix_Chunk* explosionSound = nullptr;
private:
    SDL_Texture* explosionTexture = nullptr;

    SDL_Rect dstRect;
    SDL_Rect srcRect;

    Uint32 startTime;
    Uint32 duration;
    int frameCount;
    int currentFrame;
    int frameHeight;
    int frameDelay;
};

class bossExplosion {
public:
    bossExplosion(SDL_Renderer* renderer, int x, int y)
        : startTime(SDL_GetTicks()), duration(300), frameCount(12), currentFrame(0), frameWidth(96)
    {
        explosionTexture = IMG_LoadTexture(renderer, "resource/image/bossExplosion.png");
        explosionSound = Mix_LoadWAV("resource/sound/bossExplosionSound.wav");

        dstRect = { x, y, TILE_SIZE * 5, TILE_SIZE * 5 };
        srcRect = { 0, 0, frameWidth, frameWidth }; // Giả sử sprite vuông

        frameDelay = duration / frameCount;

        if (explosionSound) {
            Mix_PlayChannel(-1, explosionSound, 0);
        }
    }

    ~bossExplosion() {
        if (explosionTexture) SDL_DestroyTexture(explosionTexture);
        if (explosionSound) Mix_FreeChunk(explosionSound);
    }

    void render(SDL_Renderer* renderer) {
        Uint32 elapsed = SDL_GetTicks() - startTime;

        if (elapsed >= duration) return;

        currentFrame = elapsed / frameDelay;
        srcRect.x = currentFrame * frameWidth; // Thay đổi từ y sang x

        SDL_RenderCopy(renderer, explosionTexture, &srcRect, &dstRect);
    }

    bool isFinished() const {
        return SDL_GetTicks() - startTime >= duration;
    }

    Mix_Chunk* getSound() const {
        return explosionSound;
    }
    Mix_Chunk* explosionSound = nullptr;
private:
    SDL_Texture* explosionTexture = nullptr;


    SDL_Rect dstRect;
    SDL_Rect srcRect;

    Uint32 startTime;
    Uint32 duration;
    int frameCount;
    int currentFrame;
    int frameWidth; // Đổi từ frameHeight sang frameWidth
    int frameDelay;
};
#endif
