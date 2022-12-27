#pragma once

#include "Level.h"
#include <stdint.h>

namespace WizardGame {

class FileLevel final : public Level {
public:
    FileLevel(uint32_t level_event, std::string const& path);

private:
    // ^Level
    virtual void render_impl(SDL_Renderer*, TextRenderer&) override;
    virtual void run_frame_impl(uint32_t current_time) override;
    virtual void dismiss_dialogue_if_any() override;
    virtual void kill_player() override;

    void spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies);
    void reset_wave() { m_wave = 1; }
    void previous_wave() { m_wave--; }
    void next_wave() { m_wave++; }

    void parse_level(std::string const& path);

    struct EnemyData {
        size_t wave;
        Collider collider;
        Vec2 target_position;
        Enemies::Attack::Type attack;
        bool is_basic;
    };
    ;

    size_t m_wave;
    std::vector<EnemyData> m_enemy_infos;
};

}