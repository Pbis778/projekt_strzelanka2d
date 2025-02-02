#include "Ammo.h"
#include <allegro5/allegro_image.h>
#include <iostream>

Ammo::Ammo(const char* filePath, float x, float y, float scale)
    : x(x), y(y), scale(scale), sprite(nullptr) {
    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        std::cerr << "Failed to load ammo sprite: " << filePath << std::endl;
    }
    else {
        width = al_get_bitmap_width(sprite) * scale;
        height = al_get_bitmap_height(sprite) * scale;
    }
}

Ammo::~Ammo() {
    if (sprite) {
        al_destroy_bitmap(sprite);
    }
}

void Ammo::draw() const {
    if (sprite) {
        al_draw_scaled_bitmap(sprite, 0, 0,
            al_get_bitmap_width(sprite), al_get_bitmap_height(sprite),
            x, y, width, height, 0);
    }
}

bool Ammo::collidesWith(const Player& player) const {
    float px = player.getX();
    float py = player.getY();
    float pw = player.getWidth();
    float ph = player.getHeight();

    return !(x + width < px || x > px + pw || y + height < py || y > py + ph);
}
