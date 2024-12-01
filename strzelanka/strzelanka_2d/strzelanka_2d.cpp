#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "Player.h"

const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;

int main() {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return -1;
    }
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        std::cerr << "Failed to create display!" << std::endl;
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    ALLEGRO_BITMAP* background = al_load_bitmap("assets/background.png");
    if (!background) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }

    Player player("assets/player.png", 400, 500, 0.2); // Skalowanie gracza na 50% oryginalnego rozmiaru
    bool running = true, redraw = true;

    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;

            // Pobranie stanu klawiatury
            ALLEGRO_KEYBOARD_STATE keyState;
            al_get_keyboard_state(&keyState);

            if (al_key_down(&keyState, ALLEGRO_KEY_W)) player.move(0, -1);
            if (al_key_down(&keyState, ALLEGRO_KEY_S)) player.move(0, 1);
            if (al_key_down(&keyState, ALLEGRO_KEY_A)) player.move(-1, 0);
            if (al_key_down(&keyState, ALLEGRO_KEY_D)) player.move(1, 0);
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        if (redraw && al_is_event_queue_empty(eventQueue)) {
            redraw = false;

            // Przeskalowanie i rysowanie mapy
            al_draw_scaled_bitmap(
                background,
                0, 0,
                al_get_bitmap_width(background), al_get_bitmap_height(background),
                0, 0,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                0
            );

            // Rysowanie gracza
            player.draw();

            al_flip_display();
        }
    }

    al_destroy_bitmap(background);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);

    return 0;
}
