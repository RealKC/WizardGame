#pragma once

#include "Collider.h"
#include "Enemies/Attack.h"
#include "Enemy.h"
#include "KeyboardState.h"
#include "Player.h"
#include "Vec2.h"
#include <memory>
#include <stdlib.h>
#include <vector>

namespace WizardGame {

class Level {
public:
    Level();

    void load(std::string const& path);
    void unload();

    void dump() const;

    void run_frame(uint32_t current_time);
    void render(SDL_Renderer*);

private:
    void spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies);
    void reset_wave() { m_wave = 0; }
    void previous_wave() { m_wave--; }
    void next_wave() { m_wave++; }

    void update_bullet_positions();
    void check_collisions();
    void tick_enemies(uint32_t current_time);

    void handle_player_keypresses(uint32_t current_time);

    void kill_player();

    void render_bullets(SDL_Renderer*);
    void render_entities(SDL_Renderer*);

    struct EnemyData {
        size_t wave;
        Collider collider;
        Vec2 target_position;
        Enemies::Attack::Type attack;
        bool is_basic;
    };

    KeyboardState m_keyboard_state;

    int m_level_id;
    size_t m_wave;
    std::vector<EnemyData> m_enemy_infos;

    Player m_player;
    uint32_t m_last_bullet_shot_time;

    std::vector<Bullet> m_player_bullets;
    std::vector<Bullet> m_enemy_bullets;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
};

}
