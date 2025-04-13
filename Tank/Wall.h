#ifndef WALL_H
#define WALL_H
#include "Const.h"
const int WALL_TILE_SIZE = 20;
using namespace std;
class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * wallTexture;

    Wall(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    wallTexture = IMG_LoadTexture(renderer, "brick.png");
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
    stoneTexture = IMG_LoadTexture(renderer, "stone.png");
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
class Bush {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * bushTexture;

    Bush(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    bushTexture = IMG_LoadTexture(renderer, "bush.png");
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
    waterTexture = IMG_LoadTexture(renderer, "water.png");
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
class Ice {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture * iceTexture;

    Ice(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    iceTexture = IMG_LoadTexture(renderer, "ice.png");
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
#endif WALL_H
