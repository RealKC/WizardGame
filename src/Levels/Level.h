#pragma once

#include "../KeyboardState.h"
#include "../SpriteManager.h"
#include "../TextRenderer.h"
#include "../UI/AbstractLevelMenu.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace WizardGame {

class Level {
public:
    static constexpr int PLAYING_AREA_BOTTOM_LIMIT = 940;
    static constexpr int PLAYING_AREA_RIGHT_LIMIT = 940;
    static constexpr int PLAYING_AREA_TOP_LIMIT = 20;
    static constexpr int PLAYING_AREA_LEFT_LIMIT = 20;

    virtual ~Level() { }

    void run_frame(uint32_t current_time);
    void render(SDL_Renderer*, TextRenderer&, SpriteManager&);

    void toggle_pause_state();

    void handle_key_event(SDL_KeyboardEvent);

protected:
    Level(uint32_t level_event, Vec2 spawn);

    virtual void run_frame_impl(uint32_t current_time) = 0;
    virtual void render_impl(SDL_Renderer*, TextRenderer&, SpriteManager&) = 0;
    virtual void dismiss_dialogue_if_any() = 0;
    virtual BackgroundId background_id() const = 0;
    virtual void kill_player() = 0;
    virtual void restart_level();
    virtual void scancode_hook(SDL_Scancode) { }
    virtual int next_level() const = 0;
    virtual bool has_been_won() const = 0;

    void render_bullets(SDL_Renderer*, SpriteManager&);
    void render_entities(SDL_Renderer*, SpriteManager&);
    void render_dialog(SDL_Renderer*, TextRenderer&, SpriteManager&, PortraitId, std::string const& speaker, std::string const& speech);

    void update_bullet_positions();
    void check_collisions();
    void tick_enemies(uint32_t current_time);

    void handle_player_keypresses(uint32_t current_time);

    void set_title(std::string const& title) { m_title = title; }

    Player m_player;

    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Bullet> m_player_bullets;
    std::vector<Bullet> m_enemy_bullets;

private:
    bool is_paused() const { return m_menu != nullptr && m_menu->can_be_toggled(); }
    bool has_next_level() const { return next_level() > 0; }

    void increase_score_by(std::int64_t units, std::int64_t enemy_modifier, std::int64_t player_modifier) { m_score += units * enemy_modifier * player_modifier; }

    uint32_t const m_level_event;

    KeyboardState m_keyboard_state;

    uint32_t m_last_bullet_shot_time;

    std::string m_title;
    std::int64_t m_score;
    std::unique_ptr<UI::AbstractLevelMenu> m_menu;
};
} // WizardGame
