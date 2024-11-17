#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Player {
private:
    ALLEGRO_BITMAP* sprite;
    float x, y;
    float speed;

public:
    Player(const char* textureFile, float startX, float startY);
    ~Player();
    void move(float dx, float dy);
    void draw();
    float getX() const;
    float getY() const;
    ALLEGRO_BITMAP* getBitmap() const;
};

#endif // PLAYER_H
