#ifndef WALL_H
#define WALL_H
#include "Const.h"

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
    rect = {x, y, TILE_SIZE, TILE_SIZE};
}


    void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, wallTexture, NULL, &rect);
    }
}

};
#endif WALL_H
