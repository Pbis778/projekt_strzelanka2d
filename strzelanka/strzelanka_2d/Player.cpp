#include "Player.h"
#include <stdexcept>

Player::Player(const char* textureFile, float startX, float startY) : x(startX), y(startY), speed(5.0f) {
    sprite = al_load_bitmap(textureFile);
    if (!sprite) {
        throw std::runtime_error("Failed to load player texture!");
    }
}

Player::~Player() {
    al_destroy_bitmap(sprite);
}

void Player::move(float dx, float dy) {
    x += dx * speed;
    y += dy * speed;
}

void Player::draw() {
    al_draw_bitmap(sprite, x, y, 0);
}

float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

ALLEGRO_BITMAP* Player::getBitmap() const {
    return sprite;
}
