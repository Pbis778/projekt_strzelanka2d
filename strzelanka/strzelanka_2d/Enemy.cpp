#include "Enemy.h"
#include <stdexcept>

Enemy::Enemy(const char* textureFile, float startX, float startY) : x(startX), y(startY), speed(3.0f) {
    sprite = al_load_bitmap(textureFile);
    if (!sprite) {
        throw std::runtime_error("Failed to load enemy texture!");
    }
}

Enemy::~Enemy() {
    al_destroy_bitmap(sprite);
}

void Enemy::move(float dx, float dy) {
    x += dx * speed;
    y += dy * speed;
}

void Enemy::draw() const {
    al_draw_bitmap(sprite, x, y, 0);
}

float Enemy::getX() const {
    return x;
}

float Enemy::getY() const {
    return y;
}

bool Enemy::collidesWith(const Player& player) const {
    float playerX = player.getX();
    float playerY = player.getY();
    float playerWidth = al_get_bitmap_width(player.getBitmap());
    float playerHeight = al_get_bitmap_height(player.getBitmap());

    float enemyWidth = al_get_bitmap_width(sprite);
    float enemyHeight = al_get_bitmap_height(sprite);

    return !(playerX > x + enemyWidth ||  playerX + playerWidth < x ||
        playerY > y + enemyHeight || playerY + playerHeight < y);
}