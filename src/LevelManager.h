#pragma once

#include "Enemies/EnemyManager.h"
#include <stdlib.h>
#include <vector>

namespace WizardGame {

class LevelManager {
public:
    LevelManager(Enemies::Manager&);

    void load(std::string const& path);
    void unload();

    void spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies);
    void reset_wave() { m_wave = 0; }
    void previous_wave() { m_wave--; }
    // TODO: Level background

private:
    Enemies::Manager& m_enemy_manager;

    struct EnemyData {
        size_t wave;
        Collider collider;
        Vec2 target_position;
        bool is_basic;
    };

    int m_level_id;
    size_t m_wave;
    std::vector<EnemyData> m_enemies;
};

}
