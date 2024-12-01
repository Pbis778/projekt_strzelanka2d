#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Player {
private:
    ALLEGRO_BITMAP* sprite;
    float x, y;
    float speed;
    float scale; // Wspó³czynnik skalowania

public:
    Player(const char* textureFile, float startX, float startY, float scale = 1.0f);
    ~Player();
    void move(float dx, float dy);
    void draw();
    float getX() const;
    float getY() const;
    ALLEGRO_BITMAP* getBitmap() const;
};

#endif // PLAYER_H
