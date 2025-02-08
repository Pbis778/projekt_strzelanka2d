#ifndef AMMO_H
#define AMMO_H

#include <allegro5/allegro.h>
#include "Player.h"

class Ammo {
public:
    Ammo(const char* filePath, float x, float y, float scale);
    ~Ammo();

    void draw() const;
    bool collidesWith(const Player& player) const;

    float getX() const { return x; }
    float getY() const { return y; }

private:
    float x, y;
    float width, height;
    float scale;
    ALLEGRO_BITMAP* sprite;
};

#endif
