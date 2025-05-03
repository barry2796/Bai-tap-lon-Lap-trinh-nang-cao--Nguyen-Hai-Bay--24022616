#ifndef CONST_H
#define CONST_H
#define MAP_LEFT_BOUND (TILE_SIZE)
#define MAP_TOP_BOUND (TILE_SIZE)
#define MAP_RIGHT_BOUND ((MAP_WIDTH - 6) * TILE_SIZE)
#define MAP_BOTTOM_BOUND ((MAP_HEIGHT - 1) * TILE_SIZE)
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;
const int WALL_TILE_SIZE = 20;
enum GameMode {
    ONE_PLAYER,
    TWO_PLAYER
};
#endif // CONST_H
