#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "Player.h"

class Zombie {
public:
    Zombie(const char* filePath, float x, float y, float scale);
    ~Zombie();

    void move(float dx, float dy);
    void draw() const;
    bool collidesWith(const Player& player) const;

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    void takeDamage(int damage);
    bool isDead() const;
private:
    float x, y;
    float scale;
    ALLEGRO_BITMAP* sprite;
    float width, height;
    float health;

    ALLEGRO_BITMAP* healthBarFull;
    ALLEGRO_BITMAP* healthBarTwoThirds;
    ALLEGRO_BITMAP* healthBarOneThird;
};

#endif
