#pragma once

#include "../Bullet.h"
#include "../Enemy.h"
#include "../KeyboardState.h"
#include "../Player.h"
#include "../TextRenderer.h"
#include "../UI/PauseMenu.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace WizardGame {

class Level {
public:
    void run_frame(uint32_t current_time);
    void render(SDL_Renderer*, TextRenderer&);

    void toggle_pause_state();

    void handle_key_event(SDL_KeyboardEvent);

protected:
    explicit Level(uint32_t level_event);

    virtual void run_frame_impl(uint32_t current_time) = 0;
    virtual void render_impl(SDL_Renderer*, TextRenderer&) = 0;
    virtual void dismiss_dialogue_if_any() = 0;
    virtual void kill_player() = 0;

    void render_bullets(SDL_Renderer*);
    void render_entities(SDL_Renderer*);

    void update_bullet_positions();
    void check_collisions();
    void tick_enemies(uint32_t current_time);

    void handle_player_keypresses(uint32_t current_time);

    uint32_t const m_level_event;

    KeyboardState m_keyboard_state;

    Player m_player;
    uint32_t m_last_bullet_shot_time;

    std::vector<Bullet> m_player_bullets;
    std::vector<Bullet> m_enemy_bullets;
    std::vector<std::unique_ptr<Enemy>> m_enemies;

private:
    bool is_paused() const { return m_pause_menu != nullptr; }

    std::unique_ptr<UI::PauseMenu> m_pause_menu;
};
} // WizardGame
