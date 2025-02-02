#include "Zombie.h"
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>

Zombie::Zombie(const char* filePath, float x, float y, float scale)
    : x(x), y(y), scale(scale), health(3) {
    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        std::cerr << "Failed to load zombie sprite: " << filePath << std::endl;
    }
    width = al_get_bitmap_width(sprite) * scale;
    height = al_get_bitmap_height(sprite) * scale;

    healthBarFull = al_load_bitmap("assets/fullHealthbar.png");
    healthBarOneThird = al_load_bitmap("assets/OneOfThreeHealthBar.png");
    healthBarTwoThirds = al_load_bitmap("assets/TwoOfThreeHealthBar.png");
}

Zombie::~Zombie() {
    if (sprite) {
        al_destroy_bitmap(sprite);
    }
    if (healthBarOneThird) {
        al_destroy_bitmap(healthBarOneThird);
    }
    if (healthBarFull) {
        al_destroy_bitmap(healthBarFull);
    }
    if (healthBarTwoThirds) {
        al_destroy_bitmap(healthBarTwoThirds);
    }
}

void Zombie::move(float dx, float dy) {
    x += dx * 2.0f;
    y += dy * 2.0f;
}

void Zombie::draw() const {
    if (sprite) {
        al_draw_scaled_bitmap(sprite, 0, 0,
            al_get_bitmap_width(sprite), al_get_bitmap_height(sprite),
            x, y, width, height, 0);
    }

    ALLEGRO_BITMAP* currentZombieHealth = nullptr;
    if (health == 3) {
        currentZombieHealth = healthBarFull;
    }
    else if (health == 2) {
        currentZombieHealth = healthBarTwoThirds;
    }
    else if (health == 1) {
        currentZombieHealth = healthBarOneThird;
    }

    if (currentZombieHealth) {
        float healthBarWidth = width * 0.8f;
        float healthBarHeight = 44.0f;
        float healthBarX = x + (width - healthBarWidth) / 2;
        float healthBarY = y - 24;

        al_draw_scaled_bitmap(currentZombieHealth, 0, 0,
            al_get_bitmap_width(currentZombieHealth), al_get_bitmap_height(currentZombieHealth),
            healthBarX, healthBarY, healthBarWidth, healthBarHeight, 0);
    }
}

void Zombie::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Zombie::isDead() const {
    return health <= 0;
}

bool Zombie::collidesWith(const Player& player) const {
    float playerX = player.getX();
    float playerY = player.getY();
    float playerWidth = player.getWidth();
    float playerHeight = player.getHeight();

    return !(x + width < playerX || x > playerX + playerWidth ||
        y + height < playerY || y > playerY + playerHeight);
}
