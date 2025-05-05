#ifndef TANK_H
#define TANK_H
#include "Const.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Wall.h"
#include "Bullet.h"
#include <algorithm>
#include "EnemyTank.h"


using namespace std;
class PlayerTank {
public:
    int remainingLives=3;

    bool isActive=true;
    int x, y;

    int dirX, dirY;

    SDL_Rect rect;

    vector<Bullet> bullets;


    SDL_Texture* textureUp;


    enum Direction { UP, DOWN, LEFT, RIGHT };

    Direction dir;

    Uint32 lastShootTime = 0;     // thời điểm bắn cuối

    static const Uint32 shootCooldown = 500;



    PlayerTank(int startX, int startY, SDL_Renderer* renderer,const char* imagePath) {
    x = startX;
    y = startY;

    // Load ảnh từ file vào surface
    SDL_Surface* tempSurface = IMG_Load(imagePath);
    if (!tempSurface) {
        printf("Failed to load surface: %s\n", IMG_GetError());
    }

    // Set colorkey để làm trong suốt nền đen
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0, 0));

    // Tạo texture từ surface sau khi đã xử lý colorkey
    textureUp = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Blend để hỗ trợ trong suốt
    SDL_SetTextureBlendMode(textureUp, SDL_BLENDMODE_BLEND);

    rect = { x, y, TILE_SIZE - 1, TILE_SIZE - 1 };
    dirX = 0;
    dirY = -1; // Default direction up
}

    PlayerTank() {}




    bool isHidden;
    bool isIce;
void move(int dx, int dy, const vector<Wall>& walls, const vector<Stone>& stones, const vector<Bush>& bushes,const vector<Ice>& ices,const vector<Water>& watered,const Base base, const vector<EnemyTank>& enemies) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = { newX+2, newY+2, TILE_SIZE - 4, TILE_SIZE - 4 };

    // Kiểm tra va chạm với tường
    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
            return;
        }
    }

    // Kiểm tra va chạm với đá
    for (const auto& stone : stones) {
        if (stone.active && SDL_HasIntersection(&newRect, &stone.rect)) {
            return;
        }
    }
    //kiem tra di chuyen tren bang
    this->isIce = false;
for (const auto& ice : ices) {
    if (ice.active && SDL_HasIntersection(&newRect, &ice.rect)) {
        this->isIce = true;
        break;
    }
}

    //kiem tra di chuyen duoi nuoc
    for (const auto& water : watered) {
        if (water.active && SDL_HasIntersection(&newRect, &water.rect)) {
            return;
        }
    }
//Kiểm tra va chạm Base
if (base.active && SDL_HasIntersection(&newRect, &base.rect)) {
        return;
}
//kiem tra va cham ke dich
    for (const auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&newRect, &enemy.rect)) {
            return;
        }
    }
    // Giới hạn vùng di chuyển trong phần bản đồ
    if (newX < TILE_SIZE || newY < TILE_SIZE ||
        newX + TILE_SIZE >= SCREEN_WIDTH - TILE_SIZE * 6 ||
        newY + TILE_SIZE >= SCREEN_HEIGHT - TILE_SIZE) {
        return;
    }

    x = newX;
    y = newY;
    rect.x = x;
    rect.y = y;

    // Kiểm tra bụi
    isHidden = false;
    for (const auto& bush : bushes) {
        if (bush.active && SDL_HasIntersection(&newRect, &bush.rect)) {
            isHidden = true;
            break;
        }
    }

    // Cập nhật hướng
    if (dx > 0) dir = RIGHT;
    else if (dx < 0) dir = LEFT;
    else if (dy > 0) dir = DOWN;
    else if (dy < 0) dir = UP;
}







    void shoot(SDL_Renderer* renderer) {
    Uint32 currentTime = SDL_GetTicks(); // thời gian hiện tại (ms)

    // Nếu chưa đủ thời gian nạp đạn thì không bắn
    if (currentTime - lastShootTime < shootCooldown) {
        return;
    }

    // Cập nhật thời gian bắn
    lastShootTime = currentTime;

    // Tạo đạn
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
        this->dirX, this->dirY, renderer));
}






void updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
        [](Bullet &b) { return !b.active; }), bullets.end());
}






void render(SDL_Renderer* renderer) {
   if(isActive){

        double angle = 0;
        switch (dir) {
            case UP: angle = 0; break;
            case RIGHT: angle = 90; break;
            case DOWN: angle = 180; break;
            case LEFT: angle = 270; break;
        }

        SDL_RenderCopyEx(renderer, textureUp, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);

        for (auto& bullet : bullets) {
            bullet.render(renderer);
        }
   }
}







};

#endif // TANK_H
