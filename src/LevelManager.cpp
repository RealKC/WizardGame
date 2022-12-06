#include "LevelManager.h"

#include "Utils.h"
#include "Enemies/Attack.h"
#include <fstream>
#include <sstream>

namespace WizardGame {

using Enemies::Attack;

static Attack::Type parse_attack(char ch)
{
    switch (ch) {
    case 'c':
        return Attack::Type::Circle;
    case 'l':
        return Attack::Type::Line;
    case 't':
        return Attack::Type::ThreeAtOnce;
    }
}

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
        } else if (line.find("basic") == 0) {
            std::stringstream stream(line.substr(strlen("basic")));

            char attack_ch;
            stream >> attack_ch;
            Attack::Type attack = parse_attack(attack_ch);

            int x, y, w, h;
            stream >> x >> y >> w >> h;
            Collider collider { x, y, w, h };

            stream >> x >> y;
            Vec2 target_position { x, y };
            info() << "Target position is: " <<target_position.to_string() << std::endl;

            m_enemies.push_back(EnemyData {
                wave,
                collider,
                target_position,
                attack,
                true
            });
        } else if (line.find("boss") == 0) {
            // TODO: Parse adrian
        } else {
            error() << "Cannot parse line: '" << line << "'\n";
        }
    }
}

void LevelManager::dump() const {
    info() << "m_level_id: " << m_level_id << std::endl;
    info() << "m_wave: " << m_wave << std::endl;

    for (auto& enemy : m_enemies) {
        info() << "enemy: ->" << enemy.target_position.to_string() << ", " << enemy.collider.to_string() << ", basic? " << enemy.is_basic << " ";
    }
    info() << std::endl;
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
                Attack attack {enemy.attack, 2400, 0};
                enemies.push_back(std::make_unique<Enemies::Basic>(enemy.collider, enemy.target_position, std::vector { attack }));
            } else {
                int phase = m_level_id;
                if (phase < 1)
                    phase = 1;
                if (phase > 2)
                    phase = 2;

//                enemies.push_back(m_enemy_manager.adrian(enemy.collider, enemy.target_position, phase));
            }
        }
    }
    ++m_wave;
}

}
