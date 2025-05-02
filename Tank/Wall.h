#ifndef WALL_H
#define WALL_H
#include "Const.h"

using namespace std;

//Về cơ bản, các class trong Wall.h là tương đương nhau. Sự khác nhau chỉ rõ ràng trong logic game trong Game.h
class Wall  {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * wallTexture;

    Wall(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    wallTexture = IMG_LoadTexture(renderer, "resource/image/brick.png");
    active = true;
    rect = {x, y, WALL_TILE_SIZE, WALL_TILE_SIZE};
}


    void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, wallTexture, NULL, &rect);
    }
}

};
class Stone {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * stoneTexture;

    Stone(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    stoneTexture = IMG_LoadTexture(renderer, "resource/image/stone.png");
    active = true;
    rect = {x, y, WALL_TILE_SIZE, WALL_TILE_SIZE};
}


    void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, stoneTexture, NULL, &rect);
    }
}
};
class Bush  {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * bushTexture;

    Bush(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    bushTexture = IMG_LoadTexture(renderer, "resource/image/bush.png");
    active = true;
    rect = {x, y, WALL_TILE_SIZE, WALL_TILE_SIZE};
}


    void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, bushTexture, NULL, &rect);
    }
}
};
class Water {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * waterTexture;

    Water(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    waterTexture = IMG_LoadTexture(renderer, "resource/image/water.png");
    active = true;
    rect = {x, y, WALL_TILE_SIZE, WALL_TILE_SIZE};
}


    void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, waterTexture, NULL, &rect);
    }
}
};
class Ice   {


public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * iceTexture;

    Ice(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    iceTexture = IMG_LoadTexture(renderer, "resource/image/ice.png");
    active = true;
    rect = {x, y, WALL_TILE_SIZE, WALL_TILE_SIZE};
}


    void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, iceTexture, NULL, &rect);
    }
}
};
class Base {
public:
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;
    SDL_Texture* destroyedTexture;
    Base(){}

    Base(int x, int y, SDL_Renderer* renderer) {
        rect.x = x;
        rect.y = y;
        rect.w = TILE_SIZE;
        rect.h = TILE_SIZE;
        active = true;
        texture = loadTexture("resource/image/base.png", renderer);
        destroyedTexture = loadTexture("resource/image/base_destroyed.png", renderer);
    }

    void render(SDL_Renderer* renderer) {
        if (active) {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        }
        else {
            SDL_RenderCopy(renderer, destroyedTexture, NULL, &rect);
        }
    }

    SDL_Texture* loadTexture(const string& path, SDL_Renderer* renderer) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            cerr << "Failed to load base image: " << IMG_GetError() << endl;
            return nullptr;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return tex;
    }

};
#endif WALL_H
