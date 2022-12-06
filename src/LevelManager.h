#pragma once

#include "Collider.h"
#include "Enemies/Attack.h"
#include "Enemy.h"
#include "Vec2.h"
#include <memory>
#include <stdlib.h>
#include <vector>

namespace WizardGame {

class LevelManager {
public:
    LevelManager();

    void load(std::string const& path);
    void unload();

    void dump() const;

    void spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies);
    void reset_wave() { m_wave = 0; }
    void previous_wave() { m_wave--; }
    void next_wave() { m_wave++; }
    // TODO: Level background

private:
    struct EnemyData {
        size_t wave;
        Collider collider;
        Vec2 target_position;
        Enemies::Attack::Type attack;
        bool is_basic;
    };

    int m_level_id;
    size_t m_wave;
    std::vector<EnemyData> m_enemies;
};

}
