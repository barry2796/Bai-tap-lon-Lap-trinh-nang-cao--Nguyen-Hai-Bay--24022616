#ifndef TANK_H
#define TANK_H

#include <SDL.h>

class Tank {
protected:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;

public:
    Tank(int startX, int startY) {
        x = startX;
        y = startY;
        rect = {x, y, 40, 40};  // Giả sử TILESIZE = 40
        dirX = 0; dirY = -1;
    }

    void move() {}
    void render() {}
};

class PlayerTank : public Tank {
public:
    PlayerTank(int startX, int startY) : Tank(startX, startY) {}
};

class EnemyTank : public Tank {
public:
    EnemyTank(int startX, int startY) : Tank(startX, startY) {}
};

#endif // TANK_H
