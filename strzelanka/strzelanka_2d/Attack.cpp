#include "Attack.h"
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>

Attack::Attack(const char* filePath, float x, float y, float speed, float scale)
    : x(x), y(y), speed(speed), sprite(nullptr),scale(scale) {
    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        std::cerr << "Failed to load bullet sprite: " << filePath << std::endl;
    }
    else {
        width = al_get_bitmap_width(sprite) * scale;
        height = al_get_bitmap_height(sprite) * scale;
    }
}

Attack::~Attack() {
    if (sprite) {
        al_destroy_bitmap(sprite);
        sprite = nullptr;
    }
}

void Attack::move() {
    x += speed;
}

void Attack::draw() const {
    if (sprite) {
        al_draw_scaled_bitmap(sprite, 0, 0,
            al_get_bitmap_width(sprite), al_get_bitmap_height(sprite),
            x, y, width, height, 0);
    }
    else {
        std::cerr << "Attempted to draw a bullet without a valid sprite." << std::endl;
    }
}

bool Attack::isOutOfBounds() const {
    return x > 1800;
}

bool Attack::collidesWith(const Zombie& zombie) const {
    float zx = zombie.getX();
    float zy = zombie.getY();
    float zw = zombie.getWidth();
    float zh = zombie.getHeight();

    return !(x + width < zx || x > zx + zw || y + height < zy || y > zy + zh);
}
