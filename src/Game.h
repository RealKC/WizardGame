#pragma once

#include "Bullet.h"
#include "Enemy.h"
#include "KeyboardState.h"
#include "LevelManager.h"
#include "SpriteManager.h"
#include "Player.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace WizardGame {

class Game {
public:
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 960;

    Game();
    ~Game();

    int run();

private:
    int initialize_sdl();

    void event_loop();
    void handle_player_keypresses(uint32_t current_time);

    void render_entities();
    void render_bullets();

    void update_bullet_positions();
    void check_collisions();
    void tick_enemies(uint32_t current_time);

    void kill_player();

    // Game state
    Player m_player;
    uint32_t m_last_bullet_shot_time;

    LevelManager m_level_manager;

    std::vector<Bullet> m_player_bullets;
    std::vector<Bullet> m_enemy_bullets;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    bool m_quit;

    // Render state
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    KeyboardState m_keyboard_state;
    SpriteManager m_sprite_manager;
};

}
