#include "Attack.h"
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>

Attack::Attack(const char* filePath, float x, float y, float speed) : sprite(sprite), x(x), y(y), speed(speed) {
    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        std::cerr << "Failed to load bullet sprite!" << std::endl;
        this->sprite = nullptr; // Upewniamy siê, ¿e sprite jest null, jeœli za³adowanie nie powiod³o siê
    }
    else {
        width = al_get_bitmap_width(sprite);
        height = al_get_bitmap_height(sprite);
    }
}

Attack::~Attack() {
    if (sprite) {
        al_destroy_bitmap(sprite);
        sprite = nullptr;  // Zapobiegamy wielokrotnemu niszczeniu tego samego wskaŸnika
    }
}

void Attack::move() {
    x += speed; // Pocisk leci w prawo
}

void Attack::draw() const {
    if (sprite && al_get_bitmap_width(sprite) > 0 && al_get_bitmap_height(sprite) > 0) {
        al_draw_bitmap(sprite, x, y, 0);
    }
    else {
        std::cerr << "Sprite not loaded or invalid size, skipping drawing." << std::endl;
    }
}

bool Attack::isOutOfBounds() const {
    return x > 1800; // Jeœli pocisk wyjdzie poza ekran
}

bool Attack::collidesWith(const Zombie& zombie) const {
    float zx = zombie.getX();
    float zy = zombie.getY();
    float zw = zombie.getWidth();
    float zh = zombie.getHeight();

    return !(x + width < zx || x > zx + zw || y + height < zy || y > zy + zh);
}
