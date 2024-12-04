#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include "Player.h"
#include "Zombie.h"

const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;

int main() {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return -1;
    }
    al_init_image_addon();
    al_init_font_addon();
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

    // Wbudowana czcionka
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        std::cerr << "Failed to create builtin font!" << std::endl;
        return -1;
    }

    ALLEGRO_BITMAP* background = al_load_bitmap("assets/background.png");
    if (!background) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }

    Player player("assets/player.png", 400, 500, 0.2);
    std::vector<std::unique_ptr<Zombie>> enemies;

    bool running = true, redraw = true, gameStarted = false, gameOver = false;
    int points = 0;
    int killCount = 0;

    float timeSurvived = 0;

    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (gameStarted && !gameOver) {
                redraw = true;

                // Liczenie czasu przetrwania i punktów
                timeSurvived += 1.0 / 60.0;
                points = static_cast<int>(timeSurvived) + killCount * 10;

                // Pobranie stanu klawiatury
                ALLEGRO_KEYBOARD_STATE keyState;
                al_get_keyboard_state(&keyState);

                if (al_key_down(&keyState, ALLEGRO_KEY_W)) player.move(0, -1);
                if (al_key_down(&keyState, ALLEGRO_KEY_S)) player.move(0, 1);
                if (al_key_down(&keyState, ALLEGRO_KEY_A)) player.move(-1, 0);
                if (al_key_down(&keyState, ALLEGRO_KEY_D)) player.move(1, 0);

                // Dodawanie nowych zombie co pewien czas
                static int frameCount = 0;
                frameCount++;
                if (frameCount % 120 == 0) { // Co 2 sekundy
                    float startY = rand() % SCREEN_HEIGHT;
                    enemies.push_back(std::make_unique<Zombie>("assets/zombie.png", static_cast<float>(SCREEN_WIDTH), static_cast<float>(startY), 0.3f));
                }

                // Aktualizacja pozycji zombie
                for (auto& enemy : enemies) {
                    float dx = player.getX() - enemy->getX();
                    float dy = player.getY() - enemy->getY();
                    float length = sqrt(dx * dx + dy * dy);
                    dx /= length;
                    dy /= length;
                    enemy->move(dx, dy);
                }

                // Sprawdzanie kolizji
                for (const auto& enemy : enemies) {
                    if (enemy->collidesWith(player)) {
                        gameOver = true;
                        break;
                    }
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (!gameStarted) {
                gameStarted = true;
            }
            else if (gameOver && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                // Restart gry
                gameStarted = false;
                gameOver = false;
                points = 0;
                killCount = 0;
                timeSurvived = 0;
                enemies.clear();
                player = Player("assets/player.png", 400, 500, 0.2); // Resetowanie gracza
            }
        }

        if (redraw && al_is_event_queue_empty(eventQueue)) {
            redraw = false;

            // Ekran startowy
            if (!gameStarted) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50,
                    ALLEGRO_ALIGN_CENTER, "Start Game");
                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20,
                    ALLEGRO_ALIGN_CENTER, "Press any key to start");
                al_flip_display();
                continue;
            }

            // Ekran końcowy
            if (gameOver) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100,
                    ALLEGRO_ALIGN_CENTER, "Game Over");
                al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
                    ALLEGRO_ALIGN_CENTER, "Points: %d", points);
                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100,
                    ALLEGRO_ALIGN_CENTER, "Press Enter to restart");
                al_flip_display();
                continue;
            }

            // Gra
            al_draw_scaled_bitmap(background, 0, 0,
                al_get_bitmap_width(background), al_get_bitmap_height(background),
                0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

            player.draw();

            for (const auto& enemy : enemies) {
                enemy->draw();
            }

            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Points: %d", points);

            al_flip_display();
        }
    }

    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);

    return 0;
}
