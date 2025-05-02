#ifndef BOSS_H
#define BOSS_H

#include "Const.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Wall.h"
#include "Bullet.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define MAP_LEFT_BOUND (TILE_SIZE)
#define MAP_TOP_BOUND (TILE_SIZE)
#define MAP_RIGHT_BOUND ((MAP_WIDTH - 6) * TILE_SIZE)
#define MAP_BOTTOM_BOUND ((MAP_HEIGHT - 1) * TILE_SIZE)

#include <memory> // Thêm dòng này
enum Direction { UP, DOWN, LEFT, RIGHT };
class Boss {
public:
    int remainingLives = 2;
    bool isActive = true;
    int x, y;
    SDL_Rect rect;
    SDL_Texture* textureUp = nullptr;

    Direction dir = UP;

    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<BigBullet>> bigBullets;
    Uint32 lastShootTime = 0;
    Uint32 shootCooldown = 2000; // 2 giây
    Uint32 lastDirectionChangeTime = 0;
    Uint32 directionChangeCooldown = 2500; // 2.5 giây

    SDL_Renderer* renderer;

    Boss(int startX, int startY, SDL_Renderer* ren) : renderer(ren) {
        x = startX;
        y = startY;

        // Khởi tạo thời gian ngay khi tạo boss
        lastShootTime = SDL_GetTicks();
        lastDirectionChangeTime = SDL_GetTicks();

        SDL_Surface* tempSurface = IMG_Load("resource/image/boss.png");
        if (!tempSurface) {
            printf("Failed to load surface: %s\n", IMG_GetError());
        } else {
            SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 255, 255, 255));
            textureUp = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        }

        SDL_SetTextureBlendMode(textureUp, SDL_BLENDMODE_BLEND);
        rect = { x, y, TILE_SIZE * 5, TILE_SIZE * 5 };

        srand(static_cast<unsigned int>(time(0)));
    }

    void shoot() {
            int cx = rect.x + rect.w / 2;
            int cy = rect.y + rect.h / 2;
            int offset = 30; // khoảng cách giữa 2 viên đạn nhỏ

            auto createSmallBullet = [&](int bx, int by, int dx, int dy) {
                bullets.emplace_back(std::make_unique<Bullet>(bx, by, dx, dy, renderer));
            };

            // Đạn nhỏ – 2 viên mỗi hướng
            createSmallBullet(cx - offset, rect.y, 0, -1); // lên
            createSmallBullet(cx + offset, rect.y, 0, -1);

            createSmallBullet(cx - offset, rect.y + rect.h, 0, 1); // xuống
            createSmallBullet(cx + offset, rect.y + rect.h, 0, 1);

            createSmallBullet(rect.x, cy - offset, -1, 0); // trái
            createSmallBullet(rect.x, cy + offset, -1, 0);

            createSmallBullet(rect.x + rect.w, cy - offset, 1, 0); // phải
            createSmallBullet(rect.x + rect.w, cy + offset, 1, 0);

            // Đạn to – bắn theo hướng nòng quay
    int bigBulletX = 0, bigBulletY = 0;
    int bigDx = 0, bigDy = 0;
    int distance = 40; // khoảng cách đạn xích ra khỏi boss

    switch (dir) {
        case UP:
            bigDx = 0; bigDy = -1;
            bigBulletX = cx;
            bigBulletY = rect.y - distance;
            break;
        case RIGHT:
            bigDx = 1; bigDy = 0;
            bigBulletX = rect.x + rect.w + distance;
            bigBulletY = cy;
            break;
        case DOWN:
            bigDx = 0; bigDy = 1;
            bigBulletX = cx;
            bigBulletY = rect.y + rect.h + distance;
            break;
        case LEFT:
            bigDx = -1; bigDy = 0;
            bigBulletX = rect.x - distance;
            bigBulletY = cy;
            break;
    }

    bigBullets.emplace_back(std::make_unique<BigBullet>(bigBulletX, bigBulletY, bigDx, bigDy, renderer));

    // Quay nòng theo thời gian (vòng tròn)
    dir = static_cast<Direction>((dir + 1) % 4);
}


    void update() {
        Uint32 now = SDL_GetTicks();

        // Xử lý đổi hướng độc lập với bắn đạn
        if (now - lastDirectionChangeTime > directionChangeCooldown) {
            dir = static_cast<Direction>(rand() % 4);
            lastDirectionChangeTime = now;
        }

        // Xử lý bắn đạn độc lập với đổi hướng
        if (now - lastShootTime > shootCooldown) {
            shoot();
            lastShootTime = now;
        }

        // Cập nhật đạn
        for (auto& bullet : bullets) {
            bullet->move();
        }
        for (auto& bigBullet : bigBullets) {
            bigBullet->move();
        }

        // Xóa đạn không còn active
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<Bullet>& b) { return !b->active; }), bullets.end());
    }

    void render(SDL_Renderer* renderer) {
        if (!isActive) return;

        double angle = 0;
        switch (dir) {
        case UP: angle = 0; break;
        case RIGHT: angle = 90; break;
        case DOWN: angle = 180; break;
        case LEFT: angle = 270; break;
        }

        SDL_RenderCopyEx(renderer, textureUp, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);

        for (auto& bullet : bullets) {
            bullet->render(renderer);
        }
        for (auto& bigBullet : bigBullets) {
            bigBullet->render(renderer);
        }
    }
};

#endif //BOSS_H
