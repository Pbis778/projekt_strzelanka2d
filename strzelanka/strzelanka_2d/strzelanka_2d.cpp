#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include "Player.h"
#include "Zombie.h"
#include "Attack.h"
#include "Ammo.h"

std::vector<std::unique_ptr<Attack>> bullets;
std::vector<std::unique_ptr<Ammo>> ammoPacks;

const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;


void clearAfterGameEnd(ALLEGRO_SAMPLE* backgroundMusic, ALLEGRO_BITMAP* background, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* eventQueue) {
    al_destroy_sample(backgroundMusic);
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
};

int main() {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return -1;
    }
    al_init_image_addon();
    al_init_font_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();

    if (!al_reserve_samples(10)) {
        std::cerr << "Failed to reserve samples!" << std::endl;
        return -1;
    }

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

    ALLEGRO_SAMPLE* backgroundMusic = al_load_sample("assets/background.wav");
    if (!backgroundMusic) {
        std::cerr << "Failed to load background music!" << std::endl;
        return -1;
    }
    ALLEGRO_SAMPLE* akShotSound = al_load_sample("assets/ak_shot_sound.wav");
    if (!akShotSound) {
        std::cerr << "Failed to load Ak Shot sound!" << std::endl;
        return -1;
    }
    ALLEGRO_SAMPLE* akReloadSound = al_load_sample("assets/ak47_reload_sound.wav");
    if (!akReloadSound) {
        std::cerr << "Failed to load Ak Reload sound!" << std::endl;
        return -1;
    }
    ALLEGRO_SAMPLE* ammoPickupSound = al_load_sample("assets/ammo_pickup_sound.flac");
    if (!ammoPickupSound) {
        std::cerr << "Failed to load Ammo Pickup sound!" << std::endl;
        return -1;
    }

    al_play_sample(backgroundMusic, 0.05, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, nullptr);

    Player player("assets/player.png", 400, 500, 0.15);
    std::vector<std::unique_ptr<Zombie>> enemies;

    bool running = true, redraw = true, gameStarted = false, gameOver = false, isPaused = false;
    int points = 0, killCount = 0;
    float timeSurvived = 0, ammoSpawnTimer = 0.0f;

    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (gameStarted && !gameOver && !isPaused) {
                redraw = true;

                ammoSpawnTimer += 1.0f / 60.0f;

                if (ammoSpawnTimer >= 15.0f) {
                    ammoSpawnTimer = 0.0f;

                    float ammoX = 0.0f;
                    float ammoY = static_cast<float>(rand() % (SCREEN_HEIGHT - 50));

                    ammoPacks.push_back(std::make_unique<Ammo>("assets/ammopack.png", ammoX, ammoY, 0.2f));
                }

                for (auto it = bullets.begin(); it != bullets.end();) {
                    if ((*it)->isOutOfBounds()) {
                        it = bullets.erase(it);
                    }
                    else {
                        (*it)->move();
                        (*it)->draw();
                        ++it;
                    }
                }

                for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
                    bool bulletRemoved = false;

                    for (auto zombieIt = enemies.begin(); zombieIt != enemies.end();) {
                        if ((*bulletIt)->collidesWith(**zombieIt)) {
                            (*zombieIt)->takeDamage(1);
                            if ((*zombieIt)->isDead()) {
                                zombieIt = enemies.erase(zombieIt);
                                killCount++;
                            }
                            bulletIt = bullets.erase(bulletIt);
                            bulletRemoved = true;
                            break;
                        }
                        else {
                            ++zombieIt;
                        }
                    }

                    if (!bulletRemoved) {
                        ++bulletIt;
                    }
                }

                for (auto it = ammoPacks.begin(); it != ammoPacks.end();) {
                    if ((*it)->collidesWith(player)) {
                        player.setAmmunition(player.getAmmunition() + 30);
                        it = ammoPacks.erase(it);
                        al_play_sample(ammoPickupSound, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                    }
                    else {
                        ++it;
                    }
                }


                timeSurvived += 1.0 / 60.0;
                points = static_cast<int>(timeSurvived) + killCount * 10;

                ALLEGRO_KEYBOARD_STATE keyState;
                al_get_keyboard_state(&keyState);

                player.handlePlayerMovement(keyState, player);

                static int frameCount = 0;
                frameCount++;
                if (frameCount % 120 == 0) {
                    float startY = rand() % SCREEN_HEIGHT;
                    enemies.push_back(std::make_unique<Zombie>("assets/zombie.png", static_cast<float>(SCREEN_WIDTH), static_cast<float>(startY), 0.3f));
                }

                for (auto& enemy : enemies) {
                    float dx = player.getX() - enemy->getX();
                    float dy = player.getY() - enemy->getY();
                    float length = sqrt(dx * dx + dy * dy);
                    dx /= length;
                    dy /= length;
                    enemy->move(dx, dy);
                }

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
                gameStarted = false;
                gameOver = false;
                points = 0;
                killCount = 0;
                timeSurvived = 0;

                enemies.clear();
                player.reset("assets/player.png", 400, 500, 0.15);
                player.setAmmoInMag(30);
                player.setAmmunition(60);
                gameStarted = true;
            }

            if (!isPaused) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    if (player.shoot()) {
                        al_play_sample(akShotSound, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                        bullets.push_back(std::make_unique<Attack>("assets/bullet.png", player.getX() + player.getWidth(), (player.getY() + player.getHeight() / 2) - 32, 10.0f, 0.3));
                    }
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                    player.reload();
                    al_play_sample(akReloadSound, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                isPaused = !isPaused;
                redraw = true;
            }
            
        }

        if (redraw && al_is_event_queue_empty(eventQueue)) {
            redraw = false;

            if (!gameStarted) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50,
                    ALLEGRO_ALIGN_CENTER, "Start Game");
                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20,
                    ALLEGRO_ALIGN_CENTER, "Press any key to start");
                al_flip_display();
                continue;
            }

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

            if (isPaused) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100,
                    ALLEGRO_ALIGN_CENTER, "PAUSE");
                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
                    ALLEGRO_ALIGN_CENTER, "Press ESCAPE to continue");
                al_flip_display();
                continue;
            }

            al_draw_scaled_bitmap(background, 0, 0,
                al_get_bitmap_width(background), al_get_bitmap_height(background),
                0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

            player.draw();

            for (const auto& enemy : enemies) {
                enemy->draw();
            }

            for (const auto& bullet : bullets) {
                bullet->draw();
            }

            for (const auto& ammoPack : ammoPacks) {
                ammoPack->draw();
            }

            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Points: %d", points);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 30, 0, "Ammo: %d/%d", player.getAmmoInMag(), player.getAmmunition());

            al_flip_display();
        }
    }

    clearAfterGameEnd(backgroundMusic, background, font, display, timer, eventQueue);

    return 0;
}