#include "Attack.h"
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>

Attack::Attack(const char* filePath, float x, float y, float speed)
    : x(x), y(y), speed(speed), sprite(nullptr), width(0), height(0) {
    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        std::cerr << "Failed to load bullet sprite: " << filePath << std::endl;
    }
    else {
        width = al_get_bitmap_width(sprite);
        height = al_get_bitmap_height(sprite);
    }
}

Attack::~Attack() {
    if (sprite) {
        al_destroy_bitmap(sprite);
        sprite = nullptr;
    }
}

void Attack::move() {
    x += speed; // Pocisk porusza siê w prawo
}

void Attack::draw() const {
    if (sprite) {
        al_draw_bitmap(sprite, x, y, 0);
    }
    else {
        std::cerr << "Attempted to draw a bullet without a valid sprite." << std::endl;
    }
}

bool Attack::isOutOfBounds() const {
    return x > 1800; // Za granic¹ ekranu
}

bool Attack::collidesWith(const Zombie& zombie) const {
    float zx = zombie.getX();
    float zy = zombie.getY();
    float zw = zombie.getWidth();
    float zh = zombie.getHeight();

    // Sprawdzenie, czy prostok¹ty siê nak³adaj¹
    return !(x + width < zx || x > zx + zw || y + height < zy || y > zy + zh);
}
