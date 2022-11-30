#include "LevelManager.h"

#include "Utils.h"
#include <fstream>
#include <sstream>

namespace WizardGame {
LevelManager::LevelManager(Enemies::Manager& enemy_manager)
    : m_enemy_manager(enemy_manager)
    , m_level_id(0)
    , m_wave(0)
{
}

void LevelManager::load(std::string const& path)
{
    std::ifstream level(path);
    std::string line;

    size_t wave = 0;
    while (std::getline(level, line)) {
        if (line.find("wave") == 0) {
            wave = atoi(line.c_str() + 4);
        } else if (line.find("basic")) {
            std::stringstream stream(line.substr(strlen("basic")));

            int x, y, w, h;
            stream >> x >> y >> w >> h;
            Collider collider(x, y, w, h);

            stream >> x >> y;
            Vec2 target_position;

            m_enemies.push_back(EnemyData {
                wave,
                collider,
                target_position,
                true
            });
        } else if (line.find("boss")) {
            // TODO: Parse adrian
        } else {
            error() << "Cannot parse line: '" << line << "'\n";
        }
    }
}

void LevelManager::unload()
{
    m_level_id = 0;
    m_wave = 0;
    m_enemies.clear();
}

void LevelManager::spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies)
{
    for (auto& enemy : m_enemies) {
        if (m_wave == enemy.wave) {
            if (enemy.is_basic) {
                enemies.push_back(m_enemy_manager.basic(enemy.collider, enemy.target_position));
            } else {
                int phase = m_level_id;
                if (phase < 1)
                    phase = 1;
                if (phase > 2)
                    phase = 2;

                enemies.push_back(m_enemy_manager.adrian(enemy.collider, enemy.target_position, phase));
            }
        }
    }
    ++m_wave;
}

}
