#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

class Player {
public:
    Player(const char* filePath, float x, float y, float scale);
    ~Player();

    void move(float dx, float dy);
    void draw() const;
    void reset(const char* filePath, float x, float y, float scale);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

private:
    float x, y;
    float width, height;
    float scale;
    ALLEGRO_BITMAP* sprite;
};

#endif