#ifndef ATTACK_H
#define ATTACK_H

#include <allegro5/allegro.h>
#include "Zombie.h"


class Attack {
public:
    Attack(const char* filePath,float x, float y, float speed);
    ~Attack();

    void move();
    void draw() const;
    bool isOutOfBounds() const;
    bool collidesWith(const Zombie& zombie) const;

    float getX() const { return x; }
    float getY() const { return y; }

private:
    float x, y;
    float speed;
    float width, height;
    ALLEGRO_BITMAP* sprite;
};

#endif // !ATTACK_H

