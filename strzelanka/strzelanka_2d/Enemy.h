#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "Player.h"

class Enemy {
private:
    ALLEGRO_BITMAP* sprite;
    float x, y;
    float speed;

public:
    Enemy(const char* textureFile, float startX, float startY);
    ~Enemy();
    void move(float dx, float dy);
    void draw() const;
    float getX() const;
    float getY() const;
    bool collidesWith(const Player& player) const;
};

#endif // ENEMY_H