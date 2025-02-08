#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

class Player {
public:
    Player(const char* filePath, float x, float y, float scale, unsigned int ammunition = 60, unsigned int ammoInMag = 30);
    ~Player();

    void move(float dx, float dy);
    void draw() const;
    void reset(const char* filePath, float x, float y, float scale);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    unsigned int getAmmunition() const;
    void setAmmunition(unsigned int _ammunition);
    unsigned int getAmmoInMag() const;
    void setAmmoInMag(unsigned int _ammoInMag);
    bool isMagazineEmpty() const;
    void reload();
    bool shoot();
    void handlePlayerMovement(const ALLEGRO_KEYBOARD_STATE& keyState, Player& player);


private:
    float x, y;
    float width, height;
    float scale;
    unsigned int ammunition;
    unsigned int ammoInMag;

    ALLEGRO_BITMAP* sprite;
};

#endif