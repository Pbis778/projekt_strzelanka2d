#include "Zombie.h"
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>

Zombie::Zombie(const char* filePath, float x, float y, float scale)
    : x(x), y(y), scale(scale) {
    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        std::cerr << "Failed to load zombie sprite: " << filePath << std::endl;
    }
    width = al_get_bitmap_width(sprite) * scale;
    height = al_get_bitmap_height(sprite) * scale;
}

Zombie::~Zombie() {
    if (sprite) {
        al_destroy_bitmap(sprite);
    }
}

void Zombie::move(float dx, float dy) {
    x += dx * 2.0f; // Prêdkoœæ zombie
    y += dy * 2.0f;
}

void Zombie::draw() const {
    if (sprite) {
        al_draw_scaled_bitmap(sprite, 0, 0,
            al_get_bitmap_width(sprite), al_get_bitmap_height(sprite),
            x, y, width, height, 0);
    }
}

bool Zombie::collidesWith(const Player& player) const {
    float playerX = player.getX();
    float playerY = player.getY();
    float playerWidth = player.getWidth();
    float playerHeight = player.getHeight();

    return !(x + width < playerX || x > playerX + playerWidth ||
        y + height < playerY || y > playerY + playerHeight);
}
