#include "Player.h"
#include <allegro5/allegro_image.h>
#include <iostream>

Player::Player(const char* filePath, float x, float y, float scale, unsigned int ammunition, unsigned int ammoInMag)
    : x(x), y(y), scale(scale), ammunition(ammunition),ammoInMag(ammoInMag) {
    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        std::cerr << "Failed to load player sprite: " << filePath << std::endl;
    }
    width = al_get_bitmap_width(sprite) * scale;
    height = al_get_bitmap_height(sprite) * scale;
}

Player::~Player() {
    if (sprite) {
        al_destroy_bitmap(sprite);
    }
}

void Player::handlePlayerMovement(const ALLEGRO_KEYBOARD_STATE& keyState, Player& player) {
    if (al_key_down(&keyState, ALLEGRO_KEY_W)) player.move(0, -1);
    if (al_key_down(&keyState, ALLEGRO_KEY_S)) player.move(0, 1);
    if (al_key_down(&keyState, ALLEGRO_KEY_A)) player.move(-1, 0);
    if (al_key_down(&keyState, ALLEGRO_KEY_D)) player.move(1, 0);
}

void Player::move(float dx, float dy) {
    x += dx * 5.0f;
    y += dy * 5.0f;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + width > 1800) x = 1800 - width;
    if (y + height > 900) y = 900 - height;
}

void Player::draw() const
{
    if (sprite) {
        al_draw_scaled_bitmap(sprite, 0, 0,
            al_get_bitmap_width(sprite), al_get_bitmap_height(sprite),
            x, y, width, height, 0);
    }
}

void Player::reset(const char* filePath, float x, float y, float scale) {
    if (sprite) {
        al_destroy_bitmap(sprite);
        sprite = nullptr;
    }

    this->x = x;
    this->y = y;
    this->scale = scale;

    sprite = al_load_bitmap(filePath);
    if (!sprite) {
        return;
    }
    width = al_get_bitmap_width(sprite) * scale;
    height = al_get_bitmap_height(sprite) * scale;
}

bool Player::isMagazineEmpty() const {
    return ammoInMag == 0;
}

void Player::reload() {
    if (ammunition > 0) {
        unsigned int needed = 30 - ammoInMag;
        if (ammunition >= needed) {
            ammoInMag += needed;
            ammunition -= needed;
        }
        else {
            ammoInMag += ammunition;
            ammunition = 0;
        }
    }
}

bool Player::shoot() {
    if (ammoInMag > 0) {
        ammoInMag--;
        return true;
    }
    return false; 
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getWidth() const { return width; }
float Player::getHeight() const { return height; }
unsigned int Player::getAmmunition() const { return ammunition; }
void Player::setAmuuntion(unsigned int _ammunition) { ammunition = _ammunition; }
unsigned int Player::getAmmoInMag() const { return ammoInMag; }
void Player::setAmmoInMag(unsigned int _ammoInMag) { ammoInMag = _ammoInMag; }